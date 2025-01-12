#include <LittleFS.h>
#include <ArduinoJson.h>
#include "config.h"
#include "anim.h"
#include "web.h"
#define LastStateFileName "/lastState.json"

LastState State = LastState();

// калибровка елки
TreeRound rounds[256];
uint8_t rounds_count;

void LoadConfig()
{
  File _f = LittleFS.open(LastStateFileName, FILE_READ);
  JsonDocument _doc;
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
  LittleFS.remove(LastStateFileName);
  File _f = LittleFS.open(LastStateFileName, FILE_WRITE);
  if (!_f) {
#ifdef DEBUG
    Serial.println(F("Failed to create file"));
#endif
    return;
  }

  JsonDocument _doc;
  _doc["animId"] = State.animId;
  _doc["palId"] = State.palId;

  if (serializeJson(_doc, _f) == 0) {
#ifdef DEBUG
    Serial.println(F("Failed to write to file"));
#endif
  }
  _f.close();
}

void SaveCalibration(uint8_t *data, size_t len)
{
  LittleFS.remove(CalibrationFileName);
  File _f = LittleFS.open(CalibrationFileName, FILE_WRITE);
  _f.write(data, len);
  _f.close();
}