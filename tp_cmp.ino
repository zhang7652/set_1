//图片例子
#include "SPI.h"
#include <Adafruit_ILI9341.h>
#include "dragon.h"

// For the Adafruit shield, these are the default.
//#define TFT_DC 9
//#define TFT_CS 10

// Feather 32u4 or M0 with TFT FeatherWing:
#define TFT_DC 2
#define TFT_CS 5
// ESP8266:
//#define TFT_DC 15
//#define TFT_CS 0
// Other boards (including Feather boards) may have other pinouts;
// see learn.adafruit.com/adafruit-2-4-tft-touch-screen-featherwing/pinouts

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  tft.begin();
}

void loop(void) {
  for(uint8_t r=0; r<4; r++) {
    tft.setRotation(r);
    tft.fillScreen(ILI9341_BLACK);
    for(uint8_t j=0; j<20; j++) {
      tft.drawRGBBitmap(
        random(-DRAGON_WIDTH , tft.width()),
        random(-DRAGON_HEIGHT, tft.height()),
#if defined(__AVR__) || defined(ESP8266)
        dragonBitmap,
#else
        // Some non-AVR MCU's have a "flat" memory model and don't
        // distinguish between flash and RAM addresses.  In this case,
        // the RAM-resident-optimized drawRGBBitmap in the ILI9341
        // library can be invoked by forcibly type-converting the
        // PROGMEM bitmap pointer to a non-const uint16_t *.
        (uint16_t *)dragonBitmap,
#endif
        DRAGON_WIDTH, DRAGON_HEIGHT);
      delay(1); // Allow ESP8266 to handle watchdog & WiFi stuff
    }
    delay(3000);
  }
}
