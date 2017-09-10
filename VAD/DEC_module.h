/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <rafaellcoellho@gmail.com> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return 
* ----------------------------------------------------------------------------
*/

/*
 * @file   DEC_module.h
 * @author Rafael Coelho <rafaellcoellho@gmail.com>
 * @brief  Header with functions for 1-Q-EC Amplifier DEC Module 50/5.
 * @details   

 	Eq for voltage of each speed:
	
	Vset =  ( ( (n-nmin) / (nmax-nmin) ) * 4.9) + 0.1
	
	n = desired speed  [rpm]
	nmin = min speed [rpm]
	nmax = max speed [rpm]

	Value of duty cicle of pwm:

	x = (Vset * 100)/5
	
   	Value of variable:
   
   	y = (x * 65535) / 100
	
	Values pre calculated for each speed:
	
	4000 rpm = 0x321F
	5000 rpm = 0x3EFA
	6000 rpm = 0x4BE0
	7000 rpm = 0x58BA
	
	4500 rpm = 0x3893
	5500 rpm = 0x456D
	6500 rpm = 0x5247
	7500 rpm = 0x5F2E
 */

#ifndef DEC_MODULE_H_
#define DEC_MODULE_H_

#define set_bit(reg,bit) (reg |= (1<<bit))
#define reset_bit(reg,bit) (reg &= ~(1<<bit))

#define DIRECTION			1						// Clockwise    	- 0
													// Anticlockwise 	- 1
													
#define motor_DIRECTION_ddr		DDRD
#define motor_DIRECTION_port	PORTD
#define motor_DIRECTION_bit		PORTD7

#define motor_ENABLE_ddr		DDRC
#define motor_ENABLE_port		PORTC
#define motor_ENABLE_bit		PORTC4

#include <stdint.h>

enum
{
	SPEED_1,
	SPEED_2,
	SPEED_3,
	SPEED_4,
	ENABLE_MOTOR,
	DISABLE_MOTOR	
};

void init_DEC_module(void);
void init_pwm(void);
void set_speed_up(uint8_t speed, uint8_t mod_speed);
void set_speed_down(uint8_t speed, uint8_t mod_speed);
uint8_t read_ready(void);

#endif /* DEC_MODULE_H_ */
