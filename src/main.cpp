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

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, (char *)data);
    String command = doc["command"].as<String>();

    if(command == "wson") {
      digitalWrite(LED_PIN, HIGH);
    }
    if(command == "wsoff") {
      digitalWrite(LED_PIN, LOW);
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

  pinMode(LED_PIN, OUTPUT);
  // pinMode(BUTTON_PIN, INPUT_PULLUP);  // GPIO35 は入力専用ピンのため、プルアップ抵抗は外付けのためコメントアウト

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
}

