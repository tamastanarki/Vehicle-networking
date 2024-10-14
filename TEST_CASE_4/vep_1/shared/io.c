#ifndef __IO_C__
#define __IO_C__

#include "io.h"

// hack to link with right shared memory; cannot do this at run time
#if(TILE_ID == 1)
#define MEM vep_private_mem01
#endif
#if(TILE_ID == 2)
#define MEM vep_private_mem02
#endif

uint8_t read_button_state(uint8_t button_id)
{
  if (button_id > 3) {	
    xil_printf("read_button_state: Bad button id! Use 0, 1, 2, or 3.\n");
    return 0;
  }
  else {
    return (MEM->buttons >> button_id) & 1;
  }
}

uint8_t read_switch_state(uint8_t switch_id)
{
  if (switch_id > 1) {	
    xil_printf("read_switch_state: Bad switch id! Use 0 or 1.\n");
    return 0;
  }
  else {
    return (MEM->switches >> switch_id) & 1;
  }
}

uint8_t read_green_led_state(uint8_t led_id)
{
  if (led_id > 3) {	
    xil_printf("read_green_led: Bad LED id! Use 0, 1, 2, or 3.\n");
    return 0;
  }
  else {
    return (MEM->leds >> led_id) & 1;
  }
}


void turn_led_on(uint8_t led_id)
{
  if (led_id > 3) {	
    xil_printf("turn_led_on: Bad LED id! Use 0, 1, 2, or 3.\n");
  }
  else {
    MEM->leds = ((uint8_t)1 << led_id) | MEM->leds;
  }
}


void turn_led_off(uint8_t led_id)
{
  if (led_id > 3) {	
    xil_printf("turn_led_off: Bad LED id! Use 0, 1, 2, or 3.\n");
  }
  else {
    MEM->leds = ~((uint8_t)1 << led_id) & MEM->leds;
  }
}

void set_green_led_value(uint8_t led_id, uint8_t value)
{
  if (value == 0) turn_led_off(led_id);
  else turn_led_on(led_id);
}


void set_led_color(uint8_t led_id, uint8_t r, uint8_t g, uint8_t b)
{
  if (led_id > 1) {	
    xil_printf("set_led_color: Bad LED id! Use 0 or 1.\n");
  }
  else {
    uint8_t color = ((r & 1) << 2) | ((g & 1) << 1) | ((b & 1) << 0);
    MEM->color_leds = (color << 3*led_id) | (MEM->color_leds & ~((uint8_t)0x7 << 3*led_id));
  }
}

#endif
