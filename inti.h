#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>                  // HTTP通信库需要使用
#include <XPT2046_Touchscreen.h>
#include "TouchControllerWS.h"
#include <MiniGrafx.h>
#include <ILI9341_SPI.h>
#include "ArialRounded.h"
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>