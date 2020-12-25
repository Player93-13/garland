#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <FS.h>
#include <ArduinoJson.h>

#include "anim.h"
#include "web.h"

//#define DEBUG
extern Anim anim;
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  WiFi.mode(WIFI_STA);
#ifdef DEBUG
  Serial.println("Connecting ...");
#endif
  WiFi.begin("HUAWEI-42", "123asdqwe");
#ifdef DEBUG
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
#endif

  SPIFFS.begin();                           // Start the SPI Flash Files System
  WebServerSetup();
#ifdef DEBUG
  Serial.println("webSocket server started");
#endif
  AnimSetup();
}

void loop(void) {
  anim.run();
}
