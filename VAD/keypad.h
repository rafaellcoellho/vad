/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <rafaellcoellho@gmail.com> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

/*
 * @file   lcd_16_4.h
 * @author Rafael Coelho <rafaellcoellho@gmail.com>
 * @brief  Header with functions for keypad interaction.
 * @details
 *                 
 */


#ifndef KEYPAD_H_
#define KEYPAD_H_

#define set_bit(reg,bit)		(reg |= (1<<bit))
#define reset_bit(reg,bit)		(reg &= ~(1<<bit))

#include <stdint.h>

enum
{
	INTERMITTENT,
	PLUS,
	MINUS,
	UNLOCK,
	SPEED,
	WARNING,
	ONE,
	TWO,
	THREE,
	FOUR,
	STANDBY,
	SPEED_LED
};


void init_keypad(void);
uint8_t read_button(uint8_t button);
void led_on(uint8_t led);
void led_off(uint8_t led);
void led_toggle(uint8_t led);

#endif /* KEYPAD_H_ */