#pragma once
#include <Arduino.h>

void mqtt_init();
void mqtt_loop();
void mqtt_send(const String& payload);