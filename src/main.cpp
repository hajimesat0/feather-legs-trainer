#include <Arduino.h>
#include <WiFi.h>

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
</div></body></html>\
";

WiFiServer server(80);

const int LED_PIN = 33;
const int BUTTON_PIN = 35;

void setup() {
  printf("setup funtion\n");

  WiFi.softAP(ssid, password);
  delay(100);
  WiFi.softAPConfig(ip, ip, subnet);
  IPAddress IP = WiFi.softAPIP();

  pinMode(LED_PIN, OUTPUT);
  // pinMode(BUTTON_PIN, INPUT_PULLUP);  // GPIO35 は入力専用ピンのため、プルアップ抵抗は外付けのためコメントアウト

  server.begin();
}

void loop() {
  printf("loop function\n");
  WiFiClient client = server.available();


  if( client ) {
    String current_line = "";
    while( client.available() ) {
      char c = client.read();
      printf("%c", c);
      if( c=='\n' ) {
        if( current_line.length() == 0 ) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println(html);
          break;
        } else {
          current_line = "";
        }
      } else if( c!='\r' ) {
        current_line += c;
      }

      if(current_line.endsWith("GET /?li")) {
        digitalWrite(LED_PIN, HIGH);
      }
      if(current_line.endsWith("GET /?lo")) {
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
  client.stop();

}

