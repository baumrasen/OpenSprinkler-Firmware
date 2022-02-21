#ifndef LIBSSD1306DISPLAY_H
#define LIBSSD1306DISPLAY_H

#if !defined(ARDUINO) && !defined(ESP8266)

#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

#include <getopt.h>

#include "font.h"
#include "images.h"

#define LCD_STD 0			// Standard LCD
#define LCD_I2C 1

// Instantiate the display
ArduiPi_OLED arduoled;


// Config Option
//struct s_opts
{
	int oled;
	int verbose;
} ;

// int sleep_divisor = 1 ;
	
// default options values
//s_opts opts = {
//	OLED_SH1106_I2C_128x64,	// Default oled
//  false						// Not verbose
//};

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

class libSSD1306Display {
public:
	libSSD1306Display() {}
	void init() {
		// // do nothing
		// // SSD1306::OledI2C oled{"/dev/i2c-1", 0x3C};
		// // SPI
		// if (arduoled.oled_is_spi_proto(opts.oled))
		// {
		// 	// SPI change parameters to fit to your LCD
		// 	if ( !arduoled.init(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, opts.oled) )
		// 		exit(EXIT_FAILURE);
		// }
		// else
		// {
		// 	// I2C change parameters to fit to your LCD
		// 	if ( !arduoled.init(OLED_I2C_RESET,opts.oled) )
		// 		exit(EXIT_FAILURE);
		// }

		// I2C change parameters to fit to your LCD

		s_opts opts = {
			OLED_SH1106_I2C_128x64,	// Default oled
		 	false					// Not verbose
		};

		if ( !arduoled.init(OLED_I2C_RESET,opts.oled) )
			exit(EXIT_FAILURE);

	}
	void begin() {

		arduoled.begin();
	
		// init done
		arduoled.clearDisplay();   // clears the screen  buffer
		arduoled.display();   		// display it (clear display)
		
		// Wire.setClock(400000L); // lower clock to 400kHz
		//flipScreenVertically();
		// setFont(Monospaced_plain_13);
		fontWidth = 8;
		fontHeight = 16;
	}
	void clear() {
		// SSD1306::OledI2C oled{"/dev/i2c-1", 0x3C};
		// oled.clear();
		arduoled.clearDisplay();
	}
	void clear(int start, int end) {
		// setColor(BLACK);
		// fillRect(0, (start+1)*fontHeight, 128, (end-start+1)*fontHeight);
		// setColor(WHITE);
	}
	
	uint8_t type() { return LCD_I2C; }
	void noBlink() {/*no support*/}
	void blink() {/*no support*/}
	void setCursor(uint8_t col, int8_t row) {
		/* assume 4 lines, the middle two lines
			 are row 0 and 1 */
		cy = (row+1)*fontHeight;
		cx = col*fontWidth;
	}
	void noBacklight() {/*no support*/}
	void backlight() {/*no support*/}
	size_t write(uint8_t c) {
		// setColor(BLACK);
		// fillRect(cx, cy, fontWidth, fontHeight);
		// setColor(WHITE);

		// static constexpr SSD1306::PixelStyle style{SSD1306::PixelStyle::Set};

		// SSD1306::OledI2C oled{"/dev/i2c-1", 0x3C};

		if(c<NUM_CUSTOM_ICONS && custom_chars[c]!=NULL) {
			// drawXbm(cx, cy, fontWidth, fontHeight, (const byte*) custom_chars[c]);
		} else {
			// drawString(cx, cy, String((char)c));
			// drawString8x16(SSD1306::OledPoint{cx, cy},
			// 			String((char)c),
			// 			SSD1306::PixelStyle::Set,
			// 			oled);
		}
		cx += fontWidth;
		// display();	// todo: not very efficient
		arduoled.display();
		return 1;
	}
	size_t write(const char* s) {
		uint8_t nc = strlen(s);
		// setColor(BLACK);
		// fillRect(cx, cy, fontWidth*nc, fontHeight);  
		// setColor(WHITE);

		// static constexpr SSD1306::PixelStyle style{SSD1306::PixelStyle::Set};

		// SSD1306::OledI2C oled{"/dev/i2c-1", 0x3C};

		// drawString(cx, cy, String(s));
		arduoled.setCursor(cx, cy);
		arduoled.print(s);


		cx += fontWidth*nc;
		// display();	// todo: not very efficient
		// SSD1306::OledI2C::displayUpdate();
		arduoled.display();
		return nc;
	}
	void createChar(byte idx, PGM_P ptr) {
		if(idx>=0&&idx<NUM_CUSTOM_ICONS) custom_chars[idx]=ptr;
	}
private:
	uint8_t cx, cy;
	uint8_t fontWidth, fontHeight;
	PGM_P custom_chars[NUM_CUSTOM_ICONS];
	// SSD1306::OledI2C oled;
};

#endif // not ARDUINO or not ESP8266

#endif // LIBSSD1306DISPLAY_H



