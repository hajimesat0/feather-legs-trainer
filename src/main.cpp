#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "Haneashi-AP";
const char *password = "123456789";
const IPAddress ip(192, 168, 0, 100);
const IPAddress subnet(255,255,255,0);

const char html[] =
"\
<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'>\
<style>input {font-size:24pt;margin:8px;width:100px;height:80px}\
div {font-size:24pt;color:black;text-align:center;width:400px;}</style>\
<title>羽根足 操作</title></head>\
<body><div><p>羽根足 操作</p>\
<form method='get'>\
<input type='submit' name='li' value='点灯' />\
<input type='submit' name='lo' value='消灯' />\
</form>\
<button id='wson'>WS点灯</button>\
<button id='wsoff'>WS消灯</button>\
<script>\
let ws = new WebSocket(`ws://${window.location.hostname}/ws`);\
ws.onopen = function() {console.log('ws open')};\
ws.onclose = function() {console.log('ws close')};\
ws.onmessage = function(event) {console.log('ws message', event.data)};\
let buttons = document.querySelectorAll('button');\
buttons.forEach(function(button) {\
  button.addEventListener('touchstart', function() {\
    ws.send(button.id);\
  });\
});\
</script>\
</div></body></html>\
";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const int LED_PIN = 33;
const int BUTTON_PIN = 35;

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
      printf("ws[%s][%u] recv event(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
      break;
    
    default:
      printf("ws[%s][%u] default(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
      break;
  }
}

void setup() {
  printf("setup funtion\n");

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
      request->send(200, "text/html", html);
    }
  );

  server.begin();
}

void loop() {
  ws.cleanupClients();


  // if( client ) {
  //   String current_line = "";
  //   while( client.available() ) {
  //     char c = client.read();
  //     printf("%c", c);
  //     if( c=='\n' ) {
  //       if( current_line.length() == 0 ) {
  //         client.println("HTTP/1.1 200 OK");
  //         client.println("Content-Type: text/html");
  //         client.println("Connection: close");
  //         client.println();
  //         client.println(html);
  //         break;
  //       } else {
  //         current_line = "";
  //       }
  //     } else if( c!='\r' ) {
  //       current_line += c;
  //     }

  //     if(current_line.endsWith("GET /?li")) {
  //       digitalWrite(LED_PIN, HIGH);
  //     }
  //     if(current_line.endsWith("GET /?lo")) {
  //       digitalWrite(LED_PIN, LOW);
  //     }
  //   }
  // }
  // client.stop();

}

