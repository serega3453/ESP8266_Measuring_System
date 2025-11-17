#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "mqtt_utils.h"

// ---- настройки ----
const char* WIFI_SSID = "Top_Secret";
const char* WIFI_PASS = "0977170329qw";

const char* MQTT_HOST = "192.6.0.167";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_TOPIC = "esp8266/thermal";

WiFiClient espClient;
PubSubClient mqtt(espClient);

static unsigned long lastReconnectAttempt = 0;

// ----------------------
// попытка подключения к MQTT
// ----------------------
static bool mqtt_reconnect() {
  if (mqtt.connected())
    return true;

  Serial.println("[MQTT] Connecting...");
  // clientId можно сделать уникальным, но пока пофиг
  if (mqtt.connect("esp8266-client")) {
    Serial.println("[MQTT] Connected");
    return true;
  } else {
    Serial.print("[MQTT] Failed, rc=");
    Serial.println(mqtt.state());
    return false;
  }
}

// ----------------------
void mqtt_init() {
  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("[WiFi] Connecting to ");
  Serial.println(WIFI_SSID);

  // не висим бесконечно, просто ждём чуть-чуть
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(200);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("[WiFi] Connected, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("[WiFi] Failed to connect (timeout), продолжим, но без сети");
  }

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  lastReconnectAttempt = 0;
}

// ----------------------
void mqtt_loop() {
  // если WiFi отвалился — даже не пытаемся MQTT
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  if (!mqtt.connected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt > 5000) {   // раз в 5 секунд пробуем
      lastReconnectAttempt = now;
      mqtt_reconnect();
    }
  } else {
    mqtt.loop();
  }
}

// ----------------------
void mqtt_send(const String& payload) {
  if (!mqtt.connected()) {
    // не подключены — нечего пытаться
    return;
  }
  mqtt.publish(MQTT_TOPIC, payload.c_str());
}
