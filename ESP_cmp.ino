// 触摸屏小型列子
#include "inti.h"
#include "settings.h"
#include "function.h"
void drawProgress_1();
void drawProgress_2();
void calibrationCallback(int16_t x, int16_t y);
void getip138();
void parseInfoy();
void parseInfos();
void httpRequest();
void dacit();
void init_main();
void drawWeatherImage();
void init_loop();

void setup() {
  init_main();
}

void loop() {
  init_loop();
}