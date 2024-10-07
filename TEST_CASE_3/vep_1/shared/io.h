#ifndef __CAN_IO_H__
#define __CAN_IO_H__

#include <stdint.h>

extern uint8_t read_button_state(uint8_t button_id);
extern uint8_t read_switch_state(uint8_t switch_id);
extern uint8_t read_green_led_state(uint8_t led_id);
extern void turn_led_on(uint8_t led_id);
extern void turn_led_off(uint8_t led_id);
extern void set_green_led_value(uint8_t led_id, uint8_t value);
extern void set_led_color(uint8_t led_id, uint8_t r, uint8_t g, uint8_t b);

#endif
