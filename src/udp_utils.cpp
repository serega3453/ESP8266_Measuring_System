#include "udp_utils.h"
#include <ESP8266WiFi.h>

WiFiUDP udp;

void udpInit() {
WiFi.mode(WIFI_STA);
WiFi.begin("Top_Secret", "0977170329qw");

Serial.print("WiFi: Connecting");
while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
}
Serial.println();
Serial.print("WiFi connected, IP: ");
Serial.println(WiFi.localIP());
    udp.begin(9500); // слушать не обязательно, но пусть
}

void udpSend(const String& s) {
    udp.beginPacket("255.255.255.255", 9500);
    udp.write(s.c_str());
    udp.endPacket();
}
