 /*
* Mosiwi eCar - For ESP32
* Boards Manager: esp32 by Espressif Systems, Version 2.0.14
* Library Manager: IRremoter by shirriff, z3t0, ArminJo, Version 4.2.0
*/
#include <Arduino.h>
#include <IRremote.hpp>

#define irPin 39

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(irPin); 
}

void loop(){
  if (IrReceiver.decode()) {
    /*******
    0x18: ▲
    0x52: ▼
    0x08: ◀
    0x5A: ▶
    0x1C: OK
    0x16: *
    0x0D: #
    0x45: 1
    0x46: 2
    0x47: 3
    0x44: 4
    0x40: 5
    0x43: 6
    0x07: 7
    0x15: 8
    0x09: 9
    0x19: 0
    *******/    
    Serial.print(F("Command 0x"));
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    IrReceiver.resume();
  }
}

