#ifndef __7SEG

#define __7SEG

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>


/*

1. Get GPIO pins
2. Init gpio pins
3. Map those pins for the display char


*/


int init_7_segment();

void display_hex_char(uint8_t hex_char);





#endif