/*
* Mosiwi eCar - For ESP32
* Boards Manager: esp32 by Espressif Systems, Version 2.0.14
*/
// For ultrasonic
const int trigPin = 32;
const int echoPin = 36;

//Create task1 handle and initialize them.
TaskHandle_t TASK_HandleOne = NULL;

// The function body of task1, since the input parameter is NULL, 
// so the function body needs to be void * parameter, otherwise an error is reported.
void TASK_ONE(void *param ){
  for(;;){
    unsigned long T;
    float readEchoPinErr = 0.9;

    // trig 10us
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Calculate the time of the ultrasonic echo
    while(digitalRead(echoPin) == LOW){
      delayMicroseconds(10);
      if(T > 10000){ break; }  // About 100 milliseconds.
      T++;
    }
    T = 0;
    while(digitalRead(echoPin) == HIGH){
      delayMicroseconds(10);
      if(T > 10000){ break; }  // About 100 milliseconds.
      T++;
    }

    // Calculate the distance of the object
    long dis = T * (10 + readEchoPinErr) * 0.034 / 2;
    Serial.print("Distance: ");
    Serial.print(dis);
    Serial.println("CM");
    delay(200);   
  }
  // When the thread terminates, the thread resource is released.
  //Serial.println("Ending TaskOne!");
  //vTaskDelete( TASK_HandleOne );
}

void setup(){
  Serial.begin(115200);

  //Define the first thread 
  xTaskCreate(
    TASK_ONE,          // Task function
    "TaskOne",         // Task name
    32*1024,           // Stack size, set as needed
    NULL,              
    1,                 // priority
    &TASK_HandleOne    // Task handle
  );  
 
  // FOr ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop(){
  /*
  unsigned long T;
  float readEchoPinErr = 0.9;

  // trig 10us
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calculate the time of the ultrasonic echo
  while(digitalRead(echoPin) == LOW){
    delayMicroseconds(10);
    if(T > 10000){ break; }  // About 100 milliseconds.
    T++;
  }
  T = 0;
  while(digitalRead(echoPin) == HIGH){
    delayMicroseconds(10);
    if(T > 10000){ break; }  // About 100 milliseconds.
    T++;
  }

  // Calculate the distance of the object
  long dis = T * (10 + readEchoPinErr) * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(dis);
  Serial.println("CM");
  delay(200); 
  */
}


