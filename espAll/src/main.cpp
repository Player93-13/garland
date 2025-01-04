#ifdef ESP32
#include <WiFi.h>
//#include <FS.h>
#include <LittleFS.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "anim.h"
#include "web.h"

typedef signed char         sint8_t;

//#define DEBUG
extern Anim anim;

WiFiManager wm;

void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer

  pinMode(PIN_PS_ON, OUTPUT);
  digitalWrite(PIN_PS_ON, HIGH);
  
  //wm.resetSettings();
  //wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(60);
  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
  if (wm.autoConnect("GarlandSetup")) {
    Serial.println("connected...yeey :)");
    WebServerSetup();
  }
  else {
    Serial.println("Configportal running");
  }
  
  LittleFS.begin();
  AnimSetup();
}

void loop(void) {
  //wm.process();
  anim.run();
}
