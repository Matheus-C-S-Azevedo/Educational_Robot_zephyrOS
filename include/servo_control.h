#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <stdint.h>

void servo_control_init(void);
void set_servo_angle(uint8_t index, uint8_t angle);

#endif
