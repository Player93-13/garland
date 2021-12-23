#ifdef ESP32
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include "anim.h"
#include "web.h"

typedef signed char         sint8_t;

//#define DEBUG
extern Anim anim;

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer

  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin("HUAWEI-42", "123asdqwe");

  SPIFFS.begin();
  WebServerSetup();
#ifdef DEBUG
  Serial.println("webSocket server started");
#endif
  AnimSetup();
}

void loop(void) {
  anim.run();
}
