#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#define LED_CONFIG      (DDRD |= (1<<6))
#define TOGGLE_U_LED	(PIND |= (1<<PD6))
#define CLOCKWISE		1
#define ANTI_CLOCKWISE	-1
#define COIL_A1			(PF4)
#define COIL_A2			(PF5)
#define COIL_B1			(PF7)
#define COIL_B2			(PF6)
#define COIL_PORT		(PORTF)

int8_t motorState;

void motor(int8_t direction) {
	while (1) {
		motorState += direction;
			TOGGLE_U_LED;
		if (motorState > 7) motorState = 0;
		else if (motorState < 0) motorState = 7;
		switch (motorState) {
			case 0:
				COIL_PORT &= ~((1<<COIL_A2) | (1<<COIL_B1) | (1<<COIL_B2));
				COIL_PORT |= (1<<COIL_A1);
				break;
			case 1:
				COIL_PORT &= ~((1<<COIL_A2) | (1<<COIL_B2));
				COIL_PORT |= (1<<COIL_A1) | (1<<COIL_B1);
				break;
			case 2:
				COIL_PORT &= ~((1<<COIL_A1) | (1<<COIL_A2) | (1<<COIL_B2));
				COIL_PORT |= (1<<COIL_B1);
				break;
			case 3:
				COIL_PORT &= ~((1<<COIL_A1) | (1<<COIL_B2));
				COIL_PORT |= (1<<COIL_A2) | (1<<COIL_B1);
				break;
			case 4:
				COIL_PORT &= ~((1<<COIL_A1) | (1<<COIL_B1) | (1<<COIL_B2));
				COIL_PORT |= (1<<COIL_A2);
				break;
			case 5:
				COIL_PORT &= ~((1<<COIL_A1) | (1<<COIL_B1));
				COIL_PORT |= (1<<COIL_A2) | (1<<COIL_B2);
				break;
			case 6:
				COIL_PORT &= ~((1<<COIL_A1) | (1<<COIL_A2) | (1<<COIL_B1));
				COIL_PORT |= (1<<COIL_B2);
				break;
			case 7:
				COIL_PORT &= ~((1<<COIL_A2) | (1<<COIL_B1));
				COIL_PORT |= (1<<COIL_A1) | (1<<COIL_B2);
				break;
		}
		_delay_ms(25);
	}
}

int main(void) {
	LED_CONFIG;
	DDRF |= (1<<PF4) | (1<<PF5) | (1<<PF6) | (1<<PF7);
	motorState = 0;
	motor(CLOCKWISE);
	while (1) {
		uint64_t i = 0;
		TOGGLE_U_LED;
		while (++i != 0);
	}
}
