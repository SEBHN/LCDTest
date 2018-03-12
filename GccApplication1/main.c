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
#include <lcd.h>


/*
** constant definitions
*/
static const PROGMEM unsigned char copyRightChar[] =
{
	0x07, 0x08, 0x13, 0x14, 0x14, 0x13, 0x08, 0x07,
	0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00
};


//int main(void)
//{
    //char buffer[7];
    //int  num=134;
    //unsigned char i;
    //
    //
    //DDRD &=~ (1 << PD2);        /* Pin PD2 input              */
    //PORTD |= (1 << PD2);        /* Pin PD2 pull-up enabled    */
//
//
    ///* initialize display, cursor off */
    //lcd_init(LCD_DISP_ON);
//
    //lcd_clrscr();   /* clear display home cursor */
     //
    //lcd_puts("Copyright: ");              
   //
//}
