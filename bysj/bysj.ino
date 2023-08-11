#include <ArduinoJson.h>  //是一个用于处理JSON格式数据的库，可以方便地解析和生成JSON对象。
#include <ESP8266WiFi.h>  //是一个用于连接WiFi网络的库，可以让ESP8266通过无线方式与互联网通信。
#include <SPI.h>  //是一个用于串行外设接口（Serial Peripheral Interface）的库，可以让ESP8266与其他设备进行高速数据传输。
#include <U8g2lib.h>  //是一个用于驱动OLED显示屏的库，可以让ESP8266在屏幕上显示文字和图形。
#include <WiFiUdp.h>  //是一个用于UDP协议的库，可以让ESP8266通过无连接的方式发送和接收数据包。
#include <TimeLib.h>  //是一个用于处理时间和日期的库，可以让ESP8266获取和设置系统时间。
#include <DNSServer.h>  //是一个用于DNS服务的库，可以让ESP8266解析域名和IP地址。
#include <ESP8266WebServer.h>  //是一个用于创建Web服务器的库，可以让ESP8266响应HTTP请求和提供网页内容。
#include "bysj_1.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);   //设置OLED屏幕格式

//保存断网前的最新数据
int results_0_now_temperature_int_old;
int results_0_now_code_int_old;

//网络设置
WiFiUDP Udp;
const int NTP_PACKET_SIZE = 48;     // NTP时间在消息的前48个字节里
byte packetBuffer[NTP_PACKET_SIZE]; // 输入输出包的缓冲区
static const char ntpServerName[] = "ntp1.aliyun.com"; //NTP服务器，使用阿里云
boolean isNTPConnected = false;  //定义一个bool变量表示是否连接了服务器
WiFiClient clientNULL;  //创建一个空的TCP客户端避免冲突和错误
DNSServer dnsServer;  //创建一个DNS服务器可以拦截DNS请求并且返回IP
ESP8266WebServer server(80);  //创建一个WEB服务器  处理HTTP请求和响应
time_t prevDisplay = 0; //当时钟已经显示
unsigned long timer = 0;  //记录定时器的时间

// 心知天气HTTP请求所需信息
const char* host = "api.seniverse.com";   // 将要连接的服务器地址  
         
void setup()
{
  Serial.begin(112500);  //设置串口波特率
  u8g2.begin();  //初始化OLED
  u8g2.enableUTF8Print();  //启动UTF-8编码打印功能,方便显示其他语言的字符
  WiFi.begin("ChinaNet-fkUu", "ipew2yu5");    // 连接这个网络
  while(WiFi.status()!=WL_CONNECTED){
    //太空人动画
    const uint8_t* gImage[] = {gImage_1, gImage_2, gImage_3, gImage_4, gImage_5, gImage_6, gImage_7, gImage_8, gImage_9, gImage_10, gImage_11, gImage_12, gImage_13, gImage_14, gImage_15, gImage_16, gImage_17, gImage_18, gImage_19, gImage_20};
    for (int i = 0; i < 20; i++)
    {
      u8g2.clearBuffer();
      u8g2.drawXBM(0, 0, 128, 64, gImage[i]);
      u8g2.sendBuffer();
      delay(40);
    }
  }  //WiFi.status()函数的返回值如果连接成功返回WL_CONNECTED
  Udp.begin(8888);
  setSyncProvider(bao);  //设置时间同步后执行的函数  通过UDP向NTP服务器发送请求，获取当前时间
  httpRequest();  //获取当前温度
  setSyncInterval(300); //300秒同步一次
}

void loop(){
  server.handleClient();  //处理客户端的请求
  dnsServer.processNextRequest();  //处理DNS服务器的下一个请求
  if (timeStatus() != timeNotSet)  //表示现在时间是否设置
  {
      if (now() != prevDisplay)  //如果上一次时间与现在时间不同变更新时间
      { //时间改变时更新显示
          prevDisplay = now();
          u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
        //定义一个变量来表示当前的天气，0-阴，1-多云，2-夜晚晴，3-雨，4-晴
        uint8_t currentWeather;
        //定义一个数组来存储不同天气对应的图标
        uint8_t weatherIcons[] = {64, 65, 66, 67, 69};
        switch(results_0_now_code_int_old)
        {
          case 0:currentWeather=4;break;
          case 1:currentWeather=2;break;
          case 2:currentWeather=4;break;
          case 3:currentWeather=2;break;
          case 4:currentWeather=0;break;
          case 5:currentWeather=1;break;
          case 6:currentWeather=1;break;
          case 7:currentWeather=0;break;
          case 8:currentWeather=0;break;
          case 9:currentWeather=0;break;
          default:currentWeather=3;break;
        }
        //根据当前的天气绘制对应的图标
        u8g2.drawGlyph(3, 16, weatherIcons[currentWeather]);

        u8g2.setFont(u8g2_font_open_iconic_www_2x_t);
        //绘制是否已经连接服务器
        if(isNTPConnected){u8g2.drawGlyph(90, 18, 76);}//已连接
        else{u8g2.drawGlyph(90, 18, 67);}  //未连接

        //绘制是否连接网络
        if(WiFi.status()==WL_CONNECTED)
        {u8g2.drawGlyph(110, 16, 72);}  //有网络绘制wifi图标
        else{u8g2.drawGlyph(110, 16, 74);}  //无网络绘制无网络图标

        u8g2.setFont(u8g2_font_unifont_t_chinese2); //设置字体为unifont的符号集
        u8g2.setCursor(20, 16);
        u8g2.print(results_0_now_temperature_int_old);
        u8g2.setFont(u8g2_font_logisoso16_tf);
        u8g2.setCursor(33, 19);
        u8g2.print("°C");

        //画一条横线
        u8g2.drawLine(/*x-start*/0, /*y-start*/18, /*x-end*/128, /*y-end*/18);
        //使用sprint 函数格式化代码
        char currentTime[9];
        sprintf(currentTime, "%02d:%02d:%02d", hour(), minute(), second());

        //显示时间部分
        u8g2.setFont(u8g2_font_logisoso24_tr);
        u8g2.setCursor(9, 47);
        u8g2.print(currentTime);
        u8g2.setCursor(0, 61);
        u8g2.setFont(u8g2_font_unifont_t_chinese2);
        //串口监视器
        char currentDateTime[21];
        sprintf(currentDateTime, "%d/%02d/%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
        Serial.println("当前时间:" + String(currentDateTime));
        
        char currentDay[11];
        sprintf(currentDay, "%d/%02d/%02d", year(), month(), day());
        //显示日期部分
        u8g2.print(currentDay);
        u8g2.drawXBM(80, 48, 16, 16, xing);
        u8g2.setCursor(95, 62);
        u8g2.print("期");
        //用一个switch-case语句来显示不同的星期
        switch(weekday())
        {
          case 1:u8g2.print("日");break;
          case 2:u8g2.print("一");break;
          case 3:u8g2.print("二");break;
          case 4:u8g2.print("三");break;
          case 5:u8g2.print("四");break;
          case 6:u8g2.print("五");break;
          case 7:u8g2.drawXBM(111, 49, 16, 16, liu);break;
        }
        u8g2.drawFrame(/*x*/0,/*y*/0,/*width*/128,/*height*/64);
        u8g2.sendBuffer();
      }
  }
}   //重复执行


// 向心知天气服务器请求信息并对信息进行解析
void httpRequest(){
  char reqRes[100] = "";
  WiFiClient client;
  sprintf(reqRes, "/v3/weather/now.json?key=%s&location=%s&language=en&unit=c", "SXhkrLEZYb31FCjBI", "29.29:106.26");
  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + host + "\r\n" + 
                              "Connection: close\r\n\r\n";
  if (client.connect(host, 80)){ // 尝试连接服务器，如果不成功就没有天气信息
    client.print(httpRequest);// 向服务器发送http请求信息
    String status_response = client.readStringUntil('\n');// 获取并显示服务器响应状态行 
    if (client.find("\r\n\r\n"));// 使用find跳过HTTP响应头
    parseInfo_now(client); // 利用ArduinoJson库解析心知天气响应信息
  }
  else {
      parseInfo_now(clientNULL);   // 利用ArduinoJson库解析心知天气响应信息
  }
  client.stop();   //断开客户端与服务器连接工作
}


// 利用ArduinoJson库解析心知天气响应信息
void parseInfo_now(WiFiClient client){
  // 定义JSON文档的容量
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, client);
  JsonObject results_0 = doc["results"][0];
  JsonObject results_0_now = results_0["now"];
  String results_0_last_update_str = results_0["last_update"].as<String>();     // "2020-06-02T14:40:00+08:00" 
  results_0_now_code_int_old = results_0_now["code"].as<int>();   // "0"
  results_0_now_temperature_int_old = results_0_now["temperature"].as<int>();   // "32"
}


//NTP
time_t bao()
{
  IPAddress ntpServerIP; // NTP服务器的地址
  while(Udp.parsePacket()>0);  //丢弃所有包
  WiFi.hostByName(ntpServerName, ntpServerIP);  // 从池中获取随机服务器放置在ntpServerName
  sendNTP_packet(ntpServerIP);   //向地址处发送请求
  uint32_t beginWait = millis();  //定义一个变量存储请求离开的毫秒数
  while (millis() - beginWait < 1500)// 离开时间减去现在时间
  {
      int size = Udp.parsePacket();  //获取包的大小
      if (size >= NTP_PACKET_SIZE)   //如果超过了48说明有效
      {
          isNTPConnected = true;  //表示连接到了服务器
          Udp.read(packetBuffer, NTP_PACKET_SIZE); // 将数据包读取到缓冲区
          unsigned long secsSince1900;    //定义一个无符号长整型变量  存储1900-1-1-0-0 UTC到现在的秒速
          // 将从位置40开始的四个字节转换为长整型，只取前32位整数部分
          secsSince1900 = (unsigned long)packetBuffer[40] << 24;
          secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
          secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
          secsSince1900 |= (unsigned long)packetBuffer[43];
          return secsSince1900 - 2208988800UL + 8 * SECS_PER_HOUR;  //返回现在的时间
      }
  }
  isNTPConnected = false;  //表示没有连接到服务器
  return 0;  //无时间
}

// 向给定地址的时间服务器发送NTP请求
void sendNTP_packet(IPAddress &address)
{
  // 用于标识NTP请求的随机数
  const uint32_t NTP_ID = 0x31314E52; // 49.78.49.52
  memset(packetBuffer, 0, NTP_PACKET_SIZE);  //设置缓冲区的所有字节为0，并且初始化一个空的包
  packetBuffer[0] = 0b11100011; // 闰秒指示符, 版本号, 模式
  packetBuffer[1] = 0;          // 层次
  packetBuffer[2] = 6;          // 间隔
  packetBuffer[3] = 0xEC;       // 精度
  //根延迟/根离散度
  //写入NTP_ID作为标识符
  *(uint32_t*)(packetBuffer + 12) = NTP_ID;
  Udp.beginPacket(address, 123); //指定目标地址和端口号  NTP需要使用的UDP端口号为123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);   //写入缓冲区，字节为48
  Udp.endPacket();   //结束发送UDP数据包,并且发送到目标服务器
}