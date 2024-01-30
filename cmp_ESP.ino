//触摸屏按钮例子
#include <Arduino.h> // 引入Arduino库，提供基础的硬件功能
#include <SPI.h> // 引入SPI库，用于串行外设接口通信

#include <Adafruit_ILI9341esp.h> // 引入Adafruit ILI9341库，用于驱动ILI9341 LCD显示屏
#include <Adafruit_GFX.h> // 引入Adafruit GFX库，提供基本的图形函数
#include <XPT2046.h> // 引入XPT2046库，用于触摸输入

#define TFT_DC 2 // 定义TFT_DC为2，表示LCD显示屏的数据/命令引脚连接到Arduino的第2个数字引脚
#define TFT_CS 15 // 定义TFT_CS为15，表示LCD显示屏的片选引脚连接到Arduino的第15个数字引脚


// 定义TFT屏幕的数据线引脚和片选线引脚
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// 定义触摸屏的片选线引脚和中断引脚
XPT2046 touch(/*cs=*/ 4, /*irq=*/ 5);

// 定义一个按钮对象
Adafruit_GFX_Button button;

void setup() {
  delay(1000);
  
  // 初始化串口通信，波特率为115200
  Serial.begin(115200);
  // 设置SPI频率为ESP_SPI_FREQ
  SPI.setFrequency(ESP_SPI_FREQ);

  // 初始化TFT屏幕
  tft.begin();
  // 初始化触摸屏，传入TFT屏幕的宽度和高度作为参数
  touch.begin(tft.width(), tft.height());  // Must be done before setting rotation
  // 打印TFT屏幕的宽度和高度  x=240 y=320
  Serial.print("tftx ="); Serial.print(tft.width()); 
  Serial.print(" tfty ="); Serial.println(tft.height());
  // 将TFT屏幕填充为黑色
  tft.fillScreen(ILI9341_BLACK);
  // 设置触摸屏的校准参数
  touch.setCalibration(209, 1759, 1775, 273);
  // 初始化按钮，传入TFT屏幕对象、按钮的位置和大小、颜色等参数
  button.initButton(&tft, 20, 300, 40, 30, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "Clear", 1);
  // 绘制按钮
  button.drawButton();
}


static uint16_t prev_x = 0xffff, prev_y = 0xffff; // 初始化前一次触摸点的坐标为无效值

void loop() {
  uint16_t x, y;
  if (touch.isTouching()) { // 检测是否有触摸事件发生
    touch.getPosition(x, y);
    y = tft.height() - y; // 翻转y轴坐标
    x = tft.width() - x; //反转x轴坐标

    // Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
    if (prev_x == 0xffff) { // 如果前一次触摸点的坐标无效
      tft.drawPixel(x, y, ILI9341_BLUE); // 在当前触摸点处绘制一个蓝色的像素点
    } else {
      tft.drawLine(prev_x, prev_y, x, y, ILI9341_BLUE); // 在上一次触摸点和当前触摸点之间绘制一条蓝色的线段
    }
    prev_x = x; // 更新前一次触摸点的坐标
    prev_y = y;
    Serial.print(prev_x);
    Serial.print(" ");
    Serial.println(prev_y);
  } else {
    prev_x = prev_y = 0xffff; // 如果没有触摸事件发生，将前一次触摸点的坐标重置为无效值
  }
  

  button.press(button.contains(x, y)); // 判断触摸点是否在按钮区域内，并设置按钮状态

  // 检查按钮状态是否发生变化
  if (button.justReleased()) {
    tft.fillScreen(ILI9341_BLACK); // 清空屏幕
    button.drawButton(); // 绘制正常状态的按钮
  }

  if (button.justPressed()) {
    button.drawButton(true); // 绘制按下状态的按钮
  }

  delay(20); // 延时20毫秒
}
