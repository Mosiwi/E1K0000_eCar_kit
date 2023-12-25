/*
* Mosiwi eCar - For ESP32
* Boards Manager: esp32 by Espressif Systems, Version 2.0.14
*/

// For tuch
int threshold = 30;   // Trigger touch threshold value!
bool touchActive = false;
bool lastTouchActive = false;
bool testingLower = true;

// Interrupt function for touch T2(IO02).
void gotTouchT2(){
  if (lastTouchActive != testingLower) {
    touchActive = !touchActive;
    testingLower = !testingLower;
    // Touch ISR will be inverted: Lower <--> Higher than the Threshold after ISR event is noticed
    touchInterruptSetThresholdDirection(testingLower);
  }
}

void setup(){
  Serial.begin(115200);

  // For tuch T2(IO02).
  touchAttachInterrupt(T2, gotTouchT2, threshold);
  // Touch ISR will be activated when touchRead is lower than the Threshold
  touchInterruptSetThresholdDirection(testingLower);
}

void loop() {
  if(lastTouchActive != touchActive){
    lastTouchActive = touchActive;
    if (touchActive) {
      Serial.println("---- Touch was Pressed");
    } else {
      Serial.println("---- Touch was Released");
    }
    // Print the touch value of T2. 
    Serial.print("T2(IO2) = ");      
    Serial.println(touchRead(T2));
  }
}


