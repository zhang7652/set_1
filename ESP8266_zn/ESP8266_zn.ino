//手机互联2
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

const int D1 = 5;  // D4对应的引脚编号是5
const int D2 = 4;  // D2对应的引脚编号是4

const char *host = "192.168.208.58";  //修改为手机的的tcpServer服务端的IP地址，即手机在路由器上的ip
const int tcp_Port = 8080;            //修改为你建立的Server服务端的端口号
WiFiClient client;
int i1 = 0;
int i2 = 0;
int i3 = 0;
// 建立WiFi对象
ESP8266WiFiMulti wifiMulti;
void setup() {
  Serial.begin(115200);
  pinMode(D1, OUTPUT); 
  pinMode(D2, OUTPUT);
  WiFi.mode(WIFI_AP_STA);
  //使用AP添加WiFi
  wifiMulti.addAP("遇到爱了吗？", "66612345");
  // 连接WiFi
  while (wifiMulti.run() != WL_CONNECTED)
    ;
  Serial.println("连接成功");
}
void loop() {
  String message = " ";
  if (wifiMulti.run() == WL_CONNECTED) {
    if (!client.connected()) {
      if (!client.connect(host, tcp_Port)) {
        delay(100);
        Serial.println("connect to \n");
        return;
      }
      message = "connect cept";
      client.write(message.c_str(), message.length());//向服户端发送消息
    }
    while (client.available()) {
      char val = client.read();  //读取一行数据直到遇到换行符
      message = " ";
      switch (val) {
        case '1':
          if (i1 == 0){
            digitalWrite(D1, LOW);
            // 向连接的端口发送信息
            message = "OFF-1          ";
            i1 = 1;
          }
          else {
            digitalWrite(D1, HIGH);
            message = "ON-1          ";
            i1=0;
          }
          break;
      case '2':
        if (i2 == 0){
            digitalWrite(D2, LOW);
            message = "OFF-2          ";
            i2 = 1;
          }
          else {
            digitalWrite(D2, HIGH);
            message = "ON-2          ";
            i2=0;
          }
          break;
      case '0':
        if (i3 == 0){
            digitalWrite(D1, LOW);
            digitalWrite(D2, LOW);
            message = "OFF-ALL          ";
            i3 = 1;
          }
          else {
            digitalWrite(D1, HIGH);
            digitalWrite(D2, HIGH);
            message = "ON-ALL          ";
            i3=0;
          }
          break;
      }
      client.write(message.c_str(), message.length());//向服户端发送消息
      Serial.println(val); //串口打印
    }
  }
  else {
    Serial.println("网络连接失败！");
    while (wifiMulti.run() != WL_CONNECTED)
      ;
  }
}
