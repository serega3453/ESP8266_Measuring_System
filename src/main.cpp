#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// ---- ПАРАМЕТРЫ ПИТАНИЯ/ЧУВСТВИТЕЛЬНОСТИ ----
const float VCC_SENSOR = 3.30;           // Фактическое питание ACS758, В (поставьте своё 3.30..3.35)
const float SENS_5V = 0.020;             // 20 мВ/А при 5В для 100B
const float SENS = SENS_5V * (VCC_SENSOR / 5.0);  // В/А, ратиометрия
const float VOUT_ZERO = VCC_SENSOR / 2.0;         // 0А уровень

// Выбор диапазона PGA: ±4.096В подходит под вход до 3.3В и даёт шаг 0.125мВ
void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5); // SDA, SCL на ESP8266
  ads.begin();
  ads.setGain(GAIN_ONE); // ±4.096V
  Serial.println("ACS758 x4 via ADS1115 -> UART, 1 Hz");
  Serial.print("VCC="); Serial.print(VCC_SENSOR, 3);
  Serial.print(" V, Sens="); Serial.print(SENS * 1000.0, 2);
  Serial.println(" mV/A");
}

// Чтение канала: возвращает напряжение (В) и ток (А)
void readChannel(uint8_t ch, float &v, float &i) {
  int16_t raw = ads.readADC_SingleEnded(ch);
  v = ads.computeVolts(raw);          // корректно учитывает выбранный GAIN
  i = (v - VOUT_ZERO) / SENS;         // А
}

void loop() {
  for (uint8_t ch = 0; ch < 4; ch++) {
    float v, i;
    readChannel(ch, v, i);
    Serial.print("CH"); Serial.print(ch); 
    Serial.print(": ");
    Serial.print(v, 4); Serial.print(" V, ");
    Serial.print(i, 3); Serial.print(" A  |  ");
  }
  Serial.println();
  delay(1000);
}
