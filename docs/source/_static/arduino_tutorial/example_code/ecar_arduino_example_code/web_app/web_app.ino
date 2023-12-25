/*
* Mosiwi eCar - For ESP32
* Boards Manager: esp32 by Espressif Systems, Version 2.0.14  
*/
#include <WiFi.h>
#include "app_server.h"

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

int currentSpeed = 99;
int currentLight = 0;
int currentAservo = 90;
int currentBservo = 90;
int currentVol = 21; 
char currentReturnKey = 0;
char currentPauseKey = 0;
char currentNextKey = 0;
char currentAkey = 0;
char currentSkey = 0;
char currentCkey = 0;
  
void setup(){
  Serial.begin(115200);

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

  // Webserver / Controls Function
  startCarServer();
  delay(1000);

  displayWindow(0, "Left display window");
  displayWindow(1, "right display window");
  displayWindow(2, "P: 100%");
}

void loop() {
  //Web app Control
  if(App.Fkey == 1){             
    Serial.println("F key is pressed!");
  }

  if(App.Bkey == 1){             
    Serial.println("B key is pressed!");
  }

  if(App.Lkey == 1){             
    Serial.println("L key is pressed!");
  }

  if(App.Rkey == 1){             
    Serial.println("R key is pressed!");
  }
    
  if(App.Pkey == 1){             
    Serial.println("P key is pressed!");
  }

  if(currentSpeed != App.speed){
    currentSpeed = App.speed;
    Serial.print("Speed: ");
    Serial.println(App.speed);
  }

  if(currentLight != App.light){
    currentLight = App.light;
    Serial.print("Light: ");
    Serial.println(App.light);
  }

  if(currentAservo != App.Aservo){
    currentAservo = App.Aservo;
    Serial.print("Aservo: ");
    Serial.println(App.Aservo);
  }

  if(currentBservo != App.Bservo){
    currentBservo = App.Bservo;
    Serial.print("Bservo: ");
    Serial.println(App.Bservo);
  }
  
  if(currentVol != App.vol){
    currentVol = App.vol;
    Serial.print("Volume: ");
    Serial.println(App.vol);
  }

  if(currentReturnKey != App.returnKey){
    currentReturnKey = App.returnKey;
    Serial.println("Return key is clicked!");
  }

  if(currentPauseKey != App.pauseKey){
    currentPauseKey = App.pauseKey;
    Serial.println("Pause key is clicked!");
  }

  if(currentNextKey != App.nextKey){
    currentNextKey = App.nextKey;
    Serial.println("Next key is clicked!");
  }

  if(currentAkey != App.Akey){
    currentAkey = App.Akey;
    Serial.println("A key is clicked!");
  }
  
  if(currentSkey != App.Skey){
    currentSkey = App.Skey;
    Serial.println("S key is clicked!");
  }

  if(currentCkey != App.Ckey){
    currentCkey = App.Ckey;
    Serial.println("C key is clicked!");
  }
}


