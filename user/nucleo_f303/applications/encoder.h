#ifndef __ENCODER_H__
#define __ENCODER_H__

struct encoder{
    int left_count;
    int right_count;
};


void encoder_init(void);
void encoder_get(struct encoder *encoder);
#endif