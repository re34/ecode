#include "iic_bit.h"


#define SET_SDA(ops, val)   ops->set_sda(ops->data, val)
#define SET_SCL(ops, val)   ops->set_scl(ops->data, val)
#define GET_SDA(ops)        ops->get_sda(ops->data)
#define GET_SCL(ops)        ops->get_scl(ops->data)

e_inline void iic_bit_delay(struct iic_bit_operations *ops)
{
    ops->udelay(ops->delay_us);
}

e_inline void iic_bit_delay2(struct iic_bit_operations *ops)
{
    ops->udelay((ops->delay_us+1)>>1);
}

#define SDA_L(ops)      SET_SDA(ops, 0)
#define SDA_H(ops)      SET_SDA(ops, 1)
#define SCL_L(ops)      SET_SCL(ops, 0)


static e_err_t SCL_H(struct iic_bit_operations *ops)
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
    iic_bit_delay(ops);
    
    return E_EOK;
}

static void iic_bit_start(struct iic_bit_operations *ops)
{
    SDA_L(ops);
    iic_bit_delay(ops);
    SCL_L(ops);
    
}

static void iic_bit_restart(struct iic_bit_operations *ops)
{
    SDA_H(ops);
    SCL_H(ops);
    iic_bit_delay(ops);
    SDA_L(ops);
    iic_bit_delay(ops);
    SCL_L(ops);
}

static void iic_bit_stop(struct iic_bit_operations *ops)
{
    SDA_L(ops);
    iic_bit_delay(ops);
    SCL_H(ops);
    iic_bit_delay(ops);
    SDA_H(ops);
    iic_bit_delay2(ops);
}

e_inline e_bool_t iic_bit_waitack(struct iic_bit_operations *ops)
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

static e_int32_t iic_bit_writeb(struct iic_dev *bus, e_uint8_t data)
{
    e_int32_t i;
    e_uint8_t bit;
    
    struct iic_bit_operations *ops = bus->priv;
    
    for(i=7;i>=0;i--)
    {
        SCL_L(ops);
        bit = (data>>i)&1;
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

static e_int32_t iic_bit_readb(struct iic_dev *bus)
{
    e_uint8_t i;
    e_uint8_t data = 0;
    struct iic_bit_operations *ops = bus->priv;
    
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

    return data;
}