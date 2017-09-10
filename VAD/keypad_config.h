/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <rafaellcoellho@gmail.com> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return 
* ----------------------------------------------------------------------------
*/

/*
 * @file   keypad_config.h
 * @author Rafael Coelho <rafaellcoellho@gmail.com>
 * @brief  Header with macros for keypad interaction.
 * @details
 */

#ifndef KEYPAD_CONFIG_H_
#define KEYPAD_CONFIG_H_

#define set_bit(reg,bit)		(reg |= (1<<bit))
#define reset_bit(reg,bit)		(reg &= ~(1<<bit))

#define bt_INTERMITTENT_port			PORTC
#define bt_INTERMITTENT_bit				PORTC0
#define bt_INTERMITTENT_ddr				DDRC
#define bt_INTERMITTENT_pin				PINC
#define bt_INTERMITTENT_read			PINC0

#define bt_PLUS_port					PORTC
#define bt_PLUS_bit						PORTC2
#define bt_PLUS_ddr						DDRC
#define bt_PLUS_pin						PINC
#define bt_PLUS_read					PINC2

#define bt_MINUS_port					PORTC
#define bt_MINUS_bit					PORTC1
#define bt_MINUS_ddr					DDRC
#define bt_MINUS_pin					PINC
#define bt_MINUS_read					PINC1

#define bt_UNLOCK_port					PORTB
#define bt_UNLOCK_bit					PORTB0
#define bt_UNLOCK_ddr					DDRB
#define bt_UNLOCK_pin					PINB
#define bt_UNLOCK_read					PINB0

#define bt_SPEED_port					PORTC
#define bt_SPEED_bit					PORTC3
#define bt_SPEED_ddr					DDRC
#define bt_SPEED_pin					PINC
#define bt_SPEED_read					PINC3

#define led_WARNING_port				PORTD
#define led_WARNING_bit					PORTD0	
#define led_WARNING_ddr					DDRD
#define led_WARNING_pin					PIND

#define led_ONE_port					PORTD
#define led_ONE_bit						PORTD4
#define led_ONE_ddr						DDRD
#define led_ONE_pin						PIND

#define led_TWO_port					PORTD
#define led_TWO_bit						PORTD3
#define led_TWO_ddr						DDRD
#define led_TWO_pin						PIND

#define led_THREE_port					PORTD
#define led_THREE_bit					PORTD2
#define led_THREE_ddr					DDRD
#define led_THREE_pin					PIND

#define led_FOUR_port					PORTD
#define led_FOUR_bit					PORTD1
#define led_FOUR_ddr					DDRD
#define led_FOUR_pin					PIND

#define led_STANDBY_port				PORTD
#define led_STANDBY_bit					PORTD6
#define led_STANDBY_ddr					DDRD
#define led_STANDBY_pin					PIND

#define led_SPEED_port					PORTD
#define led_SPEED_bit					PORTD5
#define led_SPEED_ddr					DDRD
#define led_SPEED_pin					PIND



#endif /* KEYPAD_CONFIG_H_ */
