#include "wifi_handler.h"

#define DEBUG_WIFI 1
#define WIFI_MAX_INITCNT 10
#define WIFI_MAX_CONNECTCNT 100
#define RSSI_SAMPLECNT 10

#if defined(ssid) && defined (wpa2) 
#else
#define ssid "ssid"
#define wpa2 "wpa2"
#endif

uint32_t wifiReconnectDelay[] = {100, 500, 1000, 5000, 10000, 30000, 60000, 300000};
uint32_t prevWifiConnectTime = 0; 
uint8_t wifiInitCnt = 0;
uint8_t wifiConnectCnt = 0;
uint8_t wifiReconnectCnt = 0; 
int rssi = 0;
int rssiAv = 0;
uint8_t rssiCnt = 0;
uint32_t lastWifiTime;

enum rssi
{
  UNAVAILABLE = 0, 
  BAD, 
  GOOD, 
  STRONG
}wifi_rssi;

WiFiClient espClient; 

void initWifi()
{
  WiFi.begin(ssid, wpa2);

  if(DEBUG_WIFI)
  {
    Serial.print("\n\nTrying to connect to WiFi");

    if(WiFi.status() == WL_CONNECTED)
    {
      Serial.print("\n"); 
      Serial.print("\n---WIFI CONNECTED---"); 
      Serial.print("\nIPV4: "); 
      Serial.print(WiFi.localIP());
    }
    else
    {
      Serial.print("\n"); 
      Serial.print("\n---WIFI ");
      Serial.print(" DISCONNECTED---");     
    }
  }
}

void wifiHandler()
{
  if(WiFi.status() == WL_CONNECTED) //if connected via wifi try to send mqtt msg
  {
    wifiReconnectCnt = 0;
    wifiConnectCnt++; 

    if(wifiConnectCnt > WIFI_MAX_CONNECTCNT)
    {
      wifiConnectCnt = WIFI_MAX_CONNECTCNT;
    }


    if(wifiConnectCnt >= WIFI_MAX_CONNECTCNT)
    {
      int rssiTmp = rssiAvCalc(WiFi.RSSI());

      if(rssiTmp)
      {
        rssi = rssiTmp;
        if(rssi < -60)
        {
          wifi_rssi = STRONG;
        }
        else if((-70 > rssi) && (rssi >= -60))
        {
          wifi_rssi = GOOD;
        }
        else if(rssi >= -70)
        {
          wifi_rssi = BAD;
        }
    }
    
    if(DEBUG_WIFI && (millis()-lastWifiTime > 5000))
    {
      Serial.print("\n---WIFI CONNECTED---"); 
      Serial.print("\nIPV4: ");
      Serial.print(WiFi.localIP());
      Serial.print("\nRSSI: ");
      Serial.print(rssi);
      Serial.print("dBm");
      Serial.print("\nenum: ");
      Serial.print(wifi_rssi);
      lastWifiTime = millis();
    }

    }


  }
  else if(WiFi.status() != WL_CONNECTED && (millis()-prevWifiConnectTime > wifiReconnectDelay[wifiReconnectCnt])) //if not connected try again later, reconnect delay time inceases with a failed reconnect
  {
    prevWifiConnectTime = millis();
    initWifi();
    wifi_rssi = UNAVAILABLE;
    wifiConnectCnt = 0;
    wifiReconnectCnt++;

    if(wifiReconnectCnt > sizeof(wifiReconnectDelay)/sizeof(wifiReconnectDelay[0])-1)
    {
      wifiReconnectCnt = sizeof(wifiReconnectDelay)/sizeof(wifiReconnectDelay[0])-1;
    }

    if(DEBUG_WIFI)
    {
      Serial.print("\nReconnect to Wifi - Cnt: ");
      Serial.print(wifiReconnectCnt);
      Serial.print(" - Next Timeout: ");
      Serial.print(wifiReconnectDelay[wifiReconnectCnt]);   
      Serial.print(" - prev Time: ");
      Serial.print(prevWifiConnectTime); 
    }
  } 
}

int rssiAvCalc(int rssi)
{
  if(rssiCnt <= RSSI_SAMPLECNT)
  {
    rssiAv += rssi;
    rssiCnt++;
    return 0;
  }
  else
  {
    rssiAv = rssiAv / rssiCnt; 
    rssiCnt = 0;
    return rssiAv;
  }
}
