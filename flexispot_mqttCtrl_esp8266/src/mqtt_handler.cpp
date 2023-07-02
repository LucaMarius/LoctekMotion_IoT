#include "mqtt_handler.h"
#include "credentials.h"
#include "desk_handler.h"

#define MQTT_DEFAULT_CYCLE 1000 //ms

#ifndef MQTT_USER
#define MQTT_USER ""
#endif
#ifndef MQTT_PASSWORD
#define MQTT_PASSWORD ""
#endif

// Topics
#define CLIENT "FLEXISPOT"
#define will_topic "office/desk/status"
#define preset_topic "office/desk/preset"
#define height_topic "office/desk/height"
#define payload_luca_sitting "luca_sitting"
#define payload_luca_standing "luca_standing"
#define payload_iza_sitting "iza_sitting"
#define payload_iza_standing "iza_standing"

uint32_t oldMsgTime;
uint16_t mqttMsgCycle = MQTT_DEFAULT_CYCLE;

enum class presets
{
  NONE = 0,
  SIT_LUCA,
  STAND_LUCA,
  SIT_IZA,
  STAND_IZA
}lastPreset;

extern WiFiClient espClient;
PubSubClient client(espClient);

void sendMQTTMsg()
{
  if(millis()-oldMsgTime > mqttMsgCycle)
  {   
    if(!client.connected())
    {
      reconnectMQTT();
    }

    client.publish(will_topic, "online", true);

    oldMsgTime = millis();
  }
}


void reconnectMQTT()
{
  Serial.print("\nAttempting MQTT connection ..."); 

  
  if(client.connect(CLIENT, MQTT_USER, MQTT_PASSWORD, will_topic, 0, true, "offline"))
  {
    Serial.print(" Connected");
    client.subscribe("office/desk/preset");
  }

  if(!client.connected())
  {
    Serial.print(" Disconnected");
  }
}


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("\n\nMessage arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print(", Length: ");
  Serial.print(length);
  Serial.print("\nPayload: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  // Check if message is a preset
  presets preset = presets::NONE;
  if(strcmp(topic, preset_topic) == 0)
  {    
    if(memcmp(payload, payload_luca_sitting, length) == 0)
    {
      preset = presets::SIT_LUCA;
    }
    else if(memcmp(payload, payload_luca_standing, length) == 0)
    {
      preset = presets::STAND_LUCA;
    }
    else if(memcmp(payload, payload_iza_sitting, length) == 0)
    {
      preset = presets::SIT_IZA;
    }
    else if(memcmp(payload, payload_iza_standing, length) == 0)
    {
      preset = presets::STAND_IZA;
    }
  }

  if (preset != lastPreset)
  {
    switch(preset)
    {
      case presets::SIT_LUCA:
      {
        sit(LUCA);
        Serial.print("\nLuca sitting");
        break;
      }
      case presets::STAND_LUCA:
      {
        stand(LUCA);
        Serial.print("\nLuca standing");
        break;
      }
      case presets::SIT_IZA:
      {
        sit(IZA);
        Serial.print("\nIza sitting");
        break;
      }
      case presets::STAND_IZA:
      {
        stand(IZA);
        Serial.print("\nIza standing");
        break;
      }
      default:
        break;
    }
    lastPreset = preset;
  }
}