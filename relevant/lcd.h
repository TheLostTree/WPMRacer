#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>

// #define USEPORTBFORCTL



#ifdef USEPORTBFORCTL
#define CTL_BUS		PORTB
#define CTL_DDR		DDRB
#define	LCD_RS			0
#define LCD_EN			1
#define LCD_RW          2 

#else
#define CTL_BUS		PORTD
#define CTL_DDR		DDRD
#define LCD_EN			3			
#define	LCD_RS			2
#endif

#define DATA_BUS	PORTD
#define DATA_DDR	DDRD
#define LCD_D4			4
#define LCD_D5			5
#define LCD_D6			6
#define LCD_D7			7


// common ops

#define lcd_rs_low() CTL_BUS &= ~(1<<LCD_RS)
#define lcd_rs_high() CTL_BUS |= (1<<LCD_RS)
#define lcd_en_low() CTL_BUS &= ~(1<<LCD_EN)
#define lcd_en_high() CTL_BUS |= (1<<LCD_EN)
// #define lcd_rw_low() CTL_BUS &= ~(1<<LCD_RW)
// #define lcd_rw_high() CTL_BUS |= (1<<LCD_RW)

#define LCD_CMD_CLEAR_DISPLAY	          0x01
#define LCD_CMD_CURSOR_HOME		          0x02

// Display control
#define LCD_CMD_DISPLAY_OFF                0x08
#define LCD_CMD_DISPLAY_NO_CURSOR          0x0c
#define LCD_CMD_DISPLAY_CURSOR_NO_BLINK    0x0E
#define LCD_CMD_DISPLAY_CURSOR_BLINK       0x0F

// Function set
#define LCD_CMD_4BIT_2ROW_5X7              0x28
#define LCD_CMD_8BIT_2ROW_5X7              0x38


void lcd_write_nibble(uint8_t nibble, volatile uint8_t* dest)
{
	*dest = (*dest & 0x0F) | (nibble & 0b11110000);
	lcd_en_high();
	_delay_us(1);
	lcd_en_low();
	_delay_us(1);
}

// void lcd_send_command_orig(uint8_t command)
// {
// 	// send higher nibble
// 	DATA_BUS=(command&0b11110000); 
// 	CTL_BUS &=~(1<<LCD_RS); // set rs low
// 	CTL_BUS |=(1<<LCD_EN); // set en high
// 	_delay_ms(1);
// 	CTL_BUS &=~(1<<LCD_EN); // set en low
// 	_delay_ms(1);
// 	// send lower nibble
// 	DATA_BUS=((command&0b00001111)<<4);
// 	CTL_BUS |=(1<<LCD_EN); // set en high
// 	_delay_ms(1);
// 	CTL_BUS &=~(1<<LCD_EN); // set en low
// 	_delay_ms(1);
// }

void lcd_send_command(uint8_t command)
{
	lcd_rs_low(); // set rs low
	_delay_us(1);

	// send higher nibble
	lcd_write_nibble(command & 0b11110000, &CTL_BUS);
	// send lower nibble
	lcd_write_nibble((command << 4) & 0b11110000, &CTL_BUS);

}

void lcd_init(void)
{	
	// set up arduino pins
	DATA_DDR = (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5)| (1<<LCD_D4);
	CTL_DDR |= (1<<LCD_EN)|(1<<LCD_RS);

	// set initial states
	DATA_BUS = 0b00000000; 

	lcd_en_high();
	lcd_rs_low();
	// lcd_rw_low();
	// CTL_BUS |= (1<<LCD_EN)|(0<<LCD_RS); // ? why are we |= 0 here

	_delay_ms(1);
	// CTL_BUS &=~(1<<LCD_EN);
	lcd_en_low();
	_delay_ms(1); 
	// 0b00101000 LCD_CMD_4BIT_2ROW_5X7
	// 0b00110011
	lcd_send_command(LCD_CMD_4BIT_2ROW_5X7);
	_delay_ms(1);
	// 0b00001111
	lcd_send_command(LCD_CMD_DISPLAY_CURSOR_BLINK);
	_delay_ms(1);
	// 0b10000000
	lcd_send_command(0x80);
    _delay_ms(10);
	
}

// todo, need to wire up the rw pin
void lcd_check_busy(){
	lcd_rs_low(); // command mode

}
void lcd_write_character(char character)
{
	lcd_rs_high(); //data mode
	lcd_en_low();
	_delay_us(1);
	
	lcd_write_nibble(character & 0b11110000, &DATA_BUS);
	lcd_write_nibble((character<<4) & 0b11110000, &DATA_BUS);
}


void lcd_shift_left()
{
	lcd_send_command(0x18); // shift display left
	_delay_us(50);
}
void lcd_shift_right()
{
	lcd_send_command(0x1C); // shift display right
	_delay_us(50);
}

void lcd_write_str(const char* str)
{
	int i=0;
	while(str[i]!='\0')
	{	

		// todo: might be bad (bc the char sheet isnt totally aligned with ascii)
		if(!isprint(str[i])){
			lcd_write_character('?');
			_delay_us(50);
		} else {
			lcd_write_character(str[i]);
			_delay_us(50);
		}
		i++;
	}
}

void lcd_clear()
{
	lcd_send_command(LCD_CMD_CLEAR_DISPLAY);
	_delay_ms(2); // clear command needs >1.52ms to execute
}
void lcd_goto_xy (uint8_t line,uint8_t pos)				//line = 0 or 1
{
	lcd_send_command((0x80 | (line<<6)) + pos);
	_delay_us (50); // 40 us here
}

char make_printable(char c){
	return isprint(c) ? c : ' ';
}

// Custom character support for 1602 LCD
// CGRAM addresses: 0x00-0x07 (8 custom characters, 5x7 pixels each)
void lcd_define_custom_char(uint8_t char_code, const uint8_t* pattern)
{
	// char_code should be 0-7 (selects which of 8 custom chars to define)
	// pattern should be an array of 8 bytes (5x7 pixel bitmap)
	
	// Set CGRAM address: 0x40 + (char_code * 8)
	lcd_send_command(0x40 | (char_code << 3));
	_delay_us(50);
	
	// Write 8 bytes of pattern data
	for(uint8_t i = 0; i < 8; i++){
		lcd_rs_high(); // data mode
		_delay_us(1);
		lcd_write_nibble(pattern[i] & 0xF0, &DATA_BUS);
		lcd_write_nibble((pattern[i] << 4) & 0xF0, &DATA_BUS);
		_delay_us(50);
	}
	
	// Return to DDRAM (normal character writing)
	lcd_send_command(0x80);
	_delay_us(50);
}

// Write a custom character to the display
void lcd_write_custom_char(uint8_t char_code)
{
	// char_code 0-7 selects which custom character to write
	lcd_rs_high(); // data mode
	_delay_us(1);
	lcd_write_nibble(char_code & 0xF0, &DATA_BUS);
	lcd_write_nibble((char_code << 4) & 0xF0, &DATA_BUS);
	_delay_us(50);
}

void lcd_write_into_hidden(const char * str, uint8_t line, uint8_t size = 16)
{	
	lcd_goto_xy(line, 0);
	int i=0;
	// always iterate to 32
	while(i < 32){
		lcd_write_character(i + 1 > size ? ' ' : make_printable(str[i]));
		_delay_us(50);
		i++;
	}
}

// 17 char strings, 16 chars for the line + \0
void lcd_write_line(const char * str, uint8_t line, uint8_t size = 16)
{
	lcd_goto_xy(line, 0);
	int i=0;
	// always iterate to 16
	while(i < 16){
		lcd_write_character(i + 1 > size ? ' ' : make_printable(str[i]));
		_delay_us(50);
		i++;
	}
}

#endif /* LCD_H_ */