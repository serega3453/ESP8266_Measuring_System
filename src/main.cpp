#include <ntc_utils.h>
#include <sht_utils.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

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

  // ---- SHT21 ----
  float sht_t  = readSHT21_T();
  float sht_rh = readSHT21_RH();

  Serial.print("|  SHT_T: ");
  Serial.print(sht_t, 2);
  Serial.print(" °C  ");

  Serial.print("SHT_RH: ");
  Serial.print(sht_rh, 1);
  Serial.println(" %");

  delay(1000);
}
