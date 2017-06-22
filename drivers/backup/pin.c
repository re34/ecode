#include "pin.h"


static struct pin_device _hw_pin;

int pin_device_register(const char *name,const struct pin_operation *ops)
{
    _hw_pin.name = (char *)name;
    _hw_pin.ops = ops;

    return 0;
}
void pin_mode(pin_index_t pin, pin_mode_t mode, pin_pupd_t pupd)
{
    _hw_pin.ops->pin_mode(&_hw_pin, pin, mode, pupd);    
}

void pin_write(pin_index_t pin, pin_val_t val)
{
    _hw_pin.ops->pin_write(&_hw_pin, pin, val);
}

int pin_read(pin_index_t pin)
{
    return _hw_pin.ops->pin_read(&_hw_pin, pin);
}