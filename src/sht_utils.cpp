#include <sht_utils.h>

const uint8_t SHT21_ADDR = 0x40;
const uint8_t TRIGGER_T  = 0xF3;
const uint8_t TRIGGER_RH = 0xF5;

float readSHT21_RH() {
  Wire.beginTransmission(SHT21_ADDR);
  Wire.write(TRIGGER_RH);
  Wire.endTransmission();
  delay(29); // время измерения

  Wire.requestFrom(SHT21_ADDR, 2);
  if (Wire.available() < 2) return NAN;

  uint16_t raw = (Wire.read() << 8) | Wire.read();
  raw &= ~0x0003;

  return -6.0 + 125.0 * raw / 65536.0;
}

float readSHT21_T() {
  Wire.beginTransmission(SHT21_ADDR);
  Wire.write(TRIGGER_T);
  Wire.endTransmission();
  delay(85); // время измерения

  Wire.requestFrom(SHT21_ADDR, 2);
  if (Wire.available() < 2) return NAN;

  uint16_t raw = (Wire.read() << 8) | Wire.read();
  raw &= ~0x0003; // очистка статуса

  return -46.85 + 175.72 * raw / 65536.0;
}