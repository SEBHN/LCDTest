/*************************************************************************
Title:    Testing output to a HD44780 based LCD display.
Author:   Peter Fleury  <pfleury@gmx.ch>  http://tinyurl.com/peterfleury
File:     $Id: test_lcd.c,v 1.8 2015/01/31 18:04:08 peter Exp $
Software: AVR-GCC 4.x
Hardware: HD44780 compatible LCD text display
          AVR with external SRAM interface if memory-mapped LCD interface is used
          any AVR with 7 free I/O pins if 4-bit IO port mode is used
**************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"
#include <avr/interrupt.h>
#include <stdio.h>

void wait_until_key_pressed(void);
int get_random_number_between(int lower_inclusive, int upper_inclusive);
void set_next_mole_position(void);
void draw_mole(void);
uint8_t correct_button_pressed(void);
char* get_int_as_string(int number);
void print_score(void);
void draw_heart(void);
void read_custom_chars(void);
void print_lives(void);

int mole_hit = 0;
int mole_position;
int score = 0;
int lives = 3;


ISR(INT0_vect) {	
	if(correct_button_pressed())
	{
		mole_hit = 1;
		score = score + 1;		
	}
	else
	{
		lives--;
	}
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 61500;
	if(!mole_hit)
	{
		lives--;
	}
	mole_hit = 0;
	set_next_mole_position();
}



/*
** constant definitions
*/
static const PROGMEM unsigned char mole[] =
{
	//0x07, 0x08, 0x13, 0x14, 0x14, 0x13, 0x08, 0x07,
	//0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00
	0b00000,
	0b11011,
	0b01110,
	0b10101,
	0b11111,
	0b01110,
	0b00000,
	0b00000
};

static const PROGMEM unsigned char heart[] =
{
	0b00000,
	0b01010,
	0b11111,
	0b11111,
	0b01110,
	0b00100,
	0b00000,
	0b00000
};


/*
** function prototypes
*/ 


void wait_until_key_pressed(void)
{
    unsigned char temp1, temp2;
    
    do {
        temp1 = PIND;                  // read input
        _delay_ms(5);                  // delay for key debounce
        temp2 = PIND;                  // read input
        temp1 = (temp1 & temp2);       // debounce input
    } while ( temp1 & _BV(PIND2) );	   // BV = Bit to Byte (1 << (bit))
    
    loop_until_bit_is_set(PIND,PIND2);            /* wait until key is released */
}

int get_random_number_between (int lower_inclusive, int upper_inclusive)
{
	return lower_inclusive + rand() % (upper_inclusive + 1 - lower_inclusive);
}

void set_next_mole_position(void)
{
	mole_position = get_random_number_between(0, 7) * 2;
}

void draw_mole(void)
{	
	lcd_gotoxy(mole_position, 1);
	lcd_putc(0);
}

void draw_heart(void)
{	
	lcd_gotoxy(15, 0);
	lcd_putc(1);
}

uint8_t correct_button_pressed(void)
{
	//TODO: Check if correct button has been pressed. 
	//Use mole_position for comparison
	return 1;
}

char* get_int_as_string(int number)
{
	char str[10];
	char *str_ptr = str;
	sprintf(str, "%d", number);
	return str_ptr;
}

void print_score(void)
{
	lcd_gotoxy(0, 0);
	lcd_puts("Score:");
	lcd_gotoxy(7, 0);
	lcd_puts(get_int_as_string(score));
}

void print_lives(void)
{
	lcd_gotoxy(14, 0);
	lcd_puts(get_int_as_string(lives));
	draw_heart();
}

void read_custom_chars(void)
{
	lcd_command(_BV(LCD_CGRAM));  /* set CG RAM start address 0 */
	for(int i=0; i<8; i++)
	{
		lcd_data(pgm_read_byte_near(&mole[i]));
	}
	for(int i=0; i<8; i++)
	{
		lcd_data(pgm_read_byte_near(&heart[i]));
	}
}



int main(void)
{
	cli();
	PORTA = 0xff;
	asm("sei");
    
    
    DDRD &=~ (1 << PD0);        /* Pin PD2 input              */
    PORTD |= (1 << PD0);        /* Pin PD2 pull-up enabled    */
	DDRB = 0xff;
	EIMSK = 1<<INT0;
	
/*	TCNT0 = 0;
	TCCR0B |= (1<<CS02) | (1<<CS00); // PRESCALER 1024
	TIMSK0 = (1<<TOIE0);
	MCUCR = 1<<ISC01 | 1<<ISC00;*/
	
	 TCCR1A = 0;
	 TCCR1B = 0;

	 TCNT1 = 58335;            // Timer nach obiger Rechnung vorbelegen
	 TCCR1B |= (1 << CS12);    // 256 als Prescale-Wert spezifizieren
	 TIMSK1 |= (1 << TOIE1);   // Timer Overflow Interrupt aktivieren
	 
	PORTB = 0x88; // ff aus


    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);
	read_custom_chars();
	set_next_mole_position();

    do {                           /* loop forever */
       lcd_clrscr();   /* clear display home cursor */       
       print_score();
	   print_lives();
	   if(!mole_hit)
	   {
		   draw_mole();        
	   }
	   _delay_ms(100);   
    } while (lives > 0);
	
	lcd_clrscr();   /* clear display home cursor */
	print_score();
	print_lives();
	lcd_gotoxy(3, 1);
	lcd_puts("GAME OVER");
}
