#if !defined(ARDUINO) && !defined(ESP8266)

#include "libSSD1306Display.h"
#include <inttypes.h>
// #include <Arduino.h>
// #include <Wire.h>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//		DL = 1; 8-bit interface data 
//		N = 0; 1-line display 
//		F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//		D = 0; Display off 
//		C = 0; Cursor off 
//		B = 0; Blinking off 
// 4. Entry mode set: 
//		I/D = 1; Increment by 1 
//		S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// libSSD1306Display constructor is called).

void libSSD1306Display::begin()
{	
		// display();

		// clear it off
		SSD1306::OledI2C::clear();
}

void libSSD1306Display::init()
{
	// SSD1306::OledI2C::init();
}

void libSSD1306Display::print(string text)
{
	// SSD1306::OledI2C::init();

	SSD1306::OledI2C::clear();

	// auto length = strftime(time, sizeof(time), "%l:%M:%S %P", tm);
    // int offset = (128 - (8 * length)) / 2;

	SSD1306::OledI2C::displayUpdate();

}

/********** high level commands, for the user! */
// void libSSD1306Display::clear()
// {
// 	// command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
// 	// delayMicroseconds(2000);	// this command takes a long time!
// 	clear();
// }

void libSSD1306Display::home()
{
	// command(LCD_RETURNHOME);	// set cursor position to zero
	// delayMicroseconds(2000);	// this command takes a long time!
}

void libSSD1306Display::setCursor(uint8_t col, uint8_t row)
{
	/* assume 4 lines, the middle two lines
		 are row 0 and 1 */
	cy = (row+1)*fontHeight;
	cx = col*fontWidth;
}

// Turn the display on/off (quickly)
void libSSD1306Display::noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	// command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void libSSD1306Display::display() {
	_displaycontrol |= LCD_DISPLAYON;
	// command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void libSSD1306Display::noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	// command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void libSSD1306Display::cursor() {
	_displaycontrol |= LCD_CURSORON;
	// command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void libSSD1306Display::noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	// command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void libSSD1306Display::blink() {
	_displaycontrol |= LCD_BLINKON;
	// command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void libSSD1306Display::scrollDisplayLeft(void) {
	// command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void libSSD1306Display::scrollDisplayRight(void) {
	// command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void libSSD1306Display::leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	// command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void libSSD1306Display::rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	// command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void libSSD1306Display::autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	// command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void libSSD1306Display::noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	// command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
//void libSSD1306Display::createChar(uint8_t location, uint8_t charmap[]) {
//void libSSD1306Display::createChar(uint8_t location, PGM_P ptr) {
//	location &= 0x7; // we only have 8 locations 0-7
//	// command(LCD_SETCGRAMADDR | (location << 3));
//	for (int i=0; i<8; i++) {
//		//write(charmap[i]);
//		// write(pgm_read_byte(ptr++));
//	}
//}

void libSSD1306Display::createChar(byte idx, PGM_P ptr) {
	if(idx>=0&&idx<NUM_CUSTOM_ICONS) custom_chars[idx]=ptr;
}

// Turn the (optional) backlight off/on
void libSSD1306Display::noBacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	// expanderWrite(0);
}

void libSSD1306Display::backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	// expanderWrite(0);
}

size_t libSSD1306Display::write(uint8_t c) {
	setColor(BLACK);
	fillRect(cx, cy, fontWidth, fontHeight);
	setColor(WHITE);

	if(c<NUM_CUSTOM_ICONS && custom_chars[c]!=NULL) {
		drawXbm(cx, cy, fontWidth, fontHeight, (const byte*) custom_chars[c]);
	} else {
		drawString(cx, cy, String((char)c));
	}
	cx += fontWidth;
	display();	// todo: not very efficient
	return 1;
}
size_t libSSD1306Display::write(const char* s) {
	uint8_t nc = strlen(s);
	setColor(BLACK);
	fillRect(cx, cy, fontWidth*nc, fontHeight);  
	setColor(WHITE);
	drawString(cx, cy, String(s));
	cx += fontWidth*nc;
	display();	// todo: not very efficient
	return nc;
}

/*********** mid level commands, for sending data/cmds */

inline void libSSD1306Display::command(uint8_t value) {
	send(value, 0);
}

inline size_t libSSD1306Display::write(uint8_t value) {
	send(value, Rs);
	return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data
void libSSD1306Display::send(uint8_t value, uint8_t mode) {
	if (_type == LCD_I2C) {
		uint8_t highnib=value&0xf0;
		uint8_t lownib=(value<<4)&0xf0;
		write4bits((highnib)|mode);
		write4bits((lownib)|mode); 
	} 
	if (_type == LCD_STD) {
		digitalWrite(_rs_pin, mode);

		// if there is a RW pin indicated, set it low to Write
		if (_rw_pin != 255) { 
			digitalWrite(_rw_pin, LOW);
		}
		
		write4bits(value>>4);
		write4bits(value);
	}
}

void libSSD1306Display::write4bits(uint8_t value) {
	if (_type == LCD_I2C) {
		expanderWrite(value);
		pulseEnable(value);
	}
	if (_type == LCD_STD) {
		for (int i = 0; i < 4; i++) {
			pinMode(_data_pins[i], OUTPUT);
			digitalWrite(_data_pins[i], (value >> i) & 0x01);
		}

		pulseEnable();
	}
}

void libSSD1306Display::expanderWrite(uint8_t _data){																				 
	// Wire.beginTransmission(_addr);
	// Wire.write((int)(_data) | _backlightval);
	// Wire.endTransmission();		
}

void libSSD1306Display::pulseEnable(uint8_t _data){
	expanderWrite(_data | En);	// En high
	delayMicroseconds(1);		// enable pulse must be >450ns
	
	expanderWrite(_data & ~En);	// En low
	delayMicroseconds(50);		// commands need > 37us to settle
}

void libSSD1306Display::pulseEnable(void) {
	digitalWrite(_enable_pin, LOW);
	delayMicroseconds(1);		 
	digitalWrite(_enable_pin, HIGH);
	delayMicroseconds(1);		 // enable pulse must be >450ns
	digitalWrite(_enable_pin, LOW);
	delayMicroseconds(100);		// commands need > 37us to settle
}

#endif
