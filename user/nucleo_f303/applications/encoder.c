#include "encoder.h"

static struct encoder fencoder;

e_err_t encoder_init(struct encoder encoder)
{
    fencoder = encoder;
    
    return E_EOK;
}


