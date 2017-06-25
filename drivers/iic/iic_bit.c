#include "iic_bit.h"
#include "iic.h"
#include "time.h"
#include "assert.h"


#define SET_SDA(ops, val)   ops->set_sda(ops->data, val)
#define SET_SCL(ops, val)   ops->set_scl(ops->data, val)
#define GET_SDA(ops)        ops->get_sda(ops->data)
#define GET_SCL(ops)        ops->get_scl(ops->data)

e_inline void iic_bit_delay(const struct iic_bit_operations *ops)
{
    ops->udelay(ops->delay_us);
}

e_inline void iic_bit_delay2(const struct iic_bit_operations *ops)
{
    ops->udelay((ops->delay_us+1)>>1);
}

#define SDA_L(ops)      SET_SDA(ops, 0)
#define SDA_H(ops)      SET_SDA(ops, 1)
#define SCL_L(ops)      SET_SCL(ops, 0)


static e_err_t SCL_H(const struct iic_bit_operations *ops)
{
    e_tick_t start;
    
    SET_SCL(ops, 1);
    
    if(!ops->get_scl)
        goto done;
    
    start = (!GET_SCL(ops));
    while(!GET_SCL(ops))
    {
        if(system_get_time()-start>ops->timeout)
            return -E_ETIMEOUT;
        delay_us((ops->timeout+1)>>1);
    }
    
done:
    iic_bit_delay(ops);
    
    return E_EOK;
}


static e_err_t iic_bit_start(struct iic_dev *bus)
{
    struct iic_bit_dev *iic_bit_bus = container_of(bus, struct iic_bit_dev, parent);
    const struct iic_bit_operations *ops = iic_bit_bus->ops;
    
    SDA_H(ops);
    SCL_H(ops);
    iic_bit_delay(ops);
    SDA_L(ops);
    iic_bit_delay(ops);
    SCL_L(ops);
    
    return E_EOK;
}

static e_err_t iic_bit_stop(struct iic_dev *bus)
{
    struct iic_bit_dev *iic_bit_bus = container_of(bus, struct iic_bit_dev, parent);
    const struct iic_bit_operations *ops = iic_bit_bus->ops;
    
    SDA_L(ops);
    iic_bit_delay(ops);
    SCL_H(ops);
    iic_bit_delay(ops);
    SDA_H(ops);
    iic_bit_delay2(ops);
    
    return E_EOK;
}

e_inline e_bool_t iic_bit_waitack(const struct iic_bit_operations *ops)
{
    e_bool_t ack;

    SDA_H(ops);
    iic_bit_delay(ops);

    if (SCL_H(ops) < 0)
    {
        //bit_dbg("wait ack timeout\n");

        return -E_ETIMEOUT;
    }

    ack = !GET_SDA(ops);    /* ACK : SDA pin is pulled low */

    SCL_L(ops);

    return ack;
}


static e_err_t iic_bit_send_ack_or_nack(const struct iic_bit_operations *ops, int ack)
{
    if (ack)
        SET_SDA(ops, 0);
    iic_bit_delay(ops);
    if (SCL_H(ops) < 0)
    {
        //bit_dbg("ACK or NACK timeout\n");

        return -E_ETIMEOUT;
    }
    SCL_L(ops);

    return E_EOK;
}

static int iic_bit_writeb(struct iic_dev *bus, e_uint8_t value)
{
    e_int32_t i;
    e_uint8_t bit;
    
    struct iic_bit_dev *iic_bit_bus = container_of(bus, struct iic_bit_dev, parent);
    const struct iic_bit_operations *ops = iic_bit_bus->ops;
    
    for(i=7;i>=0;i--)
    {
        SCL_L(ops);
        bit = (value>>i)&1;
        SET_SDA(ops, bit);
        iic_bit_delay(ops);
        if (SCL_H(ops) < 0)
        {
            //bit_dbg("iic_bit_writeb: 0x%02x, "
            //        "wait scl pin high timeout at bit %d\n",
             //       data, i);

            return -E_ETIMEOUT;
        }
    }
    SCL_L(ops);
    iic_bit_delay(ops);

    return iic_bit_waitack(ops);
}

static e_err_t iic_bit_readb(struct iic_dev *bus, e_uint8_t *pvalue, int ack)
{
    e_uint8_t i;
    e_uint8_t data = 0;
    struct iic_bit_dev *iic_bit_bus = container_of(bus, struct iic_bit_dev, parent);
    const struct iic_bit_operations *ops = iic_bit_bus->ops;
    
    SDA_H(ops);
    iic_bit_delay(ops);
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
        iic_bit_delay2(ops);
    }
    
    iic_bit_send_ack_or_nack(ops,ack);

    *pvalue = data;
    return E_EOK;
}


const struct iic_operations iic_ops={
    .start = iic_bit_start,
    .stop = iic_bit_stop,
    .writeb = iic_bit_writeb,
    .readb = iic_bit_readb,
};

e_err_t iic_bit_register(int bus,struct iic_bit_dev *iic_bit_dev)
{
    ASSERT_PARAM(iic_bit_dev!=NULL);
    iic_bit_dev->parent.ops = &iic_ops;
    
    return iic_register(bus, &(iic_bit_dev->parent));
}



