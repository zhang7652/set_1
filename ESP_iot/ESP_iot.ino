//手机互联
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

// const int D0 = 16;  // D3对应的引脚编号是16
const int D1 = 5;  // D4对应的引脚编号是5
const int D2 = 4;  // D2对应的引脚编号是4
// const int D5 = 14;  // D5对应的引脚编号是14
// const int D6 = 12;  // D6对应的引脚编号是12
// const int D7 = 13;  // D7对应的引脚编号是13
// const int D8 = 15;  // D8对应的引脚编号是15
// //也可以使用，但是不推荐
// const int D3 = 0;  // D3对应的引脚编号是0   固件烧录
// const int D4 = 2;  // D4对应的引脚编号是2   自代灯

const char *host = "192.168.208.58";  //修改为手机的的tcpServer服务端的IP地址，即手机在路由器上的ip
const int tcp_Port = 8080;            //修改为你建立的Server服务端的端口号
WiFiClient client;
// 建立WiFi对象
ESP8266WiFiMulti wifiMulti;
void setup() {
  Serial.begin(115200);
  pinMode(D1, OUTPUT);
  delay(100);
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
  if (wifiMulti.run() == WL_CONNECTED) {
    if (!client.connected()) {
      if (!client.connect(host, tcp_Port)) {
        delay(100);
        Serial.println("连接->>>...\n");
        return;
      }
    }
    while (client.available()) {
      char val = client.read();  //读取一行数据直到遇到换行符
      switch(val){
        case '1':
        digitalWrite(D1, LOW);
        digitalWrite(D2, HIGH);
        break;
        case '2':
        digitalWrite(D1, HIGH);
        digitalWrite(D2, LOW);
        break;
        case 'F':
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        break;
        case 'N':
        digitalWrite(D1, HIGH);
        digitalWrite(D2, HIGH);
        break;
      }

      Serial.println(val);
    }
  }
}


