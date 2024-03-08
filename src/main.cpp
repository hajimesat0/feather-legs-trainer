#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "CReactionTimer.h"

const char *ssid = "Haneashi-AP";
const char *password = "123456789";
const IPAddress ip(192, 168, 0, 100);
const IPAddress subnet(255,255,255,0);

static CReactionTimer *ReactionTimer;

void setup() {
  printf("setup funtion\n");

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  delay(100);
  WiFi.softAPConfig(ip, ip, subnet);

  ReactionTimer = new CReactionTimer(2);
  ReactionTimer->Setup();

}

void loop() {
  ReactionTimer->Loop();
}

