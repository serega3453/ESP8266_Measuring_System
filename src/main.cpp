#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;  // Создаем объект для ADS1115

// Константы для NTC
const float BETA = 3950.0;  // B-параметр
const float R0 = 100000.0;  // Номинал NTC при 25°C
const float T0 = 298.15;    // 25°C в Кельвинах
const float SERIES_RESISTOR = 10000.0; // Резистор в делителе

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5);  // SDA, SCL
  ads.begin();
  Serial.println("ESP8266 Thermal Monitor started");
}

float readTemperature(int channel) {
  int16_t adcValue = ads.readADC_SingleEnded(channel);
  float voltage = adcValue * 0.1875 / 1000.0; // шаг 0.1875мВ
  float vcc = 3.3;
  float resistance = SERIES_RESISTOR * (vcc / voltage - 1.0);
  float temperatureK = 1.0 / (1.0 / T0 + (1.0 / BETA) * log(resistance / R0));
  return temperatureK - 273.15; // в °C
}

void loop() {
  for (int i = 0; i < 4; i++) {
    float temp = readTemperature(i);
    Serial.print("T");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(temp, 2);
    Serial.print(" °C  ");
  }
  Serial.println();
  delay(1000);
}
