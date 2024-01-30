// #include <Arduino.h> // 引入Arduino库，提供基础的硬件功能
#include <SPI.h> // 引入SPI库，用于串行外设接口通信
#include <WiFiUdp.h>  //进行Wi-Fi通信，包括UDP协议的支持
#include <NTPClient.h>  //获取网络时间协议（Network Time Protocol，NTP）的时间信息
#include <ESP8266WiFiMulti.h>                   // 预存多个WiFi需要使用
#include <ESP8266HTTPClient.h>                  // HTTP通信库需要使用
// #include <ESP8266WiFi.h>  //wifi库
#include <Adafruit_ILI9341esp.h> // 引入Adafruit ILI9341库，用于驱动ILI9341 LCD显示屏
// #include <Adafruit_GFX.h> // 引入Adafruit GFX库，提供基本的图形函数
#include <XPT2046.h> // 引入XPT2046库，用于触摸输入
#include <ArduinoJson.h>  //处理JSON数据


#define TFT_DC 2 // 定义TFT_DC为2，表示LCD显示屏的数据/命令引脚连接到Arduino的第2个数字引脚
#define TFT_CS 15 // 定义TFT_CS为15，表示LCD显示屏的片选引脚连接到Arduino的第15个数字引脚

// 定义TFT屏幕的数据线引脚和片选线引脚
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// 定义触摸屏的片选线引脚和中断引脚
XPT2046 touch(/*cs=*/ 4, /*irq=*/ 5);

static uint16_t prev_x = 0xffff, prev_y = 0xffff; // 初始化前一次触摸点的坐标为无效值
int i = 0;
ESP8266WiFiMulti wifiMulti;                     // 新建ESP8266WiFiMulti对象,对象名称是'wifiMulti'

//界面
int tetx = 0;
bool flag = false;
uint16_t x, y;

//城市信息
/* HTTP请求用的URL，注意网址前面必须添加 "http:" */
#define URL "http://ipinfo.io/ip"
String Payload = " ";

//天气信息组
WiFiClient clientNULL;  //创建一个空的TCP客户端避免冲突和错误
String reqUserKey = "SXhkrLEZYb31FCjBI";  // 私钥
String name = " ";
const char* host = "api.seniverse.com";  // 将要连接的服务器地址 
struct DailyWeather {
 String date = " ";
 int code_day=99;
 int high;
 int low;
 float rainfall;
 float precip;
 String wind_direction=" ";
 int wind_speed;
 int wind_scale;
 int humidity;
 int temperature;
};
struct FutureWeather {
 String date = " "; 
 int code_day=99;
 int high;
 int low;
 float rainfall;
 String wind_direction= " ";
 int wind_speed;
 int wind_scale;
 int humidity;
};
// 定义天气结构体数组
DailyWeather dailyData[1];
FutureWeather futureData[2];
String reqRes1 = "/v3/weather/daily.json?key=" + reqUserKey +
         + "&location=" + Payload + "&language=en&unit=c&start=0&days=3"; //预测
String reqRes2 = "/v3/weather/now.json?key=" + reqUserKey +
         + "&location=" + Payload + "&language=en&unit=c"; //实时

//显示按钮
Adafruit_GFX_Button button1;
Adafruit_GFX_Button button2;
Adafruit_GFX_Button button3;

//time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com",60*60*8, 30*60*1000);
String lastTime = " ";
long time_1 = 0;