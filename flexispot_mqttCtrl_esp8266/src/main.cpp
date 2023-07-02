#include <Arduino.h>
#include <wifi_handler.h>
#include <mqtt_handler.h>
#include <desk_handler.h>
#include <SoftwareSerial.h>

#ifdef MQTT_SERVER
#else 
#define MQTTMODE false 
#define MQTT_SERVER ""
#endif

#ifndef MQTT_PORT 
#define MQTT_PORT 1883
#endif 

extern PubSubClient client;

void setup() {
  Serial.begin(9600);
  delay(200);

  initWifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqtt_callback);
  desk_init();
}

byte history[2];
extern SoftwareSerial sSerial;

bool msgRcv = false;
uint8_t byteCnt = 0; 
uint8_t msgLen = 0;
char rcv[9];

void loop() 
{
  wifiHandler();

  if(WiFi.status() == WL_CONNECTED)
  {
    sendMQTTMsg(); 
    client.loop();
  }
}