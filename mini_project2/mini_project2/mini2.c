/*
 * mini2.c
 *
 *  Created on: 13 Sep 2022
 *      Author: world
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
short seconds = 0 ;
short minutes = 0;
short hours= 0 ;


ISR(TIMER1_COMPA_vect)
{
	seconds++;
	if ( seconds == 60)
	{
		seconds=0;
		minutes++;
	}
	if (minutes == 60)
	{
		minutes =0;
		hours ++;
	}
	if(hours == 100)
	{
		seconds=0;
		minutes=0;
		hours=0;
	}
}

void Timer1_CTC(void)
{
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS11);
	TCNT1 = 0;
	SREG |=(1<<7);
	TIMSK|=(1<<OCIE1A);
	OCR1A = 15625;
}
ISR(INT0_vect)
{
	 seconds = 0 ;
	 minutes = 0;
	 hours= 0 ;
}
ISR(INT1_vect)
{
	TCCR1B &= ~(1<<CS10)&~(1<<CS11)&~(1<<CS12);


}
ISR(INT2_vect)
{
	TCCR1B |= (1<<CS10) | (1<<CS11)|(1<<WGM12);

}
void INT0_Init(void)
{
	DDRD  &= (~(1<<PD2));
	PORTD |= (1<<PD2);
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	GICR  |= (1<<INT0);
	SREG  |= (1<<7);
}

void INT1_Init(void)
{
	DDRD  &= (~(1<<PD3));
	PORTD |= (1<<PD3);
	MCUCR |= (1<<ISC11)|(1<<ISC10);
	GICR  |= (1<<INT1);
	SREG  |= (1<<7);
}
void INT2_Init(void)
{
	DDRB   &= (~(1<<PB2));
	PORTB |= (1<<PB2);
	MCUCSR &= (~(1<<ISC2));
	GICR   |= (1<<INT2);
	SREG   |= (1<<7);
}

int main()

{
	DDRC |= 0x0F;
	PORTC &= 0xF0;
	DDRA  |= 0xFF;
	PORTA &=0xFF;
	Timer1_CTC();
	INT0_Init();
	INT1_Init();
	INT2_Init();




	while(1)
	{
		PORTA = (1<<5);
				PORTC = (PORTC & 0xF0) | ( 0x0F & seconds % 10);
				_delay_ms(5);
				PORTA = (1<<4);
				PORTC =(PORTC & 0xF0) | ( 0x0F & seconds / 10);
				_delay_ms(5);
				PORTA = (1<<3);
				PORTC =(PORTC & 0xF0) | ( 0x0F & minutes % 10);
				_delay_ms(5);
				PORTA = (1<<2);
				PORTC =(PORTC & 0xF0) | ( 0x0F & minutes / 10);
				_delay_ms(5);
				PORTA = (1<<1);
				PORTC = (PORTC & 0xF0) | ( 0x0F &hours % 10);
				_delay_ms(5);
				PORTA = (1<<0);
				PORTC = (PORTC & 0xF0) | ( 0x0F &hours / 10);
				_delay_ms(5);
			}


	}



