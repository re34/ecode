#ifndef __SMART_CAR_H__
#define __SMART_CAR_H__



int smart_car_init();
int smart_car_get_left_speed();
int smart_car_get_right_speed();
void smart_car_set_speed(int left_speed, int right_speed);
void smart_car_speed_up();
void smart_car_speed_down();
void smart_car_turn_left();
void smart_car_turn_right();


#endif