#include <ntc_utils.h>
#include <sht_utils.h>
#include "mqtt_utils.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// ------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("Serial initialized.");
  Wire.begin(4, 5); // SDA/SCL
  Serial.println("I2C initialized.");
  ads.begin();
  Serial.println("ADS1115 initialized.");
  mqtt_init();
  Serial.println("MQTT initialized.");

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

  String payload = "{";
  payload += "\"T1\":" + String(readTemperatureNTC(0), 2) + ",";
  payload += "\"T2\":" + String(readTemperatureNTC(1), 2) + ",";
  payload += "\"T3\":" + String(readTemperatureNTC(2), 2) + ",";
  payload += "\"T4\":" + String(readTemperatureNTC(3), 2) + ",";
  payload += "\"SHT_T\":" + String(readSHT21_T(), 2) + ",";
  payload += "\"SHT_RH\":" + String(readSHT21_RH(), 1);
  payload += "}";

  mqtt_send(payload);
  mqtt_loop();

  delay(1000);
}
