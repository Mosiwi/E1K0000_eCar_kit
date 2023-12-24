/*
* Mosiwi eCar - For ESP32
* Bsards Manager: esp32 by Espressif Systems, Version 2.0.14
* Library Manager: ESP32Servo by Kevin harrington, John K.Bennett, Versoin 1.1.1
*/
#include <ESP32Servo.h> 

//For servo
Servo myAservo;      // create servo object to control a servo
Servo myBservo;      
int AservoPin = 13;
int BservoPin = 15;
int AoldData = 0;
int BoldData = 0;

//Create task1 handle and initialize them.
TaskHandle_t TASK_HandleOne = NULL;

// The function body of task1, since the input parameter is NULL, 
// so the function body needs to be void * parameter, otherwise an error is reported.
void TASK_ONE(void *param ){
  for(;;){
    // Servo
    for(int angle=0; angle<=180; angle++){            
      myAservo.write(angle);
      myBservo.write(angle);
      delay(10);   
    }
    for(int angle=180; angle>0; angle--){            
      myAservo.write(angle);
      myBservo.write(angle);
      delay(10);   
    }
  }
  // When the thread terminates, the thread resource is released.
  //Serial.println("Ending TaskOne!");
  //vTaskDelete( TASK_HandleOne );
}

void setup(){
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //Serial.println();

  //Define the first thread 
  xTaskCreate(
    TASK_ONE,          // Task function
    "TaskOne",         // Task name
    32*1024,           // Stack size, set as needed
    NULL,              
    1,                 // priority
    &TASK_HandleOne    // Task handle
  );  

  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  myAservo.setPeriodHertz(50);  // Standard 50hz servo
  myBservo.setPeriodHertz(50);  // Standard 50hz servo

  // attaches the servo on pin to the servo object
  // using SG90 servo min/max of 500us and 2400us
  // for MG995 large servo, use 1000us and 2000us,
  // which are the defaults, so this line could be
  // "myservo.attach(servoPin);"
  myAservo.attach(AservoPin, 500, 2400);   
  myBservo.attach(BservoPin, 500, 2400);  
  myAservo.write(90);
  myBservo.write(90);
}

void loop() {
  /*
  // Servo
  for(int angle=0; angle<=180; angle++){            
    myAservo.write(angle);
    myBservo.write(angle);
    delay(10);   
  }
  for(int angle=180; angle>0; angle--){            
    myAservo.write(angle);
    myBservo.write(angle);
    delay(10);   
  }
  */
}


