#include "Arduino.h"
#include "desk_handler.h"
#include <SoftwareSerial.h>

#define displayPin20 4 // D2 GPIO4
#define rxPin 12 // D5 GPIO12
#define txPin 14 // D6 GPIO14 



// Supported Commands
const byte wakeup[] = { 0x9b, 0x06, 0x02, 0x00, 0x00, 0x6c, 0xa1, 0x9d };
const byte command_up[] = { 0x9b, 0x06, 0x02, 0x01, 0x00, 0xfc, 0xa0, 0x9d };
const byte command_down[] = { 0x9b, 0x06, 0x02, 0x02, 0x00, 0x0c, 0xa0, 0x9d };
const byte command_m[] =  {0x9b, 0x06, 0x02, 0x20, 0x00, 0xac, 0xb8, 0x9d };
const byte command_preset_1[] = { 0x9b, 0x06, 0x02, 0x04, 0x00, 0xac, 0xa3, 0x9d };
const byte command_preset_2[] = { 0x9b, 0x06, 0x02, 0x08, 0x00, 0xac, 0xa6, 0x9d };
const byte command_preset_3[] = { 0x9b, 0x06, 0x02, 0x10, 0x00, 0xac, 0xac, 0x9d };
const byte command_preset_4[] = { 0x9b, 0x06, 0x02, 0x00, 0x01, 0xac, 0x60, 0x9d };

SoftwareSerial sSerial(rxPin, txPin); // RX, TX

void desk_init()
{
  sSerial.begin(9600);    // Flexispot E8

  pinMode(displayPin20, OUTPUT);
  digitalWrite(displayPin20, LOW);
}

void turnon() {
  // Turn desk in operating mode by setting controller pin20 to HIGH
  Serial.println("\nsending turn on command");
  digitalWrite(displayPin20, HIGH);
  digitalWrite(displayPin20, LOW);
}


void wake() {
  turnon();

  // This will allow us to receive the current height
  Serial.println("sending wakeup command");
  sSerial.flush();
  sSerial.enableTx(true);
  sSerial.write(wakeup, sizeof(wakeup));
  sSerial.enableTx(false);
}


void sit(users user) {
    turnon();
    
    sSerial.flush();
    sSerial.enableTx(true);

    switch(user)
    {
        case LUCA:
        {
            sSerial.write(command_preset_4, sizeof(command_preset_4));
            Serial.println("sending sit preset luca");
            break;
        }
        case IZA:
        {
            sSerial.write(command_preset_2, sizeof(command_preset_2));
            Serial.println("sending sit preset iza");
            break;
        }
        default:
            break;
    }
  sSerial.enableTx(false);
}


void stand(users user) {
    turnon();
    
    sSerial.flush();
    sSerial.enableTx(true);

    switch(user)
    {
        case LUCA:
        {
            sSerial.write(command_preset_3, sizeof(command_preset_3));
            Serial.println("sending stand preset luca");
            break;
        }
        case IZA:
        {
            sSerial.write(command_preset_1, sizeof(command_preset_1));
            Serial.println("sending stand preset iza");
            break;
        }
        default:
            break;
    }
  sSerial.enableTx(false);
}