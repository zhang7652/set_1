#ifndef WEATHER_DATA_H
#define WEATHER_DATA_H

// 心知天气HTTP请求所需信息配置
const char* host = "api.seniverse.com";   // 将要连接的服务器地址  

//服务器请求配置
static const char ntpServerName[] = "ntp1.aliyun.com"; //NTP服务器，使用阿里云
boolean isNTPConnected = false;  //定义一个bool变量表示是否连接了服务器
byte packetBuffer[48]; // 输入输出包的缓冲区

//当前时钟时间
time_t prevDisplay = 0; 

//天气信息组
struct DailyWeather {
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

//图片库太空人图片数量配置
int frames =20; 

// 心知天气HTTP请求所需信息
String reqUserKey = "SXhkrLEZYb31FCjBI";   // 私钥
String reqRes1 = "/v3/weather/daily.json?key=" + reqUserKey +
                  + "&location=chongqing&language=en&unit=c&start=0&days=3";  //预测
String reqRes2 = "/v3/weather/now.json?key=" + reqUserKey +
                  + "&location=chongqing&language=en&unit=c";  //实时
//设置动画阻塞时间
unsigned long previousMillis2 = 0;

// 声明静态变量并初始化为当前时间  切换页面
boolean isyemian = true;  //定义一个bool变量表示是否连接了服务器

//设置好久同步一次
unsigned long previousMillis1 = 0;  // 上次执行httpRequest的时间
bool httpRequestFlag = false;  // 标记是否执行httpRequest

//重新连接次数
int retryCount = 0; 
#endif