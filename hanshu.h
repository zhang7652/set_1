//函数配置调用
void dacit();
void getip();
void WIFI_I();
void WIFI_hz();
void testText_1();
void stept();
void loopst();
void parseInfoy(WiFiClient client);
void parseInfos(WiFiClient client);
void httpRequest(String reqRes, int i);
const uint16_t* image_draw(int i);
void tftprint(int x,int y,int da, String text);
const uint16_t* image_senxiao(int i);

//获取当前城市ip地址
void getip(){  
  /* 1. 新建一个WiFiClient和HTTPClient对象 */
  WiFiClient tcpClient;
  HTTPClient httpClient;
  /* 2. 通过begin函数配置请求地址，第一个参数需填入WiFiClient对象 */
  httpClient.begin(tcpClient, URL); 
  Serial.print("URL: "); Serial.println(URL);

  /* 3. 通过GET函数启动连接并发送HTTP GET请求 */
  int httpCode = httpClient.GET();                // 发送GET请求并返回获得的状态码
  Serial.println("Send GET request to URL: " + String(URL));

  /* 4. 如果服务器响应HTTP_CODE_OK(200)则从服务器获取响应体信息并通过串口输出 */
  if (httpCode == HTTP_CODE_OK){
    Payload = httpClient.getString();      // 使用getString函数获取服务器响应体内容
    Serial.println("\r\nServer Respose Code: " + httpCode);
    Serial.println("Server Response Payload: " + Payload);
  }else{Serial.print("\r\nServer Respose Code: " + httpCode);}
  /* 5. 关闭ESP8266与服务器的连接 */
  httpClient.end();
}
//wifi
void WIFI_I(){
  /* 通过addAP函数添加需要存储的WiFi名称和密码 */
  wifiMulti.addAP("ChinaNet-fkUu", "ipew2yu5");
  wifiMulti.addAP("iQOO Z3", "123456789");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 10);
  int n = WiFi.scanNetworks();
  if (n == 0) {
    tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
    tft.print("no networks found");
  } else {
    for (int i = 0; i < n; ++i) {
      tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
      tft.print(i+1);
      tft.print(": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") ");
      tft.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }

  delay(10000);
  while (wifiMulti.run() != WL_CONNECTED)       // 通过wifiMulti.run()，NodeMCU将会在当前环境中搜索addAP函数所存储的WiFi。
  {                                             // 如果搜到多个存储的WiFi那么NodeMCU将会连接信号最强的那一个WiFi信号。
    delay(100);                                 // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。   
  }
  WiFi.setAutoReconnect(true);  //ESP8266会在连接断开时自动重新连接
  WiFi.persistent(true);//自动重新连接到以前连接的接入点

  getip();
  tft.fillScreen(ILI9341_BLACK);
}
//wifi信号强度
void WIFI_hz(){
  int32_t dbm = WiFi.RSSI();
  int quality = 0;
  if (dbm <= -100) {
    quality = 0;
  } else if (dbm >= -50) {
    quality = 100;
  } else {
    quality = 2 * (dbm + 100);
  }
  for (int8_t i = 0; i < 4; i++) {
    for (int8_t j = 0; j < 2 * (i + 1); j++) {
      if (quality > i * 25 || j == 0) {
        tft.drawPixel(230 + 2 * i, 6 - j, ILI9341_WHITE);
      }
    }
  }
}
//页面一
void testText_1() {
  //显示文字
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tftprint(30, 50, 4, timeClient.getFormattedTime());
  // 在TFT屏幕上显示时间
  tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  tftprint(200, 30, 2, ((timeClient.getEpochTime() % 86400L) / 3600 < 12) ? "AM" : "PM");
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tftprint(60, 10, 2, String(dailyData[0].date));

  tftprint(155, 165, 1.5, String(dailyData[0].high));
  tft.drawCircle(168, 165, 1, ILI9341_WHITE);
  tftprint(170, 165, 1.5,"C-");
  tftprint(181, 165, 1.5, String(dailyData[0].low));
  tft.drawCircle(188, 165, 1, ILI9341_WHITE);
  tftprint(190, 165, 1.5,"C");
  if (dailyData[0].temperature < 20) {
    tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
  } else if (dailyData[0].temperature > 33) {
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  } 
    //画空心圆
  if(dailyData[0].temperature>=10){
    tft.drawCircle(200, 138, 7, ILI9341_LIGHTGREY);
    tft.drawCircle(200, 138, 8, ILI9341_LIGHTGREY);
    tftprint(210, 130, 4, "C");
  }else{
    tft.drawCircle(180, 138, 7, ILI9341_LIGHTGREY);
    tft.drawCircle(180, 138, 8, ILI9341_LIGHTGREY);
    tftprint(190, 130, 4, "C");
  }
  tftprint(145, 130, 4, String(dailyData[0].temperature));
  tftprint(130, 200, 2, name);
  
  //绘制wifi强度信号
  WIFI_hz();
  // 在TFT屏幕上绘制位图
  tft.drawRGBBitmap(30,100,image_draw(dailyData[0].code_day),DRAGON_WIDTH, DRAGON_WIDTH);
  tft.drawRGBBitmap(20,230,image_senxiao((dailyData[0].date.substring(0, 4).toInt() - 4) % 12),DRAGON_heig, DRAGON_heig);
  button1.press(button1.contains(x, y)); // 判断触摸点是否在按钮区域内，并设置按钮状态
  button2.press(button2.contains(x, y)); // 判断触摸点是否在按钮区域内，并设置按钮状态
  // 检查按钮状态是否发生变化
  if (button1.justReleased()) {
    button1.drawButton(true); // 绘制按下状态的按钮
    tft.fillScreen(ILI9341_BLACK); // 清空屏幕
    // 绘制按钮
    button3.drawButton();
    tetx = 1;
    return;
  }
  // 检查按钮状态是否发生变化
  if (button2.justReleased()) {
    button2.drawButton(true); // 绘制按下状态的按钮
    tft.fillScreen(ILI9341_BLACK); // 清空屏幕
    // 绘制按钮
    button2.drawButton();
    tetx = 2;
    return;
  }
}
void tftprint(int x,int y,int da, String text){
  tft.setTextSize(da);
  tft.setCursor(x, y);
  tft.println(text);
}
//页面2
void testText_2() {
  button3.press(button3.contains(x, y)); // 判断触摸点是否在按钮区域内，并设置按钮状态
  // 检查按钮状态是否发生变化
  if (button3.justReleased()) {
    button3.drawButton(true); // 绘制按下状态的按钮
    tft.fillScreen(ILI9341_BLACK); // 清空屏幕
    // 绘制按钮
    button1.drawButton();
    button2.drawButton();
    tetx = 0;
    return;
  }
  tft.setCursor(90, 10);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  tft.setTextSize(3);
  tft.print("Set up");
  tft.setCursor(0, 50);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  tft.setTextSize(2);
  tft.println("Date: " + String(dailyData[0].date));
  tft.println("WIFI:" + String(WiFi.SSID()));
  tft.println("Password:" + String(WiFi.psk()));
  IPAddress localIP = WiFi.localIP();
  String IP = localIP.toString();
  tft.println("IP:" + IP);
  tft.println("IP:" + Payload);
  tft.println("time:" + timeClient.getFormattedTime());
}
//页面3
void testText_3() {
  button2.press(button2.contains(x, y)); // 判断触摸点是否在按钮区域内，并设置按钮状态
  // 检查按钮状态是否发生变化
  if (button2.justReleased()) {
    button2.drawButton(true); // 绘制按下状态的按钮
    tft.fillScreen(ILI9341_BLACK); // 清空屏幕
    // 绘制按钮
    button1.drawButton();
    button2.drawButton();
    tetx = 0;
    return;
  }
}
//开始运行
void stept(){
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
  // 初始化按钮，传入TFT屏幕对象、按钮的位置和大小、颜色等参数
  button1.initButton(&tft, 20, 30, 40, 30, ILI9341_BLACK, ILI9341_BLACK, ILI9341_GREENYELLOW, "Set up", 1);
  button2.initButton(&tft, 235, 170, 10, 50, ILI9341_BLACK, ILI9341_BLACK, ILI9341_GREENYELLOW, ">", 1);
  button3.initButton(&tft, 20, 30, 40, 30, ILI9341_BLACK, ILI9341_BLACK, ILI9341_GREENYELLOW, "return", 1);
  // 设置触摸屏的校准参数
  touch.setCalibration(209, 1759, 1775, 273);
  //连接网络
  WIFI_I();
  //获取时间
  timeClient.begin();
  dacit();
  // 绘制按钮
  button1.drawButton();
  button2.drawButton();
  time_1 = millis();
}
//重复运行
void loopst(){
  if (millis() - time_1 >= 600000) { // 检测是否已经过去了10分钟
    Serial.println("1");
    //获取城市IP
    getip();
    //获取天气信息
    dacit();
    time_1 = millis(); // 更新上次执行的时间
  }
  if (touch.isTouching()) { // 检测是否有触摸事件发生
    touch.getPosition(x, y);
    y = tft.height() - y; // 翻转y轴坐标
    x = tft.width() - x; //反转x轴坐标
    // Serial.print("x ="); Serial.print(x); Serial.print(" y ="); Serial.println(y);
    prev_x = x; // 更新前一次触摸点的坐标
    prev_y = y;
    Serial.print(prev_x);
    Serial.print(" ");
    Serial.println(prev_y);
  } else {
    prev_x = prev_y = 0xffff; // 如果没有触摸事件发生，将前一次触摸点的坐标重置为无效值
  }
  
  if (tetx == 0 ){
    testText_1();
  }else if(tetx == 1){
    testText_2();
  }else if(tetx == 2){
    testText_3();
  }
  //更新显示
  timeClient.update();
  // Serial.println(timeClient.getFormattedTime());
  if (timeClient.getFormattedTime() != lastTime) {
    lastTime = timeClient.getFormattedTime();
  }
}
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
  JsonObject results = results_0["location"];
  JsonArray results_0_daily = results_0["daily"];
  JsonObject results_0_daily_0 = results_0_daily[0];             //当天
  JsonObject results_0_daily_1 = results_0_daily[1];             //未来1
  JsonObject results_0_daily_2 = results_0_daily[2];             //未来2
  const char *results_0_last_update = results_0["last_update"];  //天气数据的最后更新时间
  name = results["name"].as<String>();
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