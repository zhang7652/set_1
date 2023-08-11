#include <TFT_eSPI.h>
#include <TimeLib.h>  
#include <Ticker.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <DNSServer.h>

//WIFI库
#include <ESP8266WiFi.h>  
// #include <WiFiManager.h>
#include <ESP8266WiFiMulti.h>
//#include <ESP8266WebServer.h>

//自定义库
#include "Arial_font.h" //字体库
#include "Image.h" //图片库
#include "data_configuration.h" //数据配置库

WiFiUDP Udp;
//TFT_ESPI类  
TFT_eSPI tft;
//三个精灵对象
TFT_eSprite clk = TFT_eSprite(&tft);
TFT_eSprite tft_c = TFT_eSprite(&tft);
WiFiClient clientNULL;  //创建一个空的TCP客户端避免冲突和错误

// 建立WiFi对象
ESP8266WiFiMulti wifiMulti;

// 定时器配置
Ticker timer;
int isConnected = 0;

//AP设置
#define AP_SSID "EPS8266_TCP_KHD"
#define AP_PSW  "1234567890"
#define MAX_CONNECT 5
const int SERVER_PORT = 80;

IPAddress local_IP(192, 168, 5, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer WiFi_Server(SERVER_PORT);
WiFiClient WiFi_Clients[MAX_CONNECT];

unsigned long lastInteractionTimes[MAX_CONNECT];
static unsigned long pageSwitchMillis = millis();

void updateLastInteractionTime(int index) {
  lastInteractionTimes[index] = millis();
}
unsigned long getLastInteractionTime(int index) {
  return lastInteractionTimes[index];
}
void setup() {
  //----------WIFI连接配置----------//
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.invertDisplay(true);

  WiFi.mode(WIFI_AP_STA);  // 设置为混合AP和客户端模式
  // 设置AP的配置
  WiFi.softAPConfig(local_IP, gateway, subnet);

  WiFi.softAP(AP_SSID, AP_PSW,3,0);
  IPAddress ip = WiFi.softAPIP();
  Serial.println(ip);
  // 开始WiFi服务器
  WiFi_Server.begin();

  //WiFi.persistent(true); // 启用持久化存储WiFi数据
  //使用AP添加WiFi
  wifiMulti.addAP("ChinaNet-fkUu","ipew2yu5");
  wifiMulti.addAP("SSI","123456789");

  tft.fillScreen(TFT_BLACK);
  tft.loadFont(zhunyuan_30);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK, false);
  printText(0, 5, "重庆工程职业技术");
  printText(80, 35, "学院");
  printText(10, 65, "19电专超玩科技");
  printText(40, 95, "网络连接中");
  printText(120, 215, "--张洪银");
  for (int i = 0; i <  +200; i++) //进度条
  {
    tft.fillRect(0, 125,  i,  10, TFT_RED);
    delay(20);
  }

  // 连接WiFi
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  tft.fillScreen(TFT_BLACK);
  // 显示连接成功信息
  Serial.print("WiFi名称:");
  Serial.println(WiFi.SSID()); // 打印WiFi名称
  Serial.print("内网IP地址:"); 
  Serial.println(WiFi.localIP()); //打印IP地址
  
  tft.setCursor(20, 90, 4);
  tft.print("网络连接成功!");
  tft.unloadFont();
  delay(1000);
  tft.fillScreen(TFT_BLACK);

  Udp.begin(8888);
  setSyncProvider(bao);
  // 发起API请求
  httpRequest(reqRes1, 0);
  httpRequest(reqRes2, 1);
  setSyncInterval(300);
}
//开始页面显示文字
void printText(int x, int y, const char* text) {
  tft.setCursor(x, y, 4);
  tft.print(text);
}
//显示文字 页面1
void updateTimeDisplay1() {
  tft.loadFont(zhunyuan_30);

  tft.setTextColor(TFT_MAGENTA, TFT_TRANSPARENT);
  tft.setCursor(0, 130, 4);
  tft.printf("%d-%d℃", dailyData[0].low, dailyData[0].high);
  tft.setCursor(55, 1, 4);
  if(dailyData[0].temperature>=30){
    tft.setTextColor(TFT_BLUE, TFT_TRANSPARENT);
  }
  tft.printf("%d℃", dailyData[0].temperature);

  tft.setTextColor(TFT_RED, TFT_TRANSPARENT);
  tft.setCursor(0, 155, 4);
  tft.printf("%d%% RH", dailyData[0].humidity);
  tft.setCursor(0, 185, 4);
  tft.printf("%.0f%% 有雨", dailyData[0].precip);

  tft.setTextColor(TFT_YELLOW, TFT_TRANSPARENT);
  tft.setCursor(0, 215, 4);
  tft.printf("%s风     %dkm/h    %d级", dailyData[0].wind_direction, dailyData[0].wind_speed, dailyData[0].wind_scale);

  tft.setTextColor(TFT_CYAN, TFT_TRANSPARENT);
  tft.setCursor(135, 1, 4);
  tft.printf("%.2fmm", dailyData[0].rainfall);

  tft.setTextColor(TFT_GREEN, TFT_TRANSPARENT);
  tft.setCursor(0, 103, 4);
  tft.printf("%d/%d", month(), day());
  switch(weekday())
  {
    case 1:tft.print(" Sun");break;
    case 2:tft.print(" Mon");break;
    case 3:tft.print(" Tue");break;
    case 4:tft.print(" Wed");break;
    case 5:tft.print(" Thur");break;
    case 6:tft.print(" Fri");break;
    case 7:tft.print(" Sat");break;
  }
  tft.setCursor(190, 65, 4);
  tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  tft.print(":");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK, true);
  if (int(second()) < 10) {
    tft.print("0");
  }
  tft.print(int(second()));
  tft.unloadFont();             //释放字库,节省RAM

  // Update time
  tft.setTextColor(TFT_RED, TFT_BLACK, true);
  tft.loadFont(zhunyuan_90);
  tft.setCursor(0, 26, 4);
  if(int(hour())<10){tft.print("0");}
  tft.print(int(hour()));
  tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  tft.setCursor(75, 26, 4);
  tft.print(":");
  tft.setCursor(100, 26, 4);
  tft.setTextColor(TFT_RED, TFT_BLACK, true);
  if(int(minute())<10){tft.print("0");}
  tft.print(int(minute()));
  tft.unloadFont();             //释放字库,节省RAM
}
//显示文字 页面2
void updateTimeDisplay2() {
  tft.setCursor(30, 5, 4);
  tft.setTextColor(TFT_PINK, TFT_TRANSPARENT);
  tft.loadFont(zhunyuan_30);
  tft.printf("重庆未来天气");
  tft.setTextColor(TFT_RED, TFT_TRANSPARENT);
  tft.setCursor(10, 65, 4);
  tft.printf("%d-%d℃ %d-%d℃", futureData[0].low, futureData[0].high, futureData[1].low, futureData[1].high);
  tft.setCursor(40, 92, 4);
  tft.printf("%s风        %s风", futureData[0].wind_direction, futureData[1].wind_direction);
  tft.setCursor(30, 120, 4);
  tft.setTextColor(TFT_YELLOW, TFT_TRANSPARENT);
  tft.printf("%dkm/h      %dkm/h", futureData[0].wind_speed, futureData[1].wind_speed);
  tft.setCursor(40, 150, 4);
  tft.printf("%d级          %d级", futureData[0].wind_scale, futureData[1].wind_scale);
  tft.setCursor(20, 180, 4);
  tft.setTextColor(TFT_MAGENTA, TFT_TRANSPARENT);
  tft.printf("%d%% RH     %d%% RH", futureData[0].humidity, futureData[1].humidity);
  tft.setCursor(15, 210, 4);
  tft.printf("%.2fmm    %.2fmm", futureData[0].rainfall, futureData[1].rainfall);
  tft.unloadFont();             //释放字库,节省RAM
}
void TFT_ESPI_QH(){
  // 检查是否需要切换页面
  if (millis() - pageSwitchMillis >= 30000) {  // 切换页面的间隔为30秒
    pageSwitchMillis = millis(); // 重置计时器
    isyemian = !isyemian; // 切换页面状态
    if(isyemian){
      for (int y = 0; y < tft.height(); y += 1) {
        tft.fillRect(0, y, tft.width(), 1, TFT_BLACK); // 逐行清理
        delay(5);
      }
    }
    else{
      for (int x = 0; x < tft.width(); x += 1) {
        tft.fillRect(x, 0, 1, tft.height(), TFT_BLACK); // 逐列清理
        delay(5);
      }
    }
  }
}
//无线循环
void loop() {
  TFT_ESPI_QH();
  WIFI_DH_QH();
  wifi_culie();
}
void WIFI_DH_QH(){
  tft.setTextWrap(true);
  if(isyemian == true){
    httpRequest_s(0); //预测
    httpRequest_s(1); //实时
    //Serial.printf("当前时间:%d/%d/%d %d:%d:%d Weekday:%d\n", year(), month(), day(), hour(), minute(), second(), weekday());
    if (timeStatus() != timeNotSet) {  // Check if the time is set
      if (now() != prevDisplay) {  // If the time has changed, update the display
        prevDisplay = now();
        updateTimeDisplay1();
      }
    }
    if (millis() - previousMillis2 >= 70) {  //动画切换时间
      previousMillis2 = millis();
      frames = (frames + 1) % 20;  // Increment the image index and loop back to 0 after the last image
      xians_TFT1();
    }
  }
  else{
    xians_TFT2();
    updateTimeDisplay2();
    if (WiFi.status() != WL_CONNECTED) {
      unsigned long connectStartTime = millis();
      while (millis() - connectStartTime >= 5000) {
        if (wifiMulti.run() == WL_CONNECTED) {
          break;
        }
      }
    }
  }
}
//处理接收的数据
void handleClientData(WiFiClient client) {
  // 读取接收的数据
  char data[128];
  int bytesRead = client.readBytesUntil('\n', data, sizeof(data) - 1);
  if (bytesRead > 0) {
    data[bytesRead] = '\0';

    // 处理接收到的数据
    String receivedData = String("Received data: ") + data;
    if(String(data)=="1"){
      pageSwitchMillis = millis(); // 重置计时器
      isyemian = !isyemian; // 切换页面状态
      if(isyemian){
        for (int y = 0; y < tft.height(); y += 1) {
          tft.fillRect(0, y, tft.width(), 1, TFT_BLACK); // 逐行清理
          delay(5);
        }
      }
      else{
        for (int x = 0; x < tft.width(); x += 1) {
          tft.fillRect(x, 0, 1, tft.height(), TFT_BLACK); // 逐列清理
          delay(5);
        }
      }
      Serial.println("页面切换成功!");
    }
    else{
      Serial.println(receivedData);
    }
    // 发送回复给客户端
    String response = "接收成功!";
    client.write(response.c_str());
  }
}
void wifi_culie(){
  // 检查是否有新的客户端连接
  if (WiFi_Server.hasClient()) {
    // 找到一个可用的位置并保存客户端连接
    int availableIndex = -1;
    for (int i = 0; i < MAX_CONNECT; i++) {
      if (!WiFi_Clients[i] || !WiFi_Clients[i].connected()) {
        availableIndex = i;
        break;
      }
    }

    if (availableIndex != -1) {
      WiFi_Clients[availableIndex] = WiFi_Server.available();
      if (WiFi_Clients[availableIndex]) {
        WiFi_Clients[availableIndex].setNoDelay(true);
        Serial.print("New client connected: ");
        Serial.println(WiFi_Clients[availableIndex].remoteIP());
      }
    }
  }

  // 遍历连接池来处理接收到的数据
  int activeConnections = 0;
  unsigned long timeoutPeriod = 60000; // 60秒
  for (int i = 0; i < MAX_CONNECT; i++) {
    if (WiFi_Clients[i] && WiFi_Clients[i].connected()) {
      activeConnections++;

      if (WiFi_Clients[i].available() && WiFi_Clients[i].availableForWrite()) {
        handleClientData(WiFi_Clients[i]);
        // 更新最后交互时间
        updateLastInteractionTime(i);
      } else {
        // 计算时间差并断开超时连接
        unsigned long lastInteractionTime = getLastInteractionTime(i);
        if (millis() - lastInteractionTime > timeoutPeriod) {
          WiFi_Clients[i].println("连接超时!"); // 或者使用其他适当的消息内容
          WiFi_Clients[i].flush();
          // 清空WiFi_Clients[i]中的数据
          WiFi_Clients[i] = WiFiClient();
          WiFi_Clients[i].stop(); // 断开没有数据传输的连接
        }
      }
    } else {
      // 关闭无效的连接
      WiFi_Clients[i].stop();
      // 清空WiFi_Clients[i]中的数据
      WiFi_Clients[i] = WiFiClient();
      // 清除最后交互时间
      lastInteractionTimes[i] = 0;
    }
  }
}
//30分钟执行一次获取天气信息
void httpRequest_s(int i){
  unsigned long currentMillis1 = millis();  // 获取当前时间
  unsigned long currentMillis2 = millis();  // 获取当前时间
  if (currentMillis1 - previousMillis1 >= 1800000 and i==0) {  // 检查是否已经过了30分钟,切换时间
    previousMillis1 = currentMillis1;  // 更新上次执行httpRequest的时间
    httpRequest(reqRes1, i);
  }
  if (currentMillis2 - previousMillis1 >= 300000 and i==1) {  // 检查是否已经过了5分钟,切换时间
    previousMillis1 = currentMillis2;  // 更新上次执行httpRequest的时间
    httpRequest(reqRes2, i);
  }
}
//显示图片1
void xians_TFT1(){
  tft.setSwapBytes(true);     // RGB->BGR 加上这句才能正确显示颜色。
  //绘制动态的太空人
  clk.createSprite(96, 96);
  clk.setColorDepth(8);
  clk.fillSprite(TFT_BLACK);  // 清空 Sprite
  clk.pushImage(0, 0, 96, 96, tkr[frames]);  // 使用 pushImage() 绘制图像
  clk.pushSprite(135, 110);

  //绘制WiFi是否连接图片
  tft_c.createSprite(25, 25);
  tft_c.setColorDepth(8);
  tft_c.fillSprite(TFT_BLACK);  
  if(WiFi.status() == WL_CONNECTED){tft_c.pushImage(0, 0, 25, 25, wifi);}
  else{tft_c.pushImage(0, 0, 25, 25, wifi_tv);}
  tft_c.pushSprite(0, 0);
  // 调用函数来绘制天气图片
  drawWeatherImage(dailyData[0].code_day, 30, 1);
}
//显示图片2
void xians_TFT2(){
  tft.setSwapBytes(true);     // RGB->BGR 加上这句才能正确显示颜色。
  // 调用函数来绘制天气图片
  drawWeatherImage(futureData[0].code_day, 50, 35);
  drawWeatherImage(futureData[1].code_day, 160, 35);
}
//天气现象图片绘制
void drawWeatherImage(int code, int x, int y) {
  int currentWeather;

  if (code >= 9 and code <= 25) {
    currentWeather = code - 4;
  } else if (code >= 36 and code <= 39) {
    currentWeather = code - 8;
  } else {
    switch (code) {
      case 0: case 2: case 38: currentWeather = 0; break;
      case 1: case 3: currentWeather = 1; break;
      case 4: currentWeather = 2; break;
      case 5: case 7: currentWeather = 3; break;
      case 6: case 8: currentWeather = 4; break;
      case 26: case 27: currentWeather = 22; break;
      case 28: case 29: currentWeather = 23; break;
      case 30: currentWeather = 24; break;
      case 31: currentWeather = 25; break;
      case 32: case 33: currentWeather = 26; break;
      case 34: case 35: currentWeather = 27; break;
      case 99: currentWeather = 31; break;
    }
  }

  tft.pushImage(x, y, 25, 25, tianqi[currentWeather]);
}
//NTP
time_t bao(){
  IPAddress ntpServerIP; // NTP服务器的地址 
  while (retryCount < 100) {   //最大重新连接次数
    while(Udp.parsePacket()>0);  //丢弃所有包
    Serial.println("传输 NTP 请求");
    WiFi.hostByName(ntpServerName, ntpServerIP);  // 从池中获取随机服务器放置在ntpServerName
    Serial.print("NTP地址:");
    Serial.print(ntpServerName);
    Serial.print("-");
    Serial.println(ntpServerIP);
    sendNTP_packet(ntpServerIP);   //向地址处发送请求
    uint32_t beginWait = millis();  //定义一个变量存储请求离开的毫秒数
    while (millis() - beginWait < 1500)// 离开时间减去现在时间
    {
        int size = Udp.parsePacket();  //获取包的大小
        if (size >= 48)   //如果超过了48说明有效
        {
          Serial.println("接收NTP响应成功!");
          isNTPConnected = true;  //表示连接到了服务器
          Udp.read(packetBuffer, 48); // 将数据包读取到缓冲区  // NTP时间在消息的前48个字节里
          unsigned long secsSince1900;    //定义一个无符号长整型变量  存储1900-1-1-0-0 UTC到现在的秒速
          // 将从位置40开始的四个字节转换为长整型，只取前32位整数部分
          secsSince1900 = (unsigned long)packetBuffer[40] << 24;
          secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
          secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
          secsSince1900 |= (unsigned long)packetBuffer[43];
          //Serial.print("自 1900 年以来的秒数:");
          //Serial.println(secsSince1900);
          //Serial.print("从NTP服务器获取的秒数-8时区:");
          //Serial.println(secsSince1900 - 2208988800UL + 8 * SECS_PER_HOUR);
          return secsSince1900 - 2208988800UL + 8 * SECS_PER_HOUR;  //返回现在的时间
        }
    }
    Serial.println("没有NTP服务器响应"); //无NTP响应
    isNTPConnected = false;  //表示没有连接到服务器
    retryCount++;
    delay(1000);  //延迟1秒后重试连接
  }
  Serial.println("无法连接到NTP服务器"); //无法连接到服务器
  return 0;  //无时间
}
// 向给定地址的时间服务器发送NTP请求
void sendNTP_packet(IPAddress &address){
  // 用于标识NTP请求的随机数
  const uint32_t NTP_ID = 0x31314E52; // 49.78.49.52
  memset(packetBuffer, 0, 48);  //设置缓冲区的所有字节为0，并且初始化一个空的包
  packetBuffer[0] = 0b11100011; // 闰秒指示符, 版本号, 模式
  packetBuffer[1] = 0;          // 层次
  packetBuffer[2] = 6;          // 间隔
  packetBuffer[3] = 0xEC;       // 精度
  //根延迟/根离散度
  //写入NTP_ID作为标识符
  *(uint32_t*)(packetBuffer + 12) = NTP_ID;
  Udp.beginPacket(address, 123); //指定目标地址和端口号  NTP需要使用的UDP端口号为123
  Udp.write(packetBuffer, 48);   //写入缓冲区，字节为48
  Udp.endPacket();   //结束发送UDP数据包,并且发送到目标服务器
}
// 向心知天气服务器请求信息并对信息进行解析
void httpRequest(String reqRes, int i){
  WiFiClient client;
  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + host + "\r\n" + 
                              "Connection: close\r\n\r\n";
  Serial.print("连接服务器地址:"); Serial.print(host);
  if (client.connect(host, 80)){ // 尝试连接服务器，如果不成功就没有天气信息
    Serial.println("连接成功!");
    client.print(httpRequest);// 向服务器发送http请求信息
    //Serial.print("请求信息: ");Serial.println(httpRequest); 
    String status_response = client.readStringUntil('\n');// 获取并显示服务器响应状态行 
    //Serial.print("服务器响应状态行: ");
    //Serial.println(status_response);
    if (client.find("\r\n\r\n"));// 使用find跳过HTTP响应头
    if(i==0){parseInfoy(client);}// 利用ArduinoJson库解析心知天气响应信息预测
    if(i==1){parseInfos(client);}// 利用ArduinoJson库解析心知天气响应信息实时
  }
  else {
    Serial.println("连接服务器失败使用备用服务器");
    if(i==0){parseInfoy(clientNULL);}// 利用ArduinoJson库解析心知天气响应信息预测
    if(i==1){parseInfos(clientNULL);}// 利用ArduinoJson库解析心知天气响应信息实时
  }
  client.stop();   //断开客户端与服务器连接工作
}
// 利用ArduinoJson库解析心知天气响应信息(预测)
void parseInfoy(WiFiClient client){
  DynamicJsonDocument doc(1536);
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  Serial.println("重庆预测天气信息:");
  serializeJsonPretty(doc, Serial);
  Serial.println();
  
  JsonObject results_0 = doc["results"][0];
  JsonArray results_0_daily = results_0["daily"];
  JsonObject results_0_daily_0 = results_0_daily[0];  //当天
  JsonObject results_0_daily_1 = results_0_daily[1];  //未来1
  JsonObject results_0_daily_2 = results_0_daily[2];  //未来2
  const char* results_0_last_update = results_0["last_update"]; //天气数据的最后更新时间 
  
  // 保存断网前的最新数据配置(当天)
  dailyData[0].high = results_0_daily_0["high"].as<int>();
  dailyData[0].low = results_0_daily_0["low"].as<int>();
  dailyData[0].rainfall = results_0_daily_0["rainfall"].as<float>();
  dailyData[0].precip = 100*(results_0_daily_0["precip"].as<float>());
  dailyData[0].wind_direction = results_0_daily_0["wind_direction"].as<String>();
  dailyData[0].wind_speed = results_0_daily_0["wind_speed"].as<int>();
  dailyData[0].wind_scale = results_0_daily_0["wind_scale"].as<int>();
  dailyData[0].humidity = results_0_daily_0["humidity"].as<int>();

  // 保存断网前的最新数据配置(未来1)
  futureData[0].code_day = results_0_daily_1["code_day"].as<int>();
  futureData[0].high = results_0_daily_1["high"].as<int>();
  futureData[0].low = results_0_daily_1["low"].as<int>();
  futureData[0].rainfall = results_0_daily_1["rainfall"].as<float>();
  futureData[0].wind_direction = results_0_daily_1["wind_direction"].as<String>();
  futureData[0].wind_speed = results_0_daily_1["wind_speed"].as<int>();
  futureData[0].wind_scale = results_0_daily_1["wind_scale"].as<int>();
  futureData[0].humidity = results_0_daily_1["humidity"].as<int>();

  // 保存断网前的最新数据配置(未来2)
  futureData[1].code_day = results_0_daily_2["code_day"].as<int>();
  futureData[1].high = results_0_daily_2["high"].as<int>();
  futureData[1].low = results_0_daily_2["low"].as<int>();
  futureData[1].rainfall = results_0_daily_2["rainfall"].as<float>();
  futureData[1].wind_direction = results_0_daily_2["wind_direction"].as<String>();
  futureData[1].wind_speed = results_0_daily_2["wind_speed"].as<int>();
  futureData[1].wind_scale = results_0_daily_2["wind_scale"].as<int>();
  futureData[1].humidity = results_0_daily_2["humidity"].as<int>();

  //串口回复数据更新时间
  Serial.print("重庆预测天气数据更新时间:");
  Serial.println(results_0_last_update);
}
// 利用ArduinoJson库解析心知天气响应信息(实时)
void parseInfos(WiFiClient client){
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  Serial.println("重庆预测天气信息:");
  serializeJsonPretty(doc, Serial);
  Serial.println();

  JsonObject results_0 = doc["results"][0];
  const char* results_0_last_updates = results_0["last_update"]; // "2023-08-02T09:54:20+08:00"
  JsonObject results_0_now = results_0["now"];
  dailyData[0].code_day = results_0_now["code"].as<int>();
  dailyData[0].temperature = results_0_now["temperature"].as<int>();

  //串口回复数据更新时间
  Serial.print("重庆实时天气数据更新时间:");
  Serial.println(results_0_last_updates);
}