#include <ESP8266WiFi.h>

#include "anim.h"
#include "web.h"

//#define DEBUG
extern Anim anim;

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin("HUAWEI-42", "123asdqwe");

  SPIFFS.begin(); 
  AnimSetup();
  WebServerSetup();
#ifdef DEBUG
  Serial.println("webSocket server started");
#endif
}

void loop(void) {
  anim.run();
}
