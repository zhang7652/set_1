// const String WDAY_NAMES[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
//wifi
const char *ssid     = "ChinaNet-fkUu";
const char *password = "ipew2yu5";

//time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com",60*60*8, 30*60*1000);
String lastTime = " ";
long time_1 = 0;

//城市信息
/* HTTP请求用的URL，注意网址前面必须添加 "http:" */
#define URL "http://ipinfo.io/ip"
String Payload = " ";

//天气信息组
WiFiClient clientNULL;  //创建一个空的TCP客户端避免冲突和错误
String reqUserKey = "SXhkrLEZYb31FCjBI";  // 私钥
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

//屏幕页面
int i = 0;

//定义引脚
#define TFT_DC 4
#define TFT_CS 5
#define TFT_LED 15
#define HAVE_TOUCHPAD
#define TOUCH_CS 0
#define TOUCH_IRQ  2

// 定义颜色常量
#define MINI_BLACK 0
#define MINI_WHITE 1
#define MINI_YELLOW 2
#define MINI_BLUE 3

// 定义调色板
uint16_t palette[] = {ILI9341_BLACK, // 0
                      ILI9341_WHITE, // 1
                      ILI9341_YELLOW, // 2
                      0x7E3C
                     }; //3

// 定义屏幕尺寸和像素位数
int SCREEN_WIDTH = 240;
int SCREEN_HEIGHT = 320;
int BITS_PER_PIXEL = 2; // 2^2 =  4 colors
ADC_MODE(ADC_VCC);

// 初始化TFT屏幕
ILI9341_SPI tft = ILI9341_SPI(TFT_CS, TFT_DC);
MiniGrafx gfx = MiniGrafx(&tft, BITS_PER_PIXEL, palette);

// 初始化触摸屏
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);
TouchControllerWS touchController(&ts);
TS_Point points[10];  // 存储触摸点的数组

// 定义校准回调函数
void calibrationCallback(int16_t x, int16_t y);
CalibrationCallback calibration = &calibrationCallback;
