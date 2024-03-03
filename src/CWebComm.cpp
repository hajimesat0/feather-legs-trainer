#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "CWebComm.h"


CWebComm::CWebComm( IRequestListener *request_listener )
{
    RequestListener = request_listener;  

    if( !SPIFFS.begin(true) ) {
        printf("SPIFFS Mount Failed\n");
        return;
    }

    WebServer = new AsyncWebServer(80);
    WebSocketServer = new AsyncWebSocket("/ws");
    
    WebSocketServer->onEvent(
        [this](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
            this->OnWsEvent( server, client, type, arg, data, len );
        }
    );
    WebServer->addHandler( WebSocketServer );
    WebServer->onNotFound([](AsyncWebServerRequest *request){
        printf("NOT_FOUND: ");
        request->send(404);
    });
    WebServer->on(
        "/", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            request->send( SPIFFS, "/index.html" );
        }
    );
    WebServer->begin();
}


CWebComm::~CWebComm()
{
}


void CWebComm::Loop()
{
    WebSocketServer->cleanupClients();
}


void CWebComm::OnWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
  switch( type ) {
    case WS_EVT_CONNECT:
    //   printf("ws[%s][%u] connect\n", server->url(), client->id());
      break;
    
    case WS_EVT_DISCONNECT:
    //   printf("ws[%s][%u] disconnect\n", server->url(), client->id());
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


void CWebComm::SendMeasuringResult( unsigned int button_id, unsigned long time_ms )
{
    DynamicJsonDocument doc(1024);
    doc["buttonId"] = button_id;
    doc["time"] = time_ms;
    String json;
    serializeJson(doc, json);
    WebSocketServer->textAll(json);
}


void CWebComm::handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, (char *)data);
        String command = doc["command"].as<String>();

        if( command=="ReqLightOn") {
            unsigned int button_id = doc["buttonId"].as<unsigned int>();
            bool onoff = doc["onoff"].as<bool>();
            RequestListener->OnLight( button_id, onoff );
        } else if( command=="ReqReset") {
            RequestListener->OnReset();
        }

    // if(command == "ReqLightOn") {

    //   // 左側ボタン計測開始
    //   gIsLightOnLeft = true;

    //   // LED on 
    //   digitalWrite(LED_PIN_LEFT, HIGH);

    //   // 現在時刻記録
    //   gLightOnTime = millis();  // 現在時刻記録

    //   // 割り込み有効化
    //   attachInterrupt(BUTTON_PIN_LEFT, buttonPushedLeft, FALLING);
    // }
    // if(command == "bt_right") {
    //   // 右側ボタン計測開始
    //   gIsLightOnRight = true;

    //   // LED on 
    //   digitalWrite(LED_PIN_RIGHT, HIGH);

    //   // 現在時刻記録
    //   gLightOnTime = millis();  // 現在時刻記録

    //   // 割り込み有効化
    //   attachInterrupt(BUTTON_PIN_RIGHT, buttonPushedRight, FALLING);
    // }

  }
}



