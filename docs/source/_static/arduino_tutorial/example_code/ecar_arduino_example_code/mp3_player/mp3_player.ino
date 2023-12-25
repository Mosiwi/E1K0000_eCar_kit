/*
* Mosiwi eCar - For ESP32
* Boards Manager: esp32 by Espressif Systems, Version 2.0.14
* Library Manager: ESP32-audioI2S-master by Schreibfaul1, Version 2.0.0
*/
#include "Audio.h"
#include "SD.h"
#include "FS.h"

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
int songIndex = 0;

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

// Set the total number of songs.
int songSum = sizeof(songs)/sizeof(songs[0]);

void setup(){
  Serial.begin(115200);

  // For SD card
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  SD.begin(SD_CS);

  // For speaker
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(21);   // 0...21   
  audio.connecttoFS(SD, songs[songIndex]);     // Start playing music.
  //audio.pauseResume();
  //audio.stopSong();
}

void loop() {
  if(audio.getAudioFileDuration() == 0){       // A full song was played. 
    songIndex ++;
    if(songIndex >= songSum){
      songIndex = 0;
    } 
    audio.connecttoFS(SD, songs[songIndex]);   // Start playing music.
  }
 
  // Note: loop execution of this function 
  // cannot be blocked in the main "loop () "function.
  audio.loop();
}


