#include "sim_iic.h"
#include "iic.h"


#define SET_SDA(ops, val)   ops->set_sda(ops->data, val)
#define SET_SCL(ops, val)   ops->set_scl(ops->data, val)
#define GET_SDA(ops)        ops->get_sda(ops->data)
#define GET_SCL(ops)        ops->get_scl(ops->data)

e_inline void sim_iic_delay(struct sim_iic_operations *ops)
{
    ops->udelay(ops->delay_us);
}

e_inline void sim_iic_delay2(struct sim_iic_operations *ops)
{
    ops->udelay((ops->delay_us+1)>>1);
}

#define SDA_L(ops)      SET_SDA(ops, 0)
#define SDA_H(ops)      SET_SDA(ops, 1)
#define SCL_L(ops)      SET_SCL(ops, 0)


static e_err_t SCL_H(struct sim_iic_operations *ops)
{
    e_tick_t start;
    
    SET_SCL(ops, 1);
    
    if(!ops->get_scl)
        goto done;
    
    start = (!GET_SCL(ops));
    while(!GET_SCL(ops))
    {
        if(get_ticks()-start>ops->timeout)
            return -E_ETIMEOUT;
        delay_us((ops->timeout+1)>>1);
    }
    
done:
    sim_iic_delay(ops);
    
    return E_EOK;
}

static void sim_iic_start(struct sim_iic_operations *ops)
{
    SDA_L(ops);
    sim_iic_delay(ops);
    SCL_L(ops);
    
}

static void sim_iic_restart(struct sim_iic_operations *ops)
{
    SDA_H(ops);
    SCL_H(ops);
    sim_iic_delay(ops);
    SDA_L(ops);
    sim_iic_delay(ops);
    SCL_L(ops);
}

static void sim_iic_stop(struct sim_iic_operations *ops)
{
    SDA_L(ops);
    sim_iic_delay(ops);
    SCL_H(ops);
    sim_iic_delay(ops);
    SDA_H(ops);
    sim_iic_delay2(ops);
}

e_inline e_bool_t sim_iic_waitack(struct sim_iic_operations *ops)
{
    e_bool_t ack;

    SDA_H(ops);
    sim_iic_delay(ops);

    if (SCL_H(ops) < 0)
    {
        //bit_dbg("wait ack timeout\n");

        return -E_ETIMEOUT;
    }

    ack = !GET_SDA(ops);    /* ACK : SDA pin is pulled low */

    SCL_L(ops);

    return ack;
}

static e_int32_t sim_iic_writeb(struct iic_dev *bus, e_uint8_t data)
{
    e_int32_t i;
    e_uint8_t bit;
    
    struct sim_iic_operations *ops = bus->priv;
    
    for(i=7;i>=0;i--)
    {
        SCL_L(ops);
        bit = (data>>i)&1;
        SET_SDA(ops, bit);
        sim_iic_delay(ops);
        if (SCL_H(ops) < 0)
        {
            //bit_dbg("sim_iic_writeb: 0x%02x, "
            //        "wait scl pin high timeout at bit %d\n",
             //       data, i);

            return -E_ETIMEOUT;
        }
    }
    SCL_L(ops);
    sim_iic_delay(ops);

    return sim_iic_waitack(ops);
}

static e_int32_t sim_iic_readb(struct iic_dev *bus)
{
    e_uint8_t i;
    e_uint8_t data = 0;
    struct sim_iic_operations *ops = bus->priv;
    
    SDA_H(ops);
    sim_iic_delay(ops);
    for (i = 0; i < 8; i++)
    {
        data <<= 1;

        if (SCL_H(ops) < 0)
        {
            return -E_ETIMEOUT;
        }

        if (GET_SDA(ops))
            data |= 1;
        SCL_L(ops);
        sim_iic_delay2(ops);
    }

    return data;
}

static e_size_t sim_iic_send_bytes(struct iic_dev *bus,
                                    struct iic_msg  *msg)
{
    e_int32_t ret;
    e_size_t bytes = 0;
    const e_uint8_t *ptr = msg->buf;
    e_int32_t count = msg->len;
    e_uint16_t ignore_nack = msg->flags&IIC_IGNORE_NACK;
    
    while(count>0)
    {
        ret = sim_iic_writeb(bus, *ptr);
        
        if ((ret > 0) || (ignore_nack && (ret == 0)))
        {
            count --;
            ptr ++;
            bytes ++;
        }
        else if (ret == 0)
        {
            //sim_iic_dbg("send bytes: NACK.\n");

            return 0;
        }
        else
        {
            //sim_iic_dbg("send bytes: error %d\n", ret);

            return ret;
        }
    }

    return bytes;
}

static e_err_t sim_iic_send_ack_or_nack(struct iic_dev *bus, int ack)
{
    struct sim_iic_operations *ops = bus->priv;
    
    if (ack)
        SET_SDA(ops, 0);
    sim_iic_delay(ops);
    if (SCL_H(ops) < 0)
    {
        //bit_dbg("ACK or NACK timeout\n");

        return -E_ETIMEOUT;
    }
    SCL_L(ops);

    return E_EOK;
}

static e_size_t sim_iic_recv_bytes(struct iic_dev *bus,
                                struct iic_msg        *msg)
{
    e_int32_t val;
    e_int32_t bytes = 0;   /* actual bytes */
    e_uint8_t *ptr = msg->buf;
    e_int32_t count = msg->len;
    const e_uint32_t flags = msg->flags;

    while (count > 0)
    {
        val = sim_iic_readb(bus);
        if (val >= 0)
        {
            *ptr = val;
            bytes ++;
        }
        else
        {
            break;
        }

        ptr ++;
        count --;

        //bit_dbg("recieve bytes: 0x%02x, %s\n",
        //        val, (flags & IIC_NO_READ_ACK) ?
        //        "(No ACK/NACK)" : (count ? "ACK" : "NACK"));
        if (!(flags & IIC_NO_READ_ACK))
        {
            val = sim_iic_send_ack_or_nack(bus, count);
            if (val < 0)
                return val;
        }
    }

    return bytes;
}

static e_int32_t sim_iic_send_address(struct iic_dev *bus,
                                   e_uint8_t                addr,
                                   e_int32_t                retries)
{
    struct sim_iic_operations *ops = bus->priv;
    e_int32_t i;
    e_err_t ret = 0;

    for (i = 0; i <= retries; i++)
    {
        ret = sim_iic_writeb(bus, addr);
        if (ret == 1 || i == retries)
            break;
        //bit_dbg("send stop condition\n");
        sim_iic_stop(ops);
        sim_iic_delay2(ops);
        //bit_dbg("send stae condition\n");
        sim_iic_start(ops);
    }

    return ret;
}

static e_err_t sim_iic_bit_send_address(struct iic_dev *bus,
                                     struct iic_msg        *msg)
{
    e_uint16_t flags = msg->flags;
    e_uint16_t ignore_nack = msg->flags & IIC_IGNORE_NACK;
    struct sim_iic_operations *ops = bus->priv;

    e_uint8_t addr1, addr2;
    e_int32_t retries;
    e_err_t ret;

    retries = ignore_nack ? 0 : bus->retries;

    if (flags & IIC_ADDR_10BIT)
    {
        addr1 = 0xf0 | ((msg->addr >> 7) & 0x06);
        addr2 = msg->addr & 0xff;

        //bit_dbg("addr1: %d, addr2: %d\n", addr1, addr2);

        ret = sim_iic_send_address(bus, addr1, retries);
        if ((ret != 1) && !ignore_nack)
        {
            //bit_dbg("NACK: sending first addr\n");

            return -E_EIO;
        }

        ret = sim_iic_writeb(bus, addr2);
        if ((ret != 1) && !ignore_nack)
        {
            //bit_dbg("NACK: sending second addr\n");

            return -E_EIO;
        }
        if (flags & IIC_RD)
        {
            //bit_dbg("send repeated start condition\n");
            sim_iic_restart(ops);
            addr1 |= 0x01;
            ret = sim_iic_send_address(bus, addr1, retries);
            if ((ret != 1) && !ignore_nack)
            {
                //bit_dbg("NACK: sending repeated addr\n");

                return -E_EIO;
            }
        }
    }
    else
    {
        /* 7-bit addr */
        addr1 = msg->addr << 1;
        if (flags & IIC_RD)
            addr1 |= 1;
        ret = sim_iic_send_address(bus, addr1, retries);
        if ((ret != 1) && !ignore_nack)
            return -E_EIO;
    }

    return E_EOK;
}

static e_size_t sim_iic_bit_xfer(struct iic_dev *bus,
                              struct iic_msg         msgs[],
                              e_uint32_t               num)
{
    struct iic_msg *msg;
    struct sim_iic_operations *ops = bus->priv;
    e_int32_t i, ret;
    e_uint16_t ignore_nack;

    //bit_dbg("send start condition\n");
    sim_iic_start(ops);
    for (i = 0; i < num; i++)
    {
        msg = &msgs[i];
        ignore_nack = msg->flags & IIC_IGNORE_NACK;
        if (!(msg->flags & IIC_NO_START))
        {
            if (i)
            {
                sim_iic_restart(ops);
            }
            ret = sim_iic_bit_send_address(bus, msg);
            if ((ret != E_EOK) && !ignore_nack)
            {
                //bit_dbg("receive NACK from device addr 0x%02x msg %d\n",
                        //msgs[i].addr, i);
                goto out;
            }
        }
        if (msg->flags & IIC_RD)
        {
            ret = sim_iic_recv_bytes(bus, msg);
            if (ret >= 1)
                //bit_dbg("read %d byte%s\n", ret, ret == 1 ? "" : "s");
            if (ret < msg->len)
            {
                if (ret >= 0)
                    ret = -E_EIO;
                goto out;
            }
        }
        else
        {
            ret = sim_iic_send_bytes(bus, msg);
            if (ret >= 1)
                //bit_dbg("write %d byte%s\n", ret, ret == 1 ? "" : "s");
            if (ret < msg->len)
            {
                if (ret >= 0)
                    ret = -E_ERROR;
                goto out;
            }
        }
    }
    ret = i;

out:
    //bit_dbg("send stop condition\n");
    sim_iic_stop(ops);

    return ret;
}

static const struct iic_operations sim_iic_bus_ops={
    sim_iic_bit_xfer,
    NULL,
    NULL,
};

e_err_t sim_iic_register(int fd, struct sim_iic_dev *bus)
{
    struct iic_dev *dev;
    ASSERT_PARAM(bus!=NULL);
    dev = &bus->parent;
    dev->ops = &sim_iic_bus_ops;
    
    return iic_register(fd, dev);
}

