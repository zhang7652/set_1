#include "core_esp8266_features.h"
// 利用ArduinoJson库解析心知天气响应信息(预测)
void parseInfoy(WiFiClient client) {
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
  JsonObject results_0_daily_0 = results_0_daily[0];             //当天
  JsonObject results_0_daily_1 = results_0_daily[1];             //未来1
  JsonObject results_0_daily_2 = results_0_daily[2];             //未来2
  const char *results_0_last_update = results_0["last_update"];  //天气数据的最后更新时间
  // 保存断网前的最新数据配置(当天)
  dailyData[0].date = results_0_daily_0["date"].as<String>();
  dailyData[0].high = results_0_daily_0["high"].as<int>();
  dailyData[0].low = results_0_daily_0["low"].as<int>();
  dailyData[0].rainfall = results_0_daily_0["rainfall"].as<float>();
  dailyData[0].precip = 100 * (results_0_daily_0["precip"].as<float>());
  dailyData[0].wind_direction = results_0_daily_0["wind_direction"].as<String>();
  dailyData[0].wind_speed = results_0_daily_0["wind_speed"].as<int>();
  dailyData[0].wind_scale = results_0_daily_0["wind_scale"].as<int>();
  dailyData[0].humidity = results_0_daily_0["humidity"].as<int>();
  // 保存断网前的最新数据配置(未来1)
  futureData[0].date = results_0_daily_1["date"].as<String>();
  futureData[0].code_day = results_0_daily_1["code_day"].as<int>();
  futureData[0].high = results_0_daily_1["high"].as<int>();
  futureData[0].low = results_0_daily_1["low"].as<int>();
  futureData[0].rainfall = results_0_daily_1["rainfall"].as<float>();
  futureData[0].wind_direction = results_0_daily_1["wind_direction"].as<String>();
  futureData[0].wind_speed = results_0_daily_1["wind_speed"].as<int>();
  futureData[0].wind_scale = results_0_daily_1["wind_scale"].as<int>();
  futureData[0].humidity = results_0_daily_1["humidity"].as<int>();
  // 保存断网前的最新数据配置(未来2)
  futureData[1].date = results_0_daily_2["date"].as<String>();
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
void parseInfos(WiFiClient client) {
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  Serial.println("重庆实时天气信息:");
  serializeJsonPretty(doc, Serial);
  Serial.println();
  JsonObject results_0 = doc["results"][0];
  const char *results_0_last_updates = results_0["last_update"];  // "2023-08-02T09:54:20+08:00"
  JsonObject results_0_now = results_0["now"];
  dailyData[0].code_day = results_0_now["code"].as<int>();
  dailyData[0].temperature = results_0_now["temperature"].as<int>();
  Serial.println(dailyData[0].code_day);
  Serial.println(dailyData[0].temperature);
  //串口回复数据更新时间
  Serial.print("重庆实时天气数据更新时间:");
  Serial.println(results_0_last_updates);
}
// 向心知天气服务器请求信息并对信息进行解析
void httpRequest(String reqRes, int i) {
  WiFiClient client;
  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
  Serial.print("连接服务器地址:");
  Serial.print(host);
  if (client.connect(host, 80)) {  // 尝试连接服务器，如果不成功就没有天气信息
    Serial.println("连接成功!");
    client.print(httpRequest);  // 向服务器发送http请求信息
    Serial.print("请求信息: ");Serial.println(httpRequest);
    String status_response = client.readStringUntil('\n');  // 获取并显示服务器响应状态行
    //Serial.print("服务器响应状态行: ");
    //Serial.println(status_response);
    if (client.find("\r\n\r\n"))
      ;                                  // 使用find跳过HTTP响应头
    if (i == 0) { parseInfoy(client); }  // 利用ArduinoJson库解析心知天气响应信息预测
    if (i == 1) { parseInfos(client); }  // 利用ArduinoJson库解析心知天气响应信息实时
  } else {
    Serial.println("连接服务器失败使用备用服务器");
    if (i == 0) { parseInfoy(clientNULL); }  // 利用ArduinoJson库解析心知天气响应信息预测
    if (i == 1) { parseInfos(clientNULL); }  // 利用ArduinoJson库解析心知天气响应信息实时
  }
  client.stop();  //断开客户端与服务器连接工作
}
//直接请求
void dacit(){
  reqRes1 = "/v3/weather/daily.json?key=" + reqUserKey +
         + "&location=" + Payload + "&language=en&unit=c&start=0&days=3";
  reqRes2 = "/v3/weather/now.json?key=" + reqUserKey +
         + "&location=" + Payload + "&language=en&unit=c";
  httpRequest(reqRes1, 0);
  httpRequest(reqRes2, 1);
}
//获取当前城市ip地址
void getip138(){  
  /* 1. 新建一个WiFiClient和HTTPClient对象 */
  WiFiClient tcpClient;
  HTTPClient httpClient;
  /* 2. 通过begin函数配置请求地址，第一个参数需填入WiFiClient对象 */
  httpClient.begin(tcpClient, URL); 
  Serial.print("URL: "); Serial.println(URL);

  /* 3. 通过GET函数启动连接并发送HTTP GET请求 */
  int httpCode = httpClient.GET();                // 发送GET请求并返回获得的状态码
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);

  
  /* 4. 如果服务器响应HTTP_CODE_OK(200)则从服务器获取响应体信息并通过串口输出
        如果服务器不响应HTTP_CODE_OK(200)则将服务器响应状态码通过串口输出 */
  if (httpCode == HTTP_CODE_OK) 
  {
    Payload = httpClient.getString();      // 使用getString函数获取服务器响应体内容
    Serial.print("\r\nServer Respose Code: ");
    Serial.println(httpCode);
    Serial.println("Server Response Payload: ");
    Serial.println(Payload);
  } 
  else 
  {
    Serial.print("\r\nServer Respose Code: ");
    Serial.println(httpCode);
  }
 
  /* 5. 关闭ESP8266与服务器的连接 */
  httpClient.end();
}
//天气现象图片绘制
void drawWeatherImage(int code, int x, int y) {
  int currentWeather;
  switch (code) {
    case 0:case 1:case 2: case 3:currentWeather = 0;break;
    case 4:case 5:case 6:case 7:case 8:case 9:currentWeather = 1;break;
    case 10:case 13:case 14:case 15:case 16:case 17:case 18:case 19:currentWeather = 2;break;
    case 11:case 12:currentWeather = 3;break;
    case 20: case 21:case 22:case 23:case 24:case 25:currentWeather = 4;break;
    case 30:case 31:currentWeather = 5;break;
  }
  gfx.drawPalettedBitmapFromPgm(x, y, getMeteoconIconFromProgmem(currentWeather));
}
//界面1
void drawProgress_1() {
  // 显示图片
  gfx.fillBuffer(MINI_BLACK);
  drawWeatherImage(dailyData[0].code_day, 100, 100);
  // 显示字符
  gfx.setFont(ArialRoundedMTBold_36);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(MINI_WHITE);
  gfx.drawString(120, 50, timeClient.getFormattedTime());
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.setColor(MINI_WHITE);
  gfx.drawString(10,240, "Date:" + String(dailyData[0].date));
  gfx.drawString(10, 260, "temperature:" + String(dailyData[0].temperature) + "°C");
  gfx.drawString(10, 280, "rainfall:" + String(dailyData[0].rainfall));
  gfx.drawString(10, 300, "humidity:" + String(dailyData[0].humidity));
  // gfx.drawString(10, 10, "wind_direction:" + String(dailyData[0].wind_direction));
  // gfx.drawString(10, 10, "wind_speed:" + String(dailyData[0].wind_speed) + " m/s");
  // gfx.drawString(10, 10, "wind_scale:" + String(dailyData[0].wind_scale) + " rank");
}
//界面2
void drawProgress_2() {
  // 显示字符
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setColor(MINI_WHITE);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.drawString(110,10, "Weather for the next 3 days");
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.drawString(10,30, "Date:" + String(dailyData[0].date));
  gfx.drawString(10, 50, "low:" + String(dailyData[0].low) + "°C");
  gfx.drawString(80, 50, "high:" + String(dailyData[0].high) + "°C");
  gfx.drawString(10, 70, "rainfall:" + String(dailyData[0].rainfall));
  gfx.drawString(10, 90, "humidity:" + String(dailyData[0].humidity));
  drawWeatherImage(dailyData[0].code_day, 150, 30);

  gfx.setColor(MINI_WHITE);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.drawString(10,120, "Date:" + String(futureData[0].date));
  gfx.drawString(10, 140, "low:" + String(futureData[0].low) + "°C");
  gfx.drawString(80, 140, "high:" + String(futureData[0].high) + "°C");
  gfx.drawString(10, 160, "rainfall:" + String(futureData[0].rainfall));
  gfx.drawString(10, 180, "humidity:" + String(futureData[0].humidity));
  drawWeatherImage(futureData[0].code_day, 150, 120);

  gfx.setColor(MINI_WHITE);
  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.drawString(10,220, "Date:" + String(futureData[1].date));
  gfx.drawString(10, 240, "low:" + String(futureData[1].low) + "°C");
  gfx.drawString(80, 240, "high:" + String(futureData[1].high) + "°C");
  gfx.drawString(10, 260, "rainfall:" + String(futureData[1].rainfall));
  gfx.drawString(10, 280, "humidity:" + String(futureData[1].humidity));
  drawWeatherImage(futureData[1].code_day, 150, 220);
}
//界面3
void drawProgress_3() {
  // 显示字符
  gfx.setFont(ArialRoundedMTBold_14);
  gfx.setTextAlignment(TEXT_ALIGN_CENTER);
  gfx.setColor(MINI_WHITE);
  gfx.drawString(120, 10, "System configuration");

  gfx.setTextAlignment(TEXT_ALIGN_LEFT);
  gfx.setColor(MINI_WHITE);
  gfx.drawString(10, 40, "WIFI:" + String(ssid));
  gfx.drawString(10, 60, "Password:" + String(password));
  // Serial.println(WiFi.localIP());
  IPAddress localIP = WiFi.localIP();
  String IP = localIP.toString();
  gfx.drawString(10, 80, "IP:" + IP);
  gfx.drawString(10, 100, "IP:" + Payload);
  gfx.drawString(10, 120, "time:" + timeClient.getFormattedTime());
}
//绘制
void calibrationCallback(int16_t x, int16_t y) {
  gfx.setColor(1);
  gfx.fillCircle(x, y, 10);
}
//循环
void init_loop(){
  if (millis() - time_1 >= 600000) { // 检测是否已经过去了10分钟
    Serial.println("1");
    //获取城市IP
    getip138();
    //获取天气信息
    dacit();
    time_1 = millis(); // 更新上次执行的时间
  }
  gfx.fillBuffer(MINI_BLACK);  // 清空缓冲区

  //检测触摸屏是否被触摸
  if (touchController.isTouched(0)) {  // 检测是否有触摸事件发生
    TS_Point p = touchController.getPoint();  // 获取触摸点坐标
    Serial.printf("%d        %d\n", p.x, p.y);  // 打印触摸点坐标
    if (p.y < 80){
      i = i + 1;
      if(i > 2){
        i = 0;
      }
    };  // 判断触摸点是否在指定范围内
  }
  
  //更新显示
  timeClient.update();
  // Serial.println(timeClient.getFormattedTime());
  if (timeClient.getFormattedTime() != lastTime) {
    lastTime = timeClient.getFormattedTime();
  }
  if(i == 0){
    drawProgress_1();
  }else if(i == 1){
    drawProgress_2();
  }else if(i == 2){
    drawProgress_3();
  }
  gfx.commit();
}
//配置信息
void init_main(){
  Serial.begin(115200);  // 初始化串口通信，波特率为115200
  Serial.println(TFT_LED);  // 打印TFT_LED的值到串口监视器
  pinMode(TFT_LED, OUTPUT);  // 设置TFT_LED引脚为输出模式
  digitalWrite(TFT_LED, HIGH);    // 将TFT_LED引脚设置为高电平（打开）
  gfx.init();  // 初始化图形库
  gfx.fillBuffer(MINI_BLACK);  // 用黑色填充缓冲区
  gfx.commit();  // 提交缓冲区的内容到显示设备
  ts.begin();  // 初始化触摸屏控制器
  boolean isCalibrationAvailable = touchController.loadCalibration();  // 检查触摸校准数据是否可用
  if (!isCalibrationAvailable) {
    touchController.startCalibration(&calibration);  // 开始触摸屏幕校准
    while (!touchController.isCalibrationFinished()) {
      touchController.continueCalibration();  // 继续触摸屏幕校准
      gfx.commit();  // 提交缓冲区的内容到显示设备
      yield();  // 暂停一段时间，让其他任务有机会运行
    }
    touchController.saveCalibration();  // 保存触摸校准数据
  }
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  //获取城市IP
  getip138();
  //获取天气信息
  dacit();
  time_1 = millis();
  timeClient.begin();
}