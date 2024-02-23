#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "Haneashi-AP";
const char *password = "123456789";
const IPAddress ip(192, 168, 0, 100);
const IPAddress subnet(255,255,255,0);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const int LED_PIN = 33;
const int BUTTON_PIN = 35;
// const int LED_PIN = 32;
// const int BUTTON_PIN = 34;

unsigned long gLightOnTime = 0;
unsigned long gLightOffTime = 0;
bool gExistNewRecord = false;


void IRAM_ATTR buttonPushed() {

  // LED消灯
  digitalWrite(LED_PIN, LOW);

  // 割り込みを無効化
  detachInterrupt(BUTTON_PIN);

  // ボタンが押された時の処理
  gLightOffTime = millis();
  gExistNewRecord = true;
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, (char *)data);
    String command = doc["command"].as<String>();

    if(command == "wson") {
      // 計測開始

      // LED on 
      digitalWrite(LED_PIN, HIGH);

      // 現在時刻記録
      gLightOnTime = millis();  // 現在時刻記録

      // 割り込み有効化
      attachInterrupt(BUTTON_PIN, buttonPushed, FALLING);
    }
    if(command == "wsoff") {
      digitalWrite(LED_PIN, LOW);
      // 計測リセット
    }

  }
}


void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  switch( type ) {
    case WS_EVT_CONNECT:
      printf("ws[%s][%u] connect\n", server->url(), client->id());
      break;
    
    case WS_EVT_DISCONNECT:
      printf("ws[%s][%u] disconnect\n", server->url(), client->id());
      break;
    
    case WS_EVT_ERROR:
      printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
      break;
    
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    
    default:
      printf("ws[%s][%u] default(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
      break;
  }
}

void setup() {
  printf("setup funtion\n");

  if( !SPIFFS.begin(true) ) {
    printf("SPIFFS Mount Failed\n");
    return;
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  delay(100);
  WiFi.softAPConfig(ip, ip, subnet);

  // LED_PIN を出力に設定
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // ボタンピンを入力に設定
  // pinMode(BUTTON_PIN, INPUT_PULLUP);  // GPIO35 は入力専用ピンのため、プルアップ抵抗は外付けのためコメントアウト
  // attachInterrupt(BUTTON_PIN, buttonPushed, FALLING);

  ws.onEvent( onWsEvent );
  server.addHandler( &ws );
  server.onNotFound([](AsyncWebServerRequest *request){
    printf("NOT_FOUND: ");
    request->send(404);
  });
  server.on(
    "/", HTTP_GET,
    [](AsyncWebServerRequest *request) {
      request->send( SPIFFS, "/index.html" );
    }
  );
  server.begin();
}

void loop() {
  ws.cleanupClients();

  if( gExistNewRecord ) {
    gExistNewRecord = false;
    unsigned long interval = gLightOffTime - gLightOnTime;
    
    // JsonDocument doc;
    // static char json[200];
    // doc["interval"] = interval;
    // serializeJson(doc, json);

    // ws.textAll(json);

    printf("interval: %lu\n", interval);
    DynamicJsonDocument doc(1024);
    static char json[200];
    doc["interval"] = interval;
    serializeJson(doc, json);
    ws.textAll(json);
  }
}

