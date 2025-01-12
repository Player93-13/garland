#define CalibrationFileName "/calibration.json"

struct LastState
{
  int animId = 101;
  int palId = 0;
};

//горизонатльное кольцо на елке
struct TreeRound
{
  //количество светодиодов в кольце
  uint16_t r_width;

  //ближайший к окну светодиод (азимут)
  uint16_t r_azimuth;

  //позиция кольца относительно строк на занавесе
  uint16_t r_ypos;
};

void LoadConfig();

void SaveConfig();

void SaveCalibration(uint8_t *data, size_t len);
