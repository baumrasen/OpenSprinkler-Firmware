#ifndef LIBSSD1306DISPLAY_H
#define LIBSSD1306DISPLAY_H

#if !defined(ARDUINO) && !defined(ESP8266)

#include "OledFont8x8.h"
#include "OledFont8x16.h"
#include "OledI2C.h"

#include "font.h"
#include "images.h"

#define LCD_STD 0			// Standard LCD
#define LCD_I2C 1

class libSSD1306Display : public SSD1306::OledI2C {
public:
	libSSD1306Display(const std::string& device, uint8_t address) : SSD1306::OledI2C(device, address) {}
	void begin() {
		// Wire.setClock(400000L); // lower clock to 400kHz
		//flipScreenVertically();
		// setFont(Monospaced_plain_13);
		fontWidth = 8;
		fontHeight = 16;
	}
	void clear() {
		SSD1306::OledI2C::clear();
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

		static constexpr SSD1306::PixelStyle style{SSD1306::PixelStyle::Set};

		if(c<NUM_CUSTOM_ICONS && custom_chars[c]!=NULL) {
			// drawXbm(cx, cy, fontWidth, fontHeight, (const byte*) custom_chars[c]);
		} else {
			// drawString(cx, cy, String((char)c));
			drawString8x16(SSD1306::OledI2C::OledPoint{cx, cy},
						String((char)c),
						SSD1306::OledI2C::PixelStyle::Set,
						SSD1306::OledI2C;
		}
		cx += fontWidth;
		// display();	// todo: not very efficient
		SSD1306::OledI2C::displayUpdate();
		return 1;
	}
	size_t write(const char* s) {
		uint8_t nc = strlen(s);
		// setColor(BLACK);
		// fillRect(cx, cy, fontWidth*nc, fontHeight);  
		// setColor(WHITE);

		static constexpr SSD1306::OledI2C::PixelStyle style{SSD1306::OledI2C::PixelStyle::Set};

		// drawString(cx, cy, String(s));
		drawString8x16(SSD1306::OledI2C::OledPoint{cx, cy},
                               String(s),
                               SSD1306::OledI2C::PixelStyle::Set,
                               SSD1306::OledI2C;

		cx += fontWidth*nc;
		// display();	// todo: not very efficient
		SSD1306::OledI2C::displayUpdate();
		return nc;
	}
	void createChar(byte idx, PGM_P ptr) {
		if(idx>=0&&idx<NUM_CUSTOM_ICONS) custom_chars[idx]=ptr;
	}
private:
	uint8_t cx, cy;
	uint8_t fontWidth, fontHeight;
	PGM_P custom_chars[NUM_CUSTOM_ICONS];
};

#endif // not ARDUINO or not ESP8266

#endif // LIBSSD1306DISPLAY_H



