/*
* Mosiwi eCar - For ESP32
* Bsards Manager: esp32 by Espressif Systems, Version 2.0.14
* Library Manager: ESP32-audioI2S-master by Schreibfaul1, Version 2.0.0
*                  ESP32Servo by Kevin harrington, John K.Bennett, Versoin 1.1.1
*                  IRremoter by shirriff, z3t0, ArminJo, Version 4.2.0
*/
// Libraries. If you get errors compiling, please downgrade ESP32 by Espressif.
// Use version 1.0.2 (Tools, Manage Libraries).
#include <WiFi.h>
#include "i2c_m.h"
#include "app_server.h"

#include <IRremote.hpp>
#include <ESP32Servo.h> 

#include "Audio.h"
#include "SD.h"
#include "FS.h"

// Define Network SSID & Password
// Set ap to 1 to use mCar as Standalone Access Point with default IP 192.168.4.1
// Set ap to 0 to connect to a router using DHCP with hostname espressif
bool ap = 1; 
const char* ssid = "mCar";    //AP Name or Router SSID
const char* password = "";    //Password. Leave blank for open network. 

// AP Settings
int channel = 11;      // Channel for AP Mode
int hidden = 0;        // Probably leave at zero
int maxconnection = 1; // Only one device is allowed to connect

// For RGB LED
#define BUILTIN_RGBLED_PIN   12
#define NR_OF_LEDS   3
#define NR_OF_ALL_BITS 24*NR_OF_LEDS
// Note: This example uses Neopixel LED board, 3 LEDs chained one
//       after another, each RGB LED has its 24 bit value for color configuration (8b for each color)
//
//       Bits encoded as pulses as follows:
//       "0":
//          +-------+              +--
//          |       |              |
//          |       |              |
//       ---|       |--------------|
//          +       +              +
//          | 0.4us |   0.85 0us   |
//
//       "1":
//          +-------------+       +--
//          |             |       |
//          |             |       |
//       ---+             +-------+
//          |    0.8us    | 0.4us |
rmt_data_t led_data[NR_OF_ALL_BITS];
rmt_obj_t* rmt_send = NULL;
int color[] =  {0x55, 0x11, 0x77};  // RGB value: G, R, B
int RGBled_index = 0;
char c_RGBled = 0;

// For SD card
#define SD_CS          5
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18
// For speaker
#define I2S_DOUT      26
#define I2S_BCLK      14
#define I2S_LRC       25
Audio audio;
int music = -1;
int currentVol = 0;
int oldIndex = 0;
char oldState = 0; 

// Please enter your song list!
// Note that the song name cannot be longer than 40 characters!
char songs[][40]={
  "1_Free Loop.mp3",
  "2_Dream It Possible.mp3",
  "3_We Are The Brave.mp3",
  "4_My stupid heart.mp3",
  "5_She.mp3",
  "6_Big Big World.mp3",
  "7_My Love.mp3",
  "8_Hero.mp3",
  "9_Home.mp3",
  "10_I Got You.mp3",
  "11_Just One Last Dance.mp3",
  "12_Peaches.mp3",
  "13_Valder Fields.mp3",
  "14_You Raise Me Up.mp3"
}; 

char oldPkey = 0;
char beeps[][40]={
  "Mosiwi eCar.mp3",
  "bibi.mp3",
  "bi.mp3"
};

// Set the total number of songs.
int songSum = sizeof(songs)/sizeof(songs[0]);

// For ultrasonic
const int trigPin = 32;
const int echoPin = 36;

//For servo
Servo myAservo;      // create servo object to control a servo
Servo myBservo;      
int AservoPin = 13;
int BservoPin = 15;
int AoldData = 0;
int BoldData = 0;

// For tuch
int threshold = 30;   // Trigger touch threshold value!
bool touchActive = false;
bool lastTouchActive = false;
bool testingLower = true;

char ledState = 0;
char oldVP = 0;

/********************************************* API interface
// The initial I2C controls the motor.
void i2cMotorInit(void);
// Description: Set motor speed and steering.
// Parameters:  Motor: 0-3 --> M1-M4
//              direction: 0=CCW, 1=CW  
//              speed: 0-99
void SetMotor(char motor, char direction, char speed);
void CarRunForword(char speed);
void CarRunBack(char speed);
void CarTurnLeft(char speed);
void CarTurnRight(char speed);
void CarStop(void);
// Description: Set brightness of LEDs.
// Parameters:  brighrness: 0-99 
void LedBrightness(char brightness);  
*************************************************************/

//Create task1 and Task2 handles and initialize them.
TaskHandle_t TASK_HandleOne = NULL;
TaskHandle_t TASK_HandleTwo = NULL;

// The function body of task1, since the input parameter is NULL, 
// so the function body needs to be void * parameter, otherwise an error is reported.
void TASK_ONE(void *param ){
  char UltrasonicOFF = 0;
  for(;;){
    if(App.Skey == 1){
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
      char charNum[40] = "Distance: ";
      sprintf(&charNum[10], "%d", dis);
      strcat(charNum, "CM");
      displayWindow(1, charNum); 
      //Serial.println(dis);
      UltrasonicOFF = 1; 
    }else{
      if(UltrasonicOFF == 1){
        displayWindow(1, "Ultrasonic OFF");
        UltrasonicOFF = 0;
      }
    }
    
    // Servo
    if(AoldData != App.Aservo){
      AoldData = App.Aservo;
      myAservo.write(180-App.Aservo);
    }
    if(BoldData != App.Bservo){
      BoldData = App.Bservo;
      myBservo.write(180-App.Bservo);
    }
    delay(100);   
  }
  // When the thread terminates, the thread resource is released.
  //Serial.println("Ending TaskOne!");
  //vTaskDelete( TASK_HandleOne );
}
void TASK_TWO(void *param ){   
  for(;;){
    if(App.Akey == 1){
      c_RGBled = 1;
      // Init data with only one led ON
      int i=0, led, col, bit;
      for (led=0; led<NR_OF_LEDS; led++) {
        for (col=0; col<3; col++ ) {
          for (bit=0; bit<8; bit++){
            if ( (color[col] & (1<<(7-bit))) && (led == RGBled_index) ) {
              led_data[i].level0 = 1;
              led_data[i].duration0 = 8;
              led_data[i].level1 = 0;
              led_data[i].duration1 = 4;
            } else {
              led_data[i].level0 = 1;
              led_data[i].duration0 = 4;
              led_data[i].level1 = 0;
              led_data[i].duration1 = 8;
            }
            i++;
          }
        }
      }
      // make the led travel in the pannel
      if ((++RGBled_index)>=NR_OF_LEDS) {
        RGBled_index = 0;
      }
      // Send the data
      rmtWrite(rmt_send, led_data, NR_OF_ALL_BITS);
    }else{   
      //Turn off all leds.
      if(c_RGBled == 1){
        c_RGBled = 0;
        int led=0, i=0;
        for (led=0; led<NR_OF_LEDS*3*8; led++) {
          led_data[i].level0 = 1;
          led_data[i].duration0 = 4;
          led_data[i].level1 = 0;
          led_data[i].duration1 = 8;
          i++;
        }
        rmtWrite(rmt_send, led_data, NR_OF_ALL_BITS);
      }
    }

    // read the millivolts value for pin34.
    int analogVolt = analogReadMilliVolts(34)*2;
    // Full battery: 4200mV, Battery dead: 3200mV
    int voltP = ((float)(analogVolt - 3200)/(4200 - 3200))*100;  
    if(oldVP != voltP){  
      if(voltP < 0){voltP = 0;} 
      if(voltP > 100){voltP = 100;}
      char voltCharNum[10] = "P: ";
      sprintf(&voltCharNum[3], "%d", voltP);
      strcat(voltCharNum, "%");      
      displayWindow(2, voltCharNum);
    }  
    delay(100);
  }
  // When the thread terminates, the thread resource is released.
  //Serial.println("Ending TaskTwo!");
  //vTaskDelete( TASK_HandleTwo );
}

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
  // Prevent brownouts by silencing them. You probably want to keep this.
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //Serial.println();

  // For rgb LED
  if ((rmt_send = rmtInit(BUILTIN_RGBLED_PIN, RMT_TX_MODE, RMT_MEM_64)) == NULL){
    Serial.println("init sender failed\n");
  }
  float realTick = rmtSetTick(rmt_send, 100);
  Serial.printf("real tick set to: %fns\n", realTick);

  // For wifi
  Serial.println("ssid: " + (String)ssid);
  Serial.println("password: " + (String)password);
  if(!ap){
    // Connect to Router
    Serial.println("WiFi is Client mCar!");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.print("mCar Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect"); 
  } else {
    // Setup Access Point
    Serial.println("WiFi is Standalone mCar!");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid,password,channel,hidden,maxconnection);
    Serial.print("mCar Ready! Use 'http://");
    Serial.print(WiFi.softAPIP());
    Serial.println("' to connect");
  }     

  // For tuch
  touchAttachInterrupt(T2, gotTouchT2, threshold);
  // Touch ISR will be activated when touchRead is lower than the Threshold
  touchInterruptSetThresholdDirection(testingLower);

  //Define the first thread 
  xTaskCreate(
    TASK_ONE,          // Task function
    "TaskOne",         // Task name
    32*1024,           // Stack size, set as needed
    NULL,              
    1,                 // priority
    &TASK_HandleOne    // Task handle
  );  
 
  //Define the second thread
  xTaskCreate(
    TASK_TWO,          // Task function
    "TaskTwo",         // Task name
    16*1024,            // Stack size, set as needed
    NULL,              
    1,                 // priority
    &TASK_HandleTwo    // Task handle
  ); 
  

  // For SD card
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  SD.begin(SD_CS);

  // For speaker
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(0);   // 0...21
  Song.sum = songSum;   // Sets the total number of songs.  
  
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

  // FOr ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Start IrReceiver
  IrReceiver.begin(39);
  // Webserver / Controls Function
  startCarServer();
  // The initial I2C controls the motor.
  i2cMotorInit();
  // set the resolution to 12 bits (0-4096)
  analogReadResolution(12);
  // window: 0=left window, 1=right window, 2=voltage window; str: strlen(str)<=40
  displayWindow(0, songs[Song.index]); 
  displayWindow(1, "Ultrasonic: OFF");
  
  audio.connecttoFS(SD, beeps[0]);  
  delay(1000);
}

void loop() {
  //IR Control
  if (IrReceiver.decode()) {
    //Serial.print(F("Command 0x"));
    //Serial.println(IrReceiver.decodedIRData.command, HEX);
    switch(IrReceiver.decodedIRData.command){
      case 0x18:  CarRunForword(99);            break;    //▲
      case 0x52:  CarRunBack(99);               break;    //▼
      case 0x08:  CarTurnLeft(99);              break;    //◀
      case 0x5a:  CarTurnRight(99);             break;    //▶
      case 0x1c:  CarStop();                    break;    //OK
      case 0x16:  ledState = 99-ledState; 
                  LedBrightness(99-ledState);   break;    //*
      case 0x0d:  Song.state = 1-Song.state;    break;    //#
      case 0x19:  music = 0;                    break;    //0    
      case 0x45:  music = 1;                    break;    //1
      case 0x46:  music = 2;                    break;    //2
      case 0x47:  music = 3;                    break;    //3
      case 0x44:  music = 4;                    break;    //4
      case 0x40:  music = 5;                    break;    //5
      case 0x43:  music = 6;                    break;    //6
      case 0x07:  music = 7;                    break;    //7
      case 0x15:  music = 8;                    break;    //8
      case 0x09:  music = 9;                    break;    //9
      default: break;
    }
    IrReceiver.resume();
  }
  if(music > -1){
    audio.stopSong();
    audio.connecttoFS(SD, songs[music]); 
    displayWindow(0, songs[music]); 
    music = -1;  
  }

  //Web app Control
  if(currentVol != App.vol){
    currentVol = App.vol;
    audio.setVolume(currentVol);
  }

  if(Song.index != oldIndex){
    oldIndex = Song.index;
    audio.stopSong();
    displayWindow(0, songs[Song.index]); 
  }

  if(Song.state != oldState){
    oldState = Song.state;
    if(audio.getAudioFileDuration() == 0){       // A full song was played.  
      audio.connecttoFS(SD, songs[Song.index]);  // Start playing music.
      displayWindow(0, songs[Song.index]);  
    }else{
      audio.pauseResume();
    }
  }

  if(App.Pkey == 1 && oldPkey == 0){             // beep key is pressed
    oldPkey = 1;
    audio.stopSong();
    audio.connecttoFS(SD, beeps[2]); 
  }else if(App.Pkey == 0 && oldPkey == 1){
    audio.stopSong();
    oldPkey = 0;
  }

  if(lastTouchActive != touchActive){
    lastTouchActive = touchActive;
    if (touchActive) {
      //Serial.println("  ---- Touch was Pressed");
      audio.stopSong();
      audio.connecttoFS(SD, beeps[1]);
    } else {
      //Serial.println("  ---- Touch was Released");
    }
  }
  // Print the touch value of T2. 
  //Serial.println("T2 pin2 = %d \n", touchRead(T2));
 
  // Note: loop execution of this function 
  // cannot be blocked in the main "loop () "function.
  audio.loop();
}


