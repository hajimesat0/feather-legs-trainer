#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
// #include <SPIFFS.h>
// #include <ESPAsyncWebServer.h>
#include "CReactionTimer.h"

const char *ssid = "Haneashi-AP";
const char *password = "123456789";
const IPAddress ip(192, 168, 0, 100);
const IPAddress subnet(255,255,255,0);

static CReactionTimer *ReactionTimer;

// AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");

const int LED_PIN_LEFT = 33;
const int BUTTON_PIN_LEFT = 35;
const int LED_PIN_RIGHT = 32;
const int BUTTON_PIN_RIGHT = 34;

unsigned long gLightOnTime = 0;
unsigned long gLightOffTime = 0;
bool gExistNewRecord = false;
bool gIsLightOnLeft = false;
bool gIsLightOffLeft = false;
bool gIsLightOnRight = false;
bool gIsLightOffRight = false;


void IRAM_ATTR buttonPushedLeft() {

  gIsLightOffLeft = true;

  // LED消灯
  digitalWrite(LED_PIN_LEFT, LOW);

  // 割り込みを無効化
  detachInterrupt(BUTTON_PIN_LEFT);

  // ボタンが押された時の処理
  gLightOffTime = millis();
  gExistNewRecord = true;
}


void IRAM_ATTR buttonPushedRight() {

  gIsLightOffRight = true;

  // LED消灯
  digitalWrite(LED_PIN_RIGHT, LOW);

  // 割り込みを無効化
  detachInterrupt(BUTTON_PIN_RIGHT);

  // ボタンが押された時の処理
  gLightOffTime = millis();
  gExistNewRecord = true;
}


// void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
//   AwsFrameInfo *info = (AwsFrameInfo *)arg;
//   if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
//   {
//     data[len] = 0;
//     DynamicJsonDocument doc(1024);
//     deserializeJson(doc, (char *)data);
//     String command = doc["command"].as<String>();

//     if(command == "bt_left") {
//       // 左側ボタン計測開始
//       gIsLightOnLeft = true;

//       // LED on 
//       digitalWrite(LED_PIN_LEFT, HIGH);

//       // 現在時刻記録
//       gLightOnTime = millis();  // 現在時刻記録

//       // 割り込み有効化
//       attachInterrupt(BUTTON_PIN_LEFT, buttonPushedLeft, FALLING);
//     }
//     if(command == "bt_right") {
//       // 右側ボタン計測開始
//       gIsLightOnRight = true;

//       // LED on 
//       digitalWrite(LED_PIN_RIGHT, HIGH);

//       // 現在時刻記録
//       gLightOnTime = millis();  // 現在時刻記録

//       // 割り込み有効化
//       attachInterrupt(BUTTON_PIN_RIGHT, buttonPushedRight, FALLING);
//     }

//   }
// }


// void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
//   switch( type ) {
//     case WS_EVT_CONNECT:
//       printf("ws[%s][%u] connect\n", server->url(), client->id());
//       break;
    
//     case WS_EVT_DISCONNECT:
//       printf("ws[%s][%u] disconnect\n", server->url(), client->id());
//       break;
    
//     case WS_EVT_ERROR:
//       printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
//       break;
    
//     case WS_EVT_DATA:
//       handleWebSocketMessage(arg, data, len);
//       break;
    
//     default:
//       printf("ws[%s][%u] default(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
//       break;
//   }
// }

void setup() {
  printf("setup funtion\n");

  // if( !SPIFFS.begin(true) ) {
  //   printf("SPIFFS Mount Failed\n");
  //   return;
  // }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  delay(100);
  WiFi.softAPConfig(ip, ip, subnet);

  // // LED_PIN を出力に設定
  // pinMode(LED_PIN_LEFT, OUTPUT);
  // digitalWrite(LED_PIN_LEFT, LOW);
  // pinMode(LED_PIN_RIGHT, OUTPUT);
  // digitalWrite(LED_PIN_RIGHT, LOW);

  ReactionTimer = new CReactionTimer(2);
  ReactionTimer->Setup();

  // ボタンピンを入力に設定
  // pinMode(BUTTON_PIN, INPUT_PULLUP);  // GPIO35 は入力専用ピンのため、プルアップ抵抗は外付けのためコメントアウト
  // attachInterrupt(BUTTON_PIN, buttonPushed, FALLING);

}

void loop() {
  ReactionTimer->Loop();
  // ws.cleanupClients();

  // if( gExistNewRecord ) {
  //   gExistNewRecord = false;
  //   unsigned long interval = gLightOffTime - gLightOnTime;
    
    // JsonDocument doc;
    // static char json[200];
    // doc["interval"] = interval;
    // serializeJson(doc, json);

    // ws.textAll(json);

  //   printf("interval: %lu\n", interval);
  //   DynamicJsonDocument doc(1024);
  //   static char json[200];
  //   doc["interval"] = interval;
  //   serializeJson(doc, json);
  //   ws.textAll(json);
  // }

  // if( gIsLightOnLeft ) {
  //   // printf("gIsLightOnLeft\n");
  //   gIsLightOnLeft = false;
  //   DynamicJsonDocument doc(1024);
  //   static char json[200];
  //   doc["msg_type"] = 1;
  //   doc["button_light_index"] = 0;
  //   doc["button_light_on_off"] = 1;
  //   serializeJson(doc, json);
  //   // printf("json:%s\n",json);
  //   ws.textAll(json);
  // }

  // if( gIsLightOffLeft ) {
  //   gIsLightOffLeft = false;
  //   DynamicJsonDocument doc(1024);
  //   static char json[200];
  //   doc["msg_type"] = 1;
  //   doc["button_light_index"] = 0;
  //   doc["button_light_on_off"] = 0;
  //   serializeJson(doc, json);
  //   ws.textAll(json);
  // }

  // if( gIsLightOnRight ) {
  //   // printf("gIsLightOnRight\n");
  //   gIsLightOnRight = false;
  //   DynamicJsonDocument doc(1024);
  //   static char json[200];
  //   doc["msg_type"] = 1;
  //   doc["button_light_index"] = 1;
  //   doc["button_light_on_off"] = 1;
  //   serializeJson(doc, json);
  //   // printf("json:%s\n",json);
  //   ws.textAll(json);
  // }

  // if( gIsLightOffRight ) {
  //   gIsLightOffRight = false;
  //   DynamicJsonDocument doc(1024);
  //   static char json[200];
  //   doc["msg_type"] = 1;
  //   doc["button_light_index"] = 1;
  //   doc["button_light_on_off"] = 0;
  //   serializeJson(doc, json);
  //   ws.textAll(json);
  // }

}

