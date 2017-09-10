/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <rafaellcoellho@gmail.com> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return 
* ----------------------------------------------------------------------------
*/

/*
 * @file   main.c
 * @author Rafael Coelho <rafaellcoellho@gmail.com>
 * @brief  This file contains the main prototype logic. 
 * @details 

	This project was used in a VAD (Ventricular Assist Device) prototype. His main functionality is assisting cardiac 
	circulation, which is used either to partially or to completely replace the function of a failing heart.

	The architecture of this firmware is interrupt based. In the main loop the buttons with less priority is checked 
	continously, and the interrupt of the timer control the pump's speed.

	User interface with LED and buttons are made with the keypad.h, using the facade pattern. 

	The control of brushless DC motors are made with the DEC_module.h, using the facade pattern also.
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                           Internal Constants                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define F_CPU						16000000		//External 16 Mhz Clock

#define set_bit(reg,bit)			(reg |= (1<<bit))			
#define reset_bit(reg,bit)			(reg &= ~(1<<bit))

#define PRESCALER_LED				1000			// Prescaler for standby led
													// 1mS * 1000 = 1 Second
#define PRESCALER_CURRENT_SPEED		56000			// Prescaler for the highest speed of intermittence
													// 1mS * 56000 = 56 Seconds
#define PRESCALER_PREVIUS_SPEED		8000			// Prescaler for the lowest speed of intermittence
													// 1mS * 8000 = 8 Seconds
													
#define T_FILTER					250				// Number of times that the button was checked

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              Libraries                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>								
#include "keypad.h"								
#include "keypad_config.h"
#include "DEC_module.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          Global variables                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

unsigned char current_speed;								// Current motor speed
															// 0 - off
															// 1 - 4000 or 4500 rpm 
															// 2 - 5000 or 5500 rpm
															// 3 - 6000 or 6500 rpm
															// 4 - 7000 or 7500 rpm
												
unsigned char status_speed;									// Mode 4000/5000/6000/7000 - 1;
															// Mode 4500/5500/6500/7500 - 0;
										
unsigned int addr = 0;										// status_speed adress in eeprom
															  
unsigned char filter_intermittent = T_FILTER;				// Debounce variable for button that turns on intermittent mode

unsigned char filter_plus = T_FILTER;						// Debounce variable for button that increases speed

unsigned char filter_minus= T_FILTER;						// Debounce variable for button that decreases speed

unsigned char filter_speed = T_FILTER;						// Debounce variable for button that change the speed profile

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               Software flags                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct
{
    unsigned BIT0:1;
    unsigned BIT1:1;
    unsigned BIT2:1;
    unsigned BIT3:1;
    unsigned BIT4:1;
    unsigned BIT5:1;
    unsigned BIT6:1;
    unsigned BIT7:1;
 }FLAGSbits, FLAGSbutton;
 
 #define status_standby					FLAGSbits.BIT0		// Standby					- 1
															// Active					- 0
														
 #define status_unlock					FLAGSbits.BIT1		// Blocked					- 0
															// Unblocked				- 1
														
#define status_intermittent				FLAGSbits.BIT2		// Intermittent				- 0
															// Continuos				- 1
														
#define status_previus_speed			FLAGSbits.BIT3		// Speed 3				    - 0
															// Speed 4					- 1
		
#define status_intermittent_button		FLAGSbutton.BIT0	
												
#define status_plus_button				FLAGSbutton.BIT1	

#define status_minus_button				FLAGSbutton.BIT2	

#define status_speed_button				FLAGSbutton.BIT3	
														
														
 
 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Prototype                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
void init_timer(void);													
void up_speed(void);															
void down_speed(void);									
void up_speed_intermittent(void);						
void down_speed_intermittent(void);						

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                           	Main                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int main(void)
{
	
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                        System Inicialization                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	status_standby = 1;											// In standby
	status_speed = eeprom_read_byte((unsigned char *)addr);		// Load speed in eeprom
																// Default: 4000/5000/6000/7000 - 1
	current_speed = 0;											
	
	init_timer();												
	init_keypad();
	init_DEC_module();		
	
	sei();														// Enable global interrupt 
	
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                            Main Routine                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
    while (1) 
    {
		// Read all buttons in loop
		if(read_button(UNLOCK))												
		{
			if(read_button(INTERMITTENT))									
			{
				if(!status_intermittent_button)
				{
					if(filter_intermittent)
					{
						filter_intermittent--;
					}
					else
					{
						if(status_intermittent == 1)
						{
							status_intermittent = 0;
										
							led_off(WARNING);
										
							if (current_speed == 2)
							{
								up_speed_intermittent();
							}
										
						}
						else if(status_intermittent == 0 && current_speed > 2)
						{
							status_intermittent = 1;
										
							led_on(WARNING);
						}
						
						status_intermittent_button = 1;
					}
				}
			}
			else
			{
				
				status_intermittent_button = 0;
				filter_intermittent = T_FILTER;
			}
			
			if(read_button(PLUS))
			{
				if(!status_plus_button)
				{
					if(filter_plus)
					{
						filter_plus--;
					}
					else
					{
						
						set_bit(motor_ENABLE_port, motor_ENABLE_bit);
						
						if(!status_intermittent)
						{
							up_speed();
						}
						status_plus_button = 1;
					}
				}
			}
			else
			{
				
				status_plus_button = 0;
				filter_plus = T_FILTER;
			}
			
			if(read_button(MINUS))
			{
				if(!status_minus_button)
				{
					if(filter_minus)
					{
						filter_minus--;
					}
					else
					{
						if(current_speed > 1 && !status_intermittent)
						{
							down_speed();
						}
					
						status_minus_button = 1;
					}
				}
			}
			else
			{
			
				status_minus_button = 0;
				filter_minus = T_FILTER;
			}
			
		}
		
		if(read_button(SPEED))
		{
			if(!status_speed_button)
			{
				if(filter_speed)
				{
					filter_speed--;
				}
				else
				{
					if(status_speed)
					{
						status_speed = 0;
						eeprom_write_byte((unsigned char *)addr, status_speed);		
					}
					else
					{
						status_speed = 1;
						eeprom_write_byte((unsigned char *)addr, status_speed);		
					}
					
					status_speed_button = 1;
				}
			}
		}
		else
		{
			
			status_speed_button = 0;
			filter_speed = T_FILTER;
		}

		// Read the current state of motor
		if(read_ready() == 0)		
		{
			// Reset all config 

			status_standby = 1;
			status_intermittent = 0;
				
			current_speed = 0;
				
			led_off(ONE);
			led_off(TWO);
			led_off(THREE);
			led_off(FOUR);
			led_off(WARNING);
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *								Functions                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void init_timer(void)
{
	set_bit(TCCR0A, WGM01);								// Select mode CTC
	
	set_bit(TCCR0B, CS01);								// Select the prescaler - Clk/64
	set_bit(TCCR0B, CS00);
	
	OCR0A = 124;										
	
	/*
		T[uS] = (1/fclk[MHz]) * 2 * Prescaler * (1 + OCR0A)
		
		T = (1/16) * 2 * 64 * (1 + 124)
		T = 1000uS
	*/
	
	set_bit(TIMSK0, OCIE0A);							// Set the flag of interrupt of timer					
}

void up_speed(void)
{
	
	if(current_speed == 0)
	{
		status_standby = 0;
		current_speed = 1;
		
		set_speed_up(SPEED_1, status_speed);
		
		led_on(ONE);
		
	}
	else if(current_speed == 1)
	{
		current_speed = 2;
		
		set_speed_up(SPEED_2, status_speed);
		
		led_off(ONE);
		led_on(TWO);

	}
	else if(current_speed == 2)
	{
		current_speed = 3;
		status_previus_speed = 0;
		
		set_speed_up(SPEED_3, status_speed);
		
		led_off(TWO);
		led_on(THREE);
		
	}
	else if(current_speed == 3)
	{
		current_speed = 4;
		status_previus_speed = 1;
		
		set_speed_up(SPEED_4, status_speed);
		
		led_off(THREE);
		led_on(FOUR);
		
	}
}

void down_speed(void)
{
	if(current_speed == 4)
	{
		current_speed = 3;
		status_previus_speed = 0;
		
		set_speed_down(SPEED_3, status_speed);
		
		led_off(FOUR);
		led_on(THREE);
		
	}
	else if(current_speed == 3)
	{
		current_speed = 2;
		
		set_speed_down(SPEED_2, status_speed);
		
		led_off(THREE);
		led_on(TWO);
		
	}
	else if(current_speed == 2)
	{
		current_speed = 1;
		
		set_speed_down(SPEED_1, status_speed);
		
		led_off(TWO);
		led_on(ONE);
		
	}
}

void up_speed_intermittent(void)
{
	unsigned int i;
	
	if(status_previus_speed == 0)
	{
		current_speed = 3;
		
		set_speed_up(SPEED_3, status_speed);
		
		led_off(TWO);
		led_on(THREE);

	}
	else if(status_previus_speed == 1)
	{
		current_speed = 4;
	
		if (status_speed )
		{
			for (i = 0 ; i < 1648; i++)
			{
				_delay_ms(1);
				OCR1B += 4;
			}
		}
		else
		{
			for (i = 0 ; i < 1648; i++)
			{
				_delay_ms(1);
				OCR1B += 4;
			}
			OCR1B++;
		}
		
		led_off(TWO);
		led_on(FOUR);
	}

}

void down_speed_intermittent(void)
{
	unsigned int i;
	
	if (current_speed == 4)
	{
		current_speed = 2;

		if (status_speed)
		{
			for (i = 0 ; i < 1648; i++)
			{
				_delay_ms(1);
				OCR1B -= 4;
			}
		}
		else
		{
			for (i = 0 ; i < 1648; i++)
			{
				_delay_ms(1);
				OCR1B -= 4;
			}
			OCR1B--;
		}
		
		led_off(FOUR);
		led_on(TWO);
	}
	else if (current_speed == 3)
	{
		current_speed = 2;
		
		set_speed_down(SPEED_2, status_speed);
		
		led_off(THREE);
		led_on(TWO);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *								Interrupts                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ISR(TIMER0_COMPA_vect)
{
	static int counter_led;													
	static long int counter_inter;								
	static long int PRESCALER_SPEED = PRESCALER_CURRENT_SPEED;	
	char status_motor = read_ready();							
	
	if(counter_led == PRESCALER_LED)							
	{
		if(status_standby)										
		{
			if(status_motor)									
			{
				led_on(STANDBY);								
			}
			else
			{	
				led_toggle(STANDBY);							
			}
		}
		else
		{
			led_off(STANDBY);									
		}
		
		if(status_speed)
		{
			led_off(SPEED_LED);
		}
		else
		{
			led_on(SPEED_LED);
		}
		counter_led = 0;										
	}
	else
	{
		counter_led++;																		
	}

	if(status_intermittent)																	
	{
		if(counter_inter == PRESCALER_SPEED)					
		{
			if(PRESCALER_SPEED == PRESCALER_CURRENT_SPEED)
			{
				down_speed_intermittent();
				
				PRESCALER_SPEED = PRESCALER_PREVIUS_SPEED;
			}
			else if(PRESCALER_SPEED == PRESCALER_PREVIUS_SPEED)
			{
				up_speed_intermittent();
				
				PRESCALER_SPEED = PRESCALER_CURRENT_SPEED;
			}
			
			
			counter_inter = 0;
		}
		else
		{
			counter_inter++;	
		}
	}
	else
	{
		counter_inter = 0;									
		PRESCALER_SPEED = PRESCALER_CURRENT_SPEED;
	}
	
}
