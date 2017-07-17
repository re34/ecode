#ifndef __MPU6050_H__
#define __MPU6050_H__
#include "types.h"


struct mpu6050_dev{
    e_uint8_t iic_bus;
    e_uint8_t address;
};


#endif