#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "commands.h"

#include <Adafruit_NeoPixel.h>
#include "color.h"
#include "palette.h"
#include "anim.h"

#define ANIMS 9 //number of animations
#define PALS 8 //number of palettes
#define INTERVAL 10000 //change interval, msec

//#define DEBUG

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

WebSocketsServer webSocket = WebSocketsServer(81);

String JSONpalFile = "/pallete.json";
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
int rId = 0;

Color PalCustom_ [45];
Palette PalCustom = { 1, PalCustom_ };

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue, &PalCustom};

Anim anim = Anim();

constexpr bool disableAutoChangeEffects = true;
bool runColor = false;

unsigned long ms = 10000;//startup animation duration, 10000 for "release" AnimStart

int paletteInd = random(PALS);
int animInd = 3;
extern Adafruit_NeoPixel pixels;

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

  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.on("/effset", HTTP_POST, handleSendCommand);
  server.on("/newpal", HTTP_POST, handleNewPal); // Создание новой палитры
  server.on("/delpal", HTTP_POST, handleDelPal); // Удаление custom - палитры

  server.begin();                           // Actually start the server
#ifdef DEBUG
  Serial.println("HTTP server started");
#endif

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
#ifdef DEBUG
  Serial.println("webSocket server started");
#endif

  pixels.begin();
  //randomSeed(analogRead(0)*analogRead(1));
  anim.setAnim(animInd);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
  anim.doSetUp();
}

void loop(void) {
  server.handleClient();
  webSocket.loop();

  if (!runColor)
  {
    anim.run();
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
#ifdef DEBUG
      Serial.printf("[%u] Disconnected!\n", num);
#endif
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
#ifdef DEBUG
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
#endif
        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
#ifdef DEBUG
      Serial.printf("[%u] get Text: %s\n", num, payload);
#endif
      char str[6];
      for (int i = 0; i < 6; i++)
      {
        str[i] = payload[i + 1];
      }
      byte color[3];
      hex2bin(str, color);

      runColor = true;
      byte r = (int)pgm_read_byte_near(BRI + color[0]);
      byte g = (int)pgm_read_byte_near(BRI + color[1]);
      byte b = (int)pgm_read_byte_near(BRI + color[2]);
      for (int i = 0; i < LEDS; i++)
      {
        if (i < GARL)
          pixels.setPixelColor(i, pixels.Color(r, g, b));
        else
          pixels.setPixelColor(i, pixels.Color(g, r, b));
      }
      pixels.show();

      break;
  }
}

void handleSendCommand()
{
  if ( ! server.hasArg("b1") || ! server.hasArg("b2")
       || server.arg("b1") == NULL || server.arg("b2") == NULL) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  if (server.arg("b2").toInt() < 100)
  {
    animInd = server.arg("b1").toInt();
    int palId = server.arg("b2").toInt();
    if (palId < PALS && palId >= 0 && animInd < ANIMS && animInd >= 0)
    {
      anim.setAnim(animInd);
      anim.setPalette(pals[palId]);
      ms = millis() + INTERVAL;
    }
  }
  else
  {
    int palid = server.arg("b2").toInt();
    File _f = SPIFFS.open(JSONpalFile, "r");

    DynamicJsonDocument _doc(_f.size() * 4 + 5000);
    DeserializationError _error = deserializeJson(_doc, _f);
#ifdef DEBUG
    if (_error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(_error.c_str());
      return;
    }
#endif
    int pindex = -1;
    for (int i = 0; i < _doc.size(); i++)
    {
      int pid = _doc[i]["id"];
      if (pid == palid)
      {
        pindex = i;
        break;
      }
    }

    byte leng = (byte)(_doc[pindex]["colors"].size());
    PalCustom.numColors = leng;
    for (int j = 0; j < leng; j++)
    {
      char str[6];
      for (int i = 0; i < 6; i++)
      {
        String temp = _doc[pindex]["colors"][j];
        str[i] = temp[i];
      }
      byte color[3];
      hex2bin(str, color);

      Color _c;
      _c.r = color[0];
      _c.g = color[1];
      _c.b = color[2];

      PalCustom_[j] = _c;
    }
    animInd = server.arg("b1").toInt();
    anim.setAnim(animInd);
    anim.setPalette(pals[7]);
    ms = millis() + INTERVAL;
    _f.close();
  }
  runColor = false;
  server.send(200, "text/html", "ok");
}

void handleNewPal()
{
  if ( ! server.hasArg("palname") || ! server.hasArg("sc")
       || server.arg("palname") == NULL || server.arg("sc") == NULL)
  { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  String palname = server.arg("palname");
  String sc = server.arg("sc");
  long id = random(101, 10000000);
#ifdef DEBUG
  Serial.print("http post [newpal] palname:");
  Serial.print(palname);
  Serial.print(", sc:");
  Serial.println(sc);
#endif

  addNePalToFile(id, palname, sc);

  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void handleDelPal()
{
  if ( ! server.hasArg("id") || server.arg("id") == NULL)
  {
    server.send(400, "text/plain", "400: Invalid Request");
    return;
  }
  int id = server.arg("id").toInt();
#ifdef DEBUG
  Serial.print("http post [delpal] id:");
  Serial.println(id);
#endif

  deletePal(id);

  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void addNePalToFile(long id, String palName, String colors)
{
  File _f = SPIFFS.open(JSONpalFile, "r");

  DynamicJsonDocument _doc(_f.size() * 4 + 5000);
  DeserializationError _error = deserializeJson(_doc, _f);
#ifdef DEBUG
  if (_error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(_error.c_str());
    return;
  }
#endif

  StaticJsonDocument<1200> _newP;
  _newP["id"] = id;
  _newP["name"] = palName;
  JsonArray _colrs = _newP.createNestedArray("colors");
  for (int i = 0; i < colors.length(); i += 6)
  {
    _colrs.add(colors.substring(i, i + 6));
  }

  _doc.add(_newP);

  _f.close();

  File _fw = SPIFFS.open(JSONpalFile, "w");
  serializeJson(_doc, _fw);
  _fw.close();
#ifdef DEBUG
  Serial.print(JSONpalFile);
  Serial.println(" updated.");
#endif
}

void deletePal(int id)
{
  File _f = SPIFFS.open(JSONpalFile, "r");

  DynamicJsonDocument _doc(_f.size() * 4 + 5000);
  DeserializationError _error = deserializeJson(_doc, _f);
#ifdef DEBUG
  if (_error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(_error.c_str());
    return;
  }
#endif

  for (int i = 0; i < _doc.size(); i++)
  {
    int pid = _doc[i]["id"];
    if (pid == id)
    {
      _doc.remove(i);
      break;
    }
  }
  _f.close();

  File _fw = SPIFFS.open(JSONpalFile, "w");
  serializeJson(_doc, _fw);
  _fw.close();
#ifdef DEBUG
  Serial.print(JSONpalFile);
  Serial.println(" updated.");
#endif
}

void sendCommand(byte command, byte arrLength, byte* arr)
{
  byte *mass = new byte[arrLength + 4];
  createCommand(mass, command, arrLength, arr);
#ifdef DEBUG
  for (byte i = 0; i < arrLength + 4; i ++)
  {
    Serial.print(mass[i]);
    Serial.write(32);
    Serial.flush();
  }
  Serial.println(" ");
#else
  Serial.write(mass, arrLength + 4);
  Serial.flush();
#endif

  delete [] mass;
}

void createCommand(byte* mass, byte command, byte arrLength, byte* arr)
{
  mass[0] = COMMAND_MARKER;
  mass[1] = arrLength + 3;
  mass[2] = command;
  for (int i = 0; i < arrLength; i++)
  {
    mass[i + 3] = arr[i];
  }

  byte checksum = 0; // could be an int if preferred
  for (int i = 1; i < arrLength + 3; i++)
  {
    checksum += mass[i];
  }

  mass[arrLength + 3] = checksum;
}

int char2int(char input)
{
  if (input >= '0' && input <= '9')
    return input - '0';
  if (input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if (input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  return 0;
}

void hex2bin(const char* src, byte* target)
{
  while (*src && src[1])
  {
    *(target++) = char2int(*src) * 16 + char2int(src[1]);
    src += 2;
  }
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  rId = 0; //Сбрасываем счетчик команд на 0 (используется для быстрых ajax запросов)
#ifdef DEBUG
  Serial.println("handleFileRead: " + path);
#endif
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    if (path.endsWith(".json"))
    {
      server.sendHeader("Cache-Control", "no-cache");
    }
    else if (path.endsWith(".html"))
    {
      server.sendHeader("Cache-Control", "public, max-age=3600‬");
    }
    else
    {
      server.sendHeader("Cache-Control", "public, only-if-cached, max-age=2678400‬");
    }
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
#ifdef DEBUG
  Serial.println("\tFile Not Found");
#endif
  return false;                                         // If the file doesn't exist, return false
}
