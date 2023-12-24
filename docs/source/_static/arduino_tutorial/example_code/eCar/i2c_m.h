#ifndef _I2C_M_H
#define _I2C_M_H
/*
* The I2C speed is 100Khz, and the slave address is 0x2f
* The PWM frequency between the motor and the LED is about 200Hz
*
* Instruction format: 0x2f + device number + data
* Device number: 0=Motor1, 1=Motor2, 2=motor3, 3=motor4, 4=LED1, 5=LED2
* Data: 0-199; 
*       0--99: motor reverse speed/LED brightness, from small to large; 
*       100--199: motor forward speed, from small to large (mapped to 0-99)
*/
#include <Wire.h>
#define I2C_DEV_ADDR  0x2F

// Description: Set motor speed and steering.
// Parameters:  Motor: 0-3 --> M1-M4
//              direction: 0=CCW, 1=CW  
//              speed: 0-99
void SetMotor(char motor, char direction, char speed){
  if(direction < 0 || direction > 1 || motor < 0 || motor > 3){
    return;
  }

  if(speed < 0){ speed = 0; }
  else if(speed > 99){ speed = 99; }

  Wire.beginTransmission(I2C_DEV_ADDR);
  Wire.write(motor);
  if(direction==0){    
    Wire.write(speed);
  }
  if(direction==1){   
    Wire.write(100+speed);
  }
  Wire.endTransmission(true);
}

void CarRunForword(char speed){
  SetMotor(0, 0, speed);  //M1
  SetMotor(1, 0, speed);  //M2
  SetMotor(2, 1, speed);  //M3
  SetMotor(3, 1, speed);  //M4
} 

void CarRunBack(char speed){
  SetMotor(0, 1, speed);  //M1
  SetMotor(1, 1, speed);  //M2
  SetMotor(2, 0, speed);  //M3
  SetMotor(3, 0, speed);  //M4
}

void CarTurnLeft(char speed){
  SetMotor(0, 1, speed);  //M1
  SetMotor(1, 1, speed);  //M2
  SetMotor(2, 1, speed);  //M3
  SetMotor(3, 1, speed);  //M4
}

void CarTurnRight(char speed){
  SetMotor(0, 0, speed);  //M1
  SetMotor(1, 0, speed);  //M2
  SetMotor(2, 0, speed);  //M3
  SetMotor(3, 0, speed);  //M4
}

void CarStop(void){
  for(char motor=0; motor<4; motor++){
    SetMotor(motor, 0, 0);      //M1 -- M4
  }
}

void LedBrightness(char brightness){
  if(brightness < 0 || brightness > 99){
    return;
  }
  for(char led=0; led<2; led++){
    Wire.beginTransmission(I2C_DEV_ADDR);
    Wire.write(led + 4);     
    Wire.write(brightness);   // 0-99
    Wire.endTransmission(true);
  }
}

void i2cMotorInit(void){
  Wire.begin();
  LedBrightness(0);
  CarStop();
}

#endif

