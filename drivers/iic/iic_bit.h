#ifndef __IIC_BIT_H__
#define __IIC_BIT_H__


struct iic_bit_operations{
    void (*scl_set)();
    void (*sda_set)();
    int (*sda_get)();
};

struct iic_bit_dev{
    const struct iic_bit_operations *ops;
};





#endif