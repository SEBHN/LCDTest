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

ISR(INT0_vect) {
	PORTB = 0xFF;
	_delay_ms(2000);
	PORTB = 0x00;
}



/*
** constant definitions
*/
static const PROGMEM unsigned char copyRightChar[] =
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


/*
** function prototypes
*/ 
void wait_until_key_pressed(void);


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


int main(void)
{
	cli();
	//PORTD = 0x00;
	PORTA = 0xff;
	asm("sei");
    char buffer[7];
    int  num=134;
    unsigned char i;
    
    
    DDRD &=~ (1 << PD2);        /* Pin PD2 input              */
    PORTD |= (1 << PD2);        /* Pin PD2 pull-up enabled    */
	DDRB = 0xff;
	EIMSK = 1<<INT0;
	MCUCR = 1<<ISC01 | 1<<ISC00;
	PORTB = 0xff; // ff aus


    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);

    for (;;) {                           /* loop forever */
        /* 
         * Test 1:  write text to display
         */

        /* clear display and home cursor */
        //lcd_clrscr();
//
		///* move cursor to position 6 on line 1 */
		//lcd_gotoxy(0,0);
        //
        ///* put string to display (line 1) with linefeed */
        //lcd_puts("Hi Lukas, Phil\n");
//
		///* move cursor to position 6 on line 1 */
		//lcd_gotoxy(2,1);
//
        ///* cursor is now on second line, write second line */
        //lcd_puts("Cuong, Long");
        //
        ///* wait until push button PD2 (INT0) is pressed */
        //wait_until_key_pressed();
        //
        //
        ///*
         //* Test 2: use lcd_command() to turn on cursor
         //*/
		//
		        ///* clear display and home cursor */
		        //lcd_clrscr();
//
		        ///* move cursor to position 6 on line 1 */
		        //lcd_gotoxy(0,0);
        //
        ///* turn on cursor */
        //lcd_command(LCD_DISP_ON_CURSOR);
//
        ///* put string */
        //lcd_puts( "CurOn");
        //
        ///* wait until push button PD2 (INT0) is pressed */
        //wait_until_key_pressed();
//
//
        ///*
         //* Test 3: display shift
         //*/
        //
        //lcd_clrscr();     /* clear display home cursor */
//
        ///* put string from program memory to display */
        //lcd_puts_P( "Line 1 longer than 14 characters\n" );
        //lcd_puts_P( "Line 2 longer than 14 characters" );
        //
        ///* move BOTH lines one position to the left */
        //lcd_command(LCD_MOVE_DISP_LEFT);
        //
        ///* wait until push button PD2 (INT0) is pressed */
        //wait_until_key_pressed();
//
        ///* turn off cursor */
        //lcd_command(LCD_DISP_ON);
        //
        //
        ///*
         //*   Test: Display integer values
         //*/
        //
        //lcd_clrscr();   /* clear display home cursor */
        //
        ///* convert interger into string */
        //itoa( num , buffer, 10);
        //
        ///* put converted string to display */
        //lcd_puts(buffer);
        //
        ///* wait until push button PD2 (INT0) is pressed */
        //wait_until_key_pressed();
        //
        //
        ///*
         //*  Test: Display userdefined characters
         //*/

       lcd_clrscr();   /* clear display home cursor */
       
       lcd_puts("Copyright: ");
       
       /*
        * load two userdefined characters from program memory
        * into LCD controller CG RAM location 0 and 1
        */
       lcd_command(_BV(LCD_CGRAM));  /* set CG RAM start address 0 */
       for(i=0; i<8; i++)
       {
           lcd_data(pgm_read_byte_near(&copyRightChar[i]));
       }
       
       /* move cursor to position 0 on line 2 */
       /* Note: this switched back to DD RAM adresses */
       lcd_gotoxy(0,1);
       
       /* display user defined (c), built using two user defined chars */
	   
       lcd_putc(0);
	   _delay_ms(1000);
	   lcd_gotoxy(2,1);
	   lcd_putc(0);
       //lcd_putc(1);
       

       /* wait until push button PD2 (INT0) is pressed */
       //wait_until_key_pressed();
       //PORTB = PORTA;       
    }
}
