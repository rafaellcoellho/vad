#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "DEC_module.h"

void init_DEC_module(void)
{
	set_bit(motor_DIRECTION_ddr, motor_DIRECTION_bit);
	set_bit(motor_ENABLE_ddr, motor_ENABLE_bit);
	
	if(DIRECTION)
	{	
		set_bit(motor_DIRECTION_port, motor_DIRECTION_bit);	
	}
	else
	{
		reset_bit(motor_DIRECTION_port, motor_DIRECTION_bit);
	}
	
	reset_bit(motor_ENABLE_port, motor_ENABLE_bit);
	
	ADMUX |= (1<<REFS0);
	
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);

	init_pwm();
}


void init_pwm(void)
{
   DDRB |= (1 << DDB2);

   ICR1 = 0xFFFF;

   OCR1B = 0x0000;

   TCCR1A |= (1 << COM1B1);

   TCCR1A |= (1 << WGM11);
   TCCR1B |= (1 << WGM12)|(1 << WGM13);

   TCCR1B |= (1 << CS10);
}

void set_speed_up(uint8_t speed, uint8_t mod_speed)
{
	/*

 	*/
	   
	uint16_t i;
	
	switch(speed)
	{
		case SPEED_1:
			if (mod_speed)
			{
				for (i = 0; i < 1283; i++)
				{
					_delay_ms(1);
					OCR1B += 10;
				}
				OCR1B++;
			}
			else
			{
				for (i = 0; i < 1448; i++)
				{
					_delay_ms(1);
					OCR1B += 10;
				}
				OCR1B += 3;
			}
			break;
		case SPEED_2:
			if (mod_speed)
			{
				for (i = 0; i < 1097; i++)
				{
					_delay_ms(1);
					OCR1B += 3;
				}
			}
			else
			{
				for (i = 0; i < 1645; i++)
				{
					_delay_ms(1);
					OCR1B += 2;
				}
			}
			break;
		case SPEED_3:
			if (mod_speed)
			{
				for (i = 0; i < 1651; i++)
				{
					_delay_ms(1);
					OCR1B += 2;
				}
			}
			else
			{
				for (i = 0; i < 1645; i++)
				{
					_delay_ms(1);
					OCR1B += 2;
				}
			}
			break;
		case SPEED_4:
			if (mod_speed)
			{
				for (i = 0; i < 1645; i++)
				{
					_delay_ms(1);
					OCR1B += 2;
				}
			}
			else
			{
				for (i = 0; i < 1101; i++)
				{
					_delay_ms(1);
					OCR1B += 3;
				}
			}
			break;
	}
}

void set_speed_down(uint8_t speed, uint8_t mod_speed)
{
	uint16_t i;
	
	switch(speed)
	{
		case SPEED_1:
			if (mod_speed)
			{
				for (i = 0; i < 1097; i++)
				{
					_delay_ms(1);
					OCR1B -= 3;
				}
			}
			else
			{
				for (i = 0; i < 1645; i++)
				{
					_delay_ms(1);
					OCR1B -= 2;
				}
			}
			break;
		case SPEED_2:
			if (mod_speed)
			{
				for (i = 0; i < 1651; i++)
				{
					_delay_ms(1);
					OCR1B -= 2;
				}
			}
			else
			{
				for (i = 0; i < 1645; i++)
				{
					_delay_ms(1);
					OCR1B -= 2;
				}
			}
			break;
		case SPEED_3:
			if (mod_speed)
			{
				for (i = 0; i < 1645; i++)
				{
					_delay_ms(1);
					OCR1B -= 2;
				}
			}
			else
			{
				for (i = 0; i < 1101; i++)
				{
					_delay_ms(1);
					OCR1B -= 3;
				}
			}
			break;
	}
}

uint8_t read_ready(void)
{
	uint16_t aux;
	
	ADMUX = ADMUX & 0xF0;
	
	ADMUX |= ( 0x06 & 0x0F);
	
	ADCSRA |= (1<<ADSC);
	
	while (ADCSRA & (1<<ADSC) );
	
	aux = ADC;
	
	if(aux > 300)
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

