/*
 This example generates a square wave based tone at a specified frequency
 and sample rate. Then outputs the data using the I2S interface to a
 MAX08357 I2S Amp.
 
* Boards Manager: esp32 by Espressif Systems, Version 2.0.14 
*/
#include <I2S.h>
const int frequency = 440;   // frequency of square wave in Hz
const int amplitude = 500;   // amplitude of square wave
const int sampleRate = 8000; // sample rate in Hz
const int bps = 16;

const int halfWavelength = (sampleRate / frequency); // half wavelength of square wave
short sample = amplitude;           // current sample value
int count = 0;

i2s_mode_t mode = I2S_PHILIPS_MODE; // I2S decoder is needed

void setup() {
  Serial.begin(115200);
  Serial.println("I2S simple tone");

  // start I2S at the sample rate with 16-bits per sample
  if (!I2S.begin(mode, sampleRate, bps)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }
}

void loop() {
  if (count % halfWavelength == 0 ) {
    // invert the sample every half wavelength count multiple to generate square wave
    sample = -1 * sample;
  }
  I2S.write(sample); 

  // increment the counter for the next sample
  count++;
}



