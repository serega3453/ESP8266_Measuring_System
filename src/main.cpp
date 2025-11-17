#include <sht_utils.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// ---- NTC константы ----
const float BETA = 3950.0;
const float R0 = 100000.0;
const float T0 = 298.15;
const float SERIES_RESISTOR = 10000.0;

// ---- Встроенный АЦП ----
const float ADC_VREF = 1.0;
const int   ADC_MAX  = 1023;

// ------------------------
// NTC
// ------------------------
float readTemperatureNTC(int channel) {
  int16_t adcValue = ads.readADC_SingleEnded(channel);
  float voltage = adcValue * 0.1875 / 1000.0;
  float vcc = 3.3;
  float resistance = SERIES_RESISTOR * (vcc / voltage - 1.0);
  float temperatureK = 1.0 / (1.0 / T0 + (1.0 / BETA) * log(resistance / R0));
  return temperatureK - 273.15;
}

// ------------------------
void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5); // SDA/SCL
  ads.begin();

  Serial.println("ESP8266 Thermal Monitor + Internal ADC + SHT21");
}

// ------------------------
void loop() {
  // ---- NTC ----
  for (int i = 0; i < 4; i++) {
    float temp = readTemperatureNTC(i);
    Serial.print("T");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(temp, 2);
    Serial.print(" °C  ");
  }

  // ---- ADC ----
  int raw = analogRead(A0);
  float adc_v = (raw * ADC_VREF) / ADC_MAX;
  Serial.print("| ADC: ");
  Serial.print(adc_v, 3);
  Serial.print(" V  ");

  // ---- SHT21 ----
  float sht_t  = readSHT21_T();
  float sht_rh = readSHT21_RH();

  Serial.print("| SHT_T: ");
  Serial.print(sht_t, 2);
  Serial.print(" °C  ");

  Serial.print("SHT_RH: ");
  Serial.print(sht_rh, 1);
  Serial.println(" %");

  delay(1000);
}
