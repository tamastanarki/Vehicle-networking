#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xil_printf.h>
#include <platform.h>
#include <timers.h>
#include "vep_private_memory.h"



volatile uint32_t *gpio0 = (uint32_t*)0x81200000;
volatile uint32_t *gpio1 = (uint32_t*)0x81400000;


int main ( void )
{
  //xil_printf("IO Controller\n");
  volatile uint64_t t_start = read_global_timer();

  // GPIO0
  // Bank 1: 4 LEDs -> Set all the pins as output
  gpio0[1] = 0x0;
  // Set values
  gpio0[0] = 0xFF;
  // Bank 2: 2 LEDs , 3 Colors -> Set all the pins as output
  gpio0[3] = 0x0;
  // Set values
  gpio0[2] = 0xFF;

  // GPIO1
  // Bank 1: 4 buttons -> Set them as inputs
  gpio1[1] = 0x0F;

  // Bank 2: 2 switches -> Set them as inputs
  gpio1[3] = 0x03;


  uint8_t buttons;
  uint8_t switches;

  while(1)
  {
    // read buttons and switches status
    buttons = (uint8_t)gpio1[0] & 0xF;
    switches = (uint8_t)gpio1[2] & 0x3;
    vep_private_mem01->buttons = buttons;
    vep_private_mem02->buttons = buttons;
    vep_private_mem01->switches = switches;
    vep_private_mem02->switches = switches;
    gpio0[0] = vep_private_mem02->leds | vep_private_mem01->leds;
    gpio0[2] = vep_private_mem02->color_leds | vep_private_mem01->color_leds;
  }
  return 0;
}
