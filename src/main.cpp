#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// Константы для NTC
const float BETA = 3950.0;        // B-параметр
const float R0 = 100000.0;        // Номинал NTC при 25°C
const float T0 = 298.15;          // 25°C в К
const float SERIES_RESISTOR = 10000.0; // Резистор в делителе, Ом

// Для встроенного АЦП ESP8266
const float ADC_VREF = 1.0;  // макс. напряжение на A0
const int   ADC_MAX  = 1023; // 10 бит

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5);  // SDA, SCL
  ads.begin();
  Serial.println("ESP8266 Thermal Monitor + Internal ADC");
}

float readTemperature(int channel) {
  int16_t adcValue = ads.readADC_SingleEnded(channel);
  float voltage = adcValue * 0.1875 / 1000.0;  // шаг 0.1875мВ
  float vcc = 3.3;
  float resistance = SERIES_RESISTOR * (vcc / voltage - 1.0);
  float temperatureK = 1.0 / (1.0 / T0 + (1.0 / BETA) * log(resistance / R0));
  return temperatureK - 273.15;
}

void loop() {
  // ---- Температуры ----
  for (int i = 0; i < 4; i++) {
    float temp = readTemperature(i);
    Serial.print("T");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(temp, 2);
    Serial.print(" °C  ");
  }

  // ---- Встроенный АЦП ----
  int raw = analogRead(A0);
  float voltage = (raw * ADC_VREF) / ADC_MAX;
  Serial.print("|  ADC: ");
  Serial.print(voltage, 3);
  Serial.println(" V");

  delay(1000);
}
