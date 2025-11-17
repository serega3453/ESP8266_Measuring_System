#include <ntc_utils.h>

const float BETA = 3950.0;
const float R0 = 100000.0;
const float T0 = 298.15;
const float SERIES_RESISTOR = 10000.0;

extern Adafruit_ADS1115 ads;

float readTemperatureNTC(int channel) {
  int16_t adcValue = ads.readADC_SingleEnded(channel);
  float voltage = adcValue * 0.1875 / 1000.0;
  float vcc = 3.3;
  float resistance = SERIES_RESISTOR * (vcc / voltage - 1.0);
  float temperatureK = 1.0 / (1.0 / T0 + (1.0 / BETA) * log(resistance / R0));
  return temperatureK - 273.15;
}