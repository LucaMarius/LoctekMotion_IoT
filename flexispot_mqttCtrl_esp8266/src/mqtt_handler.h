#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void sendMQTTMsg();
void reconnectMQTT();
void mqtt_callback(char* topic, byte* payload, unsigned int length);