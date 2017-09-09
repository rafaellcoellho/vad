#define F_CPU 16000000

#include <avr/io.h>
#include <stdint.h>
#include "keypad_config.h"
#include "keypad.h"

void init_keypad(void)
{	
	reset_bit(bt_INTERMITTENT_ddr, bt_INTERMITTENT_bit);
	reset_bit(bt_PLUS_ddr, bt_PLUS_bit);
	reset_bit(bt_MINUS_ddr, bt_MINUS_bit);
	reset_bit(bt_UNLOCK_ddr, bt_UNLOCK_bit);
	reset_bit(bt_SPEED_ddr, bt_SPEED_bit);

	set_bit(bt_INTERMITTENT_port, bt_INTERMITTENT_bit);
	set_bit(bt_PLUS_port, bt_PLUS_bit);
	set_bit(bt_MINUS_port, bt_MINUS_bit);
	set_bit(bt_UNLOCK_port, bt_UNLOCK_bit);
	set_bit(bt_SPEED_port, bt_SPEED_bit);

	set_bit(led_WARNING_ddr, led_WARNING_bit);
	set_bit(led_ONE_ddr, led_ONE_bit);
	set_bit(led_TWO_ddr, led_TWO_bit);
	set_bit(led_THREE_ddr, led_THREE_bit);
	set_bit(led_FOUR_ddr, led_FOUR_bit);
	set_bit(led_STANDBY_ddr, led_STANDBY_bit);
	set_bit(led_SPEED_ddr, led_SPEED_bit);
	
	set_bit(led_WARNING_port, led_WARNING_bit);
	set_bit(led_ONE_port, led_ONE_bit);
	set_bit(led_TWO_port, led_TWO_bit);
	set_bit(led_THREE_port, led_THREE_bit);
	set_bit(led_FOUR_port, led_FOUR_bit);
	set_bit(led_STANDBY_port, led_STANDBY_bit);
	set_bit(led_SPEED_port, led_SPEED_bit);
}

uint8_t read_button(uint8_t button)
{
	switch(button)
	{
		case INTERMITTENT:
			return !(bt_INTERMITTENT_pin & (1<<bt_INTERMITTENT_read));
			break;
		case PLUS:
			return !(bt_PLUS_pin & (1<<bt_PLUS_read));
			break;
		case MINUS:
			return !(bt_MINUS_pin & (1<<bt_MINUS_read));
			break;
		case UNLOCK:
			return !(bt_UNLOCK_pin & (1<<bt_UNLOCK_read));
			break;
		case SPEED:
			return !(bt_SPEED_pin & (1<<bt_SPEED_read));
			break;
	}	
	return 0;
}

void led_on(uint8_t led)
{
		switch(led)
		{
			case WARNING:
				reset_bit(led_WARNING_port, led_WARNING_bit);
				break;
			case ONE:
				reset_bit(led_ONE_port, led_ONE_bit);
				break;
			case TWO:
				reset_bit(led_TWO_port, led_TWO_bit);
				break;
			case THREE:
				reset_bit(led_THREE_port, led_THREE_bit);
				break;
			case FOUR:
				reset_bit(led_FOUR_port, led_FOUR_bit);
				break;
			case STANDBY:
				reset_bit(led_STANDBY_port, led_STANDBY_bit);
				break;
			case SPEED_LED:
				reset_bit(led_SPEED_port, led_SPEED_bit);
				break;
		}
}

void led_off(uint8_t led)
{
	switch(led)
	{
		case WARNING:
			set_bit(led_WARNING_port, led_WARNING_bit);
			break;
		case ONE:
			set_bit(led_ONE_port, led_ONE_bit);
			break;
		case TWO:
			set_bit(led_TWO_port, led_TWO_bit);
			break;
		case THREE:
			set_bit(led_THREE_port, led_THREE_bit);
			break;
		case FOUR:
			set_bit(led_FOUR_port, led_FOUR_bit);
			break;
		case STANDBY:
			set_bit(led_STANDBY_port, led_STANDBY_bit);
			break;
		case SPEED_LED:
			set_bit(led_SPEED_port, led_SPEED_bit);
			break;
	}
}

void led_toggle(uint8_t led)
{
	switch(led)
	{
		case STANDBY:
			set_bit(led_STANDBY_pin, led_STANDBY_bit);
			break;
	}
}
