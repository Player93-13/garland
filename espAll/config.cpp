#ifdef ESP32
#include <FS.h>
#include <SPIFFS.h>
#endif
#include <ArduinoJson.h>
#include "FS.h"
#include "config.h"
#include "anim.h"
#include "web.h"
#define LastStateFileName "/lastState.json"

LastState State = LastState();

void LoadConfig()
{
  File _f = SPIFFS.open(LastStateFileName, "r");
  DynamicJsonDocument _doc(_f.size() * 4 + 100);
  DeserializationError _error = deserializeJson(_doc, _f);
#ifdef DEBUG
  if (_error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(_error.c_str());
    return;
  }
#endif
  State.animId = _doc["animId"] | 101;
  State.palId = _doc["palId"] | 0;
  _f.close();

  if (State.palId > 100)
  {
    loadCustomPal(State.palId);
    State.palId = PALCUSTOM_ID;
  }
}

void SaveConfig()
{
  SPIFFS.remove(LastStateFileName);
  File _f = SPIFFS.open(LastStateFileName, "w");
  if (!_f) {
#ifdef DEBUG
    Serial.println(F("Failed to create file"));
#endif
    return;
  }

  StaticJsonDocument<256> _doc;
  _doc["animId"] = State.animId;
  _doc["palId"] = State.palId;

  if (serializeJson(_doc, _f) == 0) {
#ifdef DEBUG
    Serial.println(F("Failed to write to file"));
#endif
  }
  _f.close();
}
