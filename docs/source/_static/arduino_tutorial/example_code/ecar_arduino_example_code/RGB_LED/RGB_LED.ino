/*
* Mosiwi eCar - For ESP32
* Boards Manager: esp32 by Espressif Systems, Version 2.0.14
*/
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
char colorFlag = 0;
char ledFlag = 0;

//Create Task2 handle and initialize them.
TaskHandle_t TASK_HandleTwo = NULL;

void TASK_TWO(void *param ){   
  for(;;){
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

    ledFlag ++;
    if(ledFlag >= NR_OF_LEDS){
      ledFlag = 0;
      colorFlag++;
      if(colorFlag >= 3){
        colorFlag = 0;
      }
      switch (colorFlag){
        case 0: color[0]=0xff; color[1]=0; color[2]=0; break;    
        case 1: color[0]=0; color[1]=0xff; color[2]=0; break;   
        case 2: color[0]=0; color[1]=0; color[2]=0xff; break;  
        default: break;
      }                          
    }
    delay(500);
  }
  // When the thread terminates, the thread resource is released.
  //Serial.println("Ending TaskTwo!");
  //vTaskDelete( TASK_HandleTwo );
}

void setup(){
  // For rgb LED
  if ((rmt_send = rmtInit(BUILTIN_RGBLED_PIN, RMT_TX_MODE, RMT_MEM_64)) == NULL){
    Serial.println("init sender failed\n");
  }
  float realTick = rmtSetTick(rmt_send, 100);
  Serial.printf("real tick set to: %fns\n", realTick);

  //Define the second thread
  xTaskCreate(
    TASK_TWO,          // Task function
    "TaskTwo",         // Task name
    16*1024,            // Stack size, set as needed
    NULL,              
    1,                 // priority
    &TASK_HandleTwo    // Task handle
  ); 
}

void loop(){
  /*
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

  ledFlag ++;
  if(ledFlag >= NR_OF_LEDS){
    ledFlag = 0;
    colorFlag++;
    if(colorFlag >= 3){
      colorFlag = 0;
    }
    switch (colorFlag){
      case 0: color[0]=0xff; color[1]=0; color[2]=0; break;    
      case 1: color[0]=0; color[1]=0xff; color[2]=0; break;   
      case 2: color[0]=0; color[1]=0; color[2]=0xff; break;  
      default: break;
    }                          
  }
  delay(500);
  */
}


