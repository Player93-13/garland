#ifdef ESP32
#include <AsyncTCP.h>
#include "AsyncUDP.h"
#elif defined(ESP8266)
#include <ESPAsyncTCP.h>
#include <ESPAsyncUDP.h>
#endif
#include "AsyncJson.h"
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include "palette.h"
#include "anim.h"
#include "config.h"

#define JSONpalFile "/pallete.json"

extern Anim anim;
extern Palette PalCustom;
extern Color PalCustom_[];
extern uint8_t wallBytes[];
extern bool wallBytesReady;
extern bool runWallVideo;
extern LastState State;
extern TreeRound rounds[];
extern uint8_t rounds_count;

AsyncWebServer server(80); // Create a webserver object that listens for HTTP request on port 80
AsyncWebSocket ws("/ws");
AsyncUDP udp;

void handleSendCommand(AsyncWebServerRequest *request);
void handleNewPal(AsyncWebServerRequest *request);
void handleDelPal(AsyncWebServerRequest *request);
void handleCalibrate(AsyncWebServerRequest *request);
void handleCalibrateBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void addNePalToFile(long id, String palName, String colors);
void deletePal(int id);
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void wsRunColor(uint8_t *payload);

void WebServerSetup()
{
  if (udp.listen(3265)) { // прием видеокадра
    udp.onPacket([](AsyncUDPPacket packet) {
      memcpy(&wallBytes, packet.data(), packet.length());
      wallBytesReady = true;
      });
  }

  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/plain", String(ESP.getFreeHeap())); });
  server.serveStatic(JSONpalFile, LittleFS, JSONpalFile).setCacheControl("no-cache");
  server.serveStatic(CalibrationFileName, LittleFS, CalibrationFileName).setCacheControl("no-cache");
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html").setCacheControl("max-age=604800");
  server.onNotFound([](AsyncWebServerRequest *request) { request->send(404); });
  server.on("/effset", HTTP_POST, handleSendCommand);
  server.on("/newpal", HTTP_POST, handleNewPal); // Создание новой палитры
  server.on("/delpal", HTTP_POST, handleDelPal); // Удаление custom - палитры
  server.on("/calibrate", HTTP_POST, handleCalibrate, nullptr, handleCalibrateBody);
  server.on("/calibrate_save", HTTP_POST, handleCalibrate, nullptr, handleCalibrateBody);

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.begin(); // Actually start the server
#ifdef DEBUG
  Serial.println("HTTP server started");
#endif
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

void hex2bin(const char *src, byte *target)
{
  while (*src && src[1])
  {
    *(target++) = char2int(*src) * 16 + char2int(src[1]);
    src += 2;
  }
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_DATA)
  {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len)
    {
      if (info->opcode == WS_TEXT)
      {
        data[len] = 0;
        if (data[0] = 'C')
        {
          wsRunColor(data);
        }
      }
    }
  }
}

void wsRunColor(uint8_t *payload)
{
  char str[6];
  for (int i = 0; i < 6; i++)
  {
    str[i] = payload[i + 1];
  }
  byte color[3];
  hex2bin(str, color);

  Color _c;
  _c.r = color[0];
  _c.g = color[1];
  _c.b = color[2];

  PalCustom.numColors = 1;
  PalCustom_[0] = _c;

  anim.setAnim(ANIM_FILL_ID);
  anim.setPaletteById(PALCUSTOM_ID);
}

void loadCustomPal(int pId)
{
  File _f = LittleFS.open(JSONpalFile, "r");
  JsonDocument _doc;
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
    if (pid == pId)
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
  _f.close();
}

void handleSendCommand(AsyncWebServerRequest *request)
{
  if (request->hasParam("b1", true) && request->hasParam("b2", true))
  {
    int aId = request->getParam("b1", true)->value().toInt();
    int pId = request->getParam("b2", true)->value().toInt();

    State.animId = aId;
    State.palId = pId;
    SaveConfig();

    if (pId > 100)
    {
      loadCustomPal(pId);
      pId = PALCUSTOM_ID;
    }

    anim.setAnim(aId);
    anim.setPaletteById(pId);

    request->send(200);
    return;
  }

  request->send(400);
}

void handleNewPal(AsyncWebServerRequest *request)
{
  if (request->hasParam("palname", true) && request->hasParam("sc", true))
  {
    String palname = request->getParam("palname", true)->value();
    String sc = request->getParam("sc", true)->value();
    long id = random(101, 10000000);
#ifdef DEBUG
    Serial.print("http post [newpal] palname:");
    Serial.print(palname);
    Serial.print(", sc:");
    Serial.println(sc);
#endif

    addNePalToFile(id, palname, sc);

    request->redirect("/");
    return;
  }

  request->send(400);
}

void handleDelPal(AsyncWebServerRequest *request)
{
  if (request->hasParam("id", true))
  {
    int id = request->getParam("id", true)->value().toInt();

#ifdef DEBUG
    Serial.print("http post [delpal] id:");
    Serial.println(id);
#endif

    deletePal(id);

    request->redirect("/");
    return;
  }

  request->send(400);
}

void handleCalibrate(AsyncWebServerRequest *request)
{

}

void handleCalibrateBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  JsonDocument _doc;
  deserializeJson(_doc, data);
  rounds_count = _doc.size();
  for (int i = 0; i < _doc.size(); i++)
  {
    rounds[i].r_width = _doc[i]["r_width"];
    rounds[i].r_azimuth = _doc[i]["r_azimuth"];
    rounds[i].r_ypos = _doc[i]["r_ypos"];
  }

  if (request->url() == "/calibrate_save")
  {
    SaveCalibration(data, len);
    request->redirect("/");
    return;
  }

  anim.setAnim(ANIM_CALIBRATE_ID);
  request->send(200);
}

void addNePalToFile(long id, String palName, String colors)
{
  File _f = LittleFS.open(JSONpalFile, "r");

  JsonDocument _doc;
  DeserializationError _error = deserializeJson(_doc, _f);
#ifdef DEBUG
  if (_error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(_error.c_str());
    return;
  }
#endif

  JsonDocument _newP;
  _newP["id"] = id;
  _newP["name"] = palName;
  JsonArray _colrs = _newP["colors"].to<JsonArray>();
  for (int i = 0; i < colors.length(); i += 6)
  {
    _colrs.add(colors.substring(i, i + 6));
  }

  _doc.add(_newP);

  _f.close();

  File _fw = LittleFS.open(JSONpalFile, "w");
  serializeJson(_doc, _fw);
  _fw.close();
#ifdef DEBUG
  Serial.print(JSONpalFile);
  Serial.println(" updated.");
#endif
}

void deletePal(int id)
{
  File _f = LittleFS.open(JSONpalFile, "r");

  JsonDocument _doc;
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

  File _fw = LittleFS.open(JSONpalFile, "w");
  serializeJson(_doc, _fw);
  _fw.close();
#ifdef DEBUG
  Serial.print(JSONpalFile);
  Serial.println(" updated.");
#endif
}
