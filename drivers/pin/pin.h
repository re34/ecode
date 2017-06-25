#ifndef __PIN_H__
#define __PIN_H__


typedef enum{
    PIN_LOW,
    PIN_HIGH,
}pin_val_t;

typedef enum{
	PIN_MODE_INPUT,
	PIN_MODE_OUTPUT_PP,
	PIN_MODE_OUTPUT_OD,
	PIN_MODE_ANALOG,
}pin_mode_t;

typedef enum{
	PIN_PUPD_NO,
	PIN_PUPD_UP,
	PIN_PUPD_DOWN,
}pin_pupd_t;

typedef int pin_index_t;

struct pin_device{
    char *name;
    const struct pin_operation *ops;
};

struct pin_operation{
    void (*pin_mode)(struct pin_device *dev, pin_index_t pin,pin_mode_t mode, pin_pupd_t pupd);
    void (*pin_write)(struct pin_device *dev, pin_index_t pin, pin_val_t val);
    int (*pin_read)(struct pin_device *dev, pin_index_t pin);
};


int pin_device_register(const char *name, const struct pin_operation *ops);
void pin_mode(pin_index_t pin, pin_mode_t mode, pin_pupd_t pupd);
void pin_write(pin_index_t pin, pin_val_t val);
int pin_read(pin_index_t pin);

#endif