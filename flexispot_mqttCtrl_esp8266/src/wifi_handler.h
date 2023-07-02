#include <ESP8266WiFi.h>
#include <credentials.h>





//extern wifiReconnectDelay[];
//extern prevWifiConnectTime; 
//extern wifiInitCnt;
//extern wifiConnectCnt;
//extern wifiReconnectCnt; 
//extern rssi;
//extern rssiAv;
//extern rssiCnt;
//extern lastWifiTime;



void initWifi();
void wifiHandler();
int rssiAvCalc(int rssi);