# Arduino_tutorial   
------------------

```{note}
The following will override the ESP32 factory default code!     
```   

Tools:     
1. PC(Win10 or uper)     
2. Type C USB cable 
               
## Install the Arduino IDE                  
--------------------------        
<a href="https://docs.mosiwi.com/en/latest/arduino/resources/arduino_ide/arduino_ide.html" target="_blank">Click me</a>      

## Installing libraries                 
-----------------------                           
1. Download the library file and unzip it.   
[Click me to download!](../_static/arduino_tutorial/libraries/ecar_arduino_libraries.rar)      

```{tip}
The library files downloaded above are tested by us, they may not be the latest library files!       
```   

2. Install the library files downloaded above into the Arduino IDE.             
Refer to: <a href="https://docs.mosiwi.com/en/latest/arduino/resources/arduino_libraries/arduino_libraries.html#installing-libraries" target="_blank">Link</a>             
![img](../_static/arduino_tutorial/img/7img.jpg)       

Latest library resources（option）:      
ESP32-audioI2S-master: <https://github.com/schreibfaul1/ESP32-audioI2S>    
ESP32Servo: <https://madhephaestus.github.io/ESP32Servo/annotated.html>    
IRremote: <https://github.com/Arduino-IRremote/Arduino-IRremote>    

## Configure the Arduino IDE for ESP32       
--------------------------------------               
1. Open the arduino IDE，click "File" > "Preferences"，as shown below:      
![img](../_static/arduino_tutorial/img/1img.jpg)    

2. Open the button marked below:   
![img](../_static/arduino_tutorial/img/2img.jpg)    

3. Copy the link：

```
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```  
4. Paste it inside and click OK, as shown below:    
![img](../_static/arduino_tutorial/img/3img.jpg)   

5. Click "Boards Manager":   
![img](../_static/arduino_tutorial/img/4img.jpg)   

6. Find the ESP32 from the pop up Boards Manager and then click install.   
![img](../_static/arduino_tutorial/img/5img.jpg)           
```{tip}
It is recommended to install version 2.0.14, which we are using!            
```  

7. Click "Tools" > "Board" > "esp32" to choose the "ESP32 Dev Module".      
![img](../_static/arduino_tutorial/img/6img.jpg)     

8. ESP32 parameter Settings.     
![img](../_static/arduino_tutorial/img/8img.jpg)      
```{note}
All the following project code needs to be set according to the above parameters, or you will get an error!                   
```     

## Project     
----------     

### Download the example code:       
[Click me to download!](../_static/arduino_tutorial/example_code/ecar_arduino_example_code.zip)    

### Project_1     
LEDs  

### Project_2     
Motors  

### Project_3     
Tuch  

### Project_4     
RGB LED   

### Project_5     
Servo   

### Project_6     
Ultrusonic     

### Project_7     
SD card         

### Project_8     
Speak   

### Project_9     
Wifi   

### Project_10     
eCar    


## Other Resources (option)     
---------------------------        
<a href="https://docs.mosiwi.com/en/latest/arduino/A1D0000_uno_r3/A1D0000_uno_r3.html#arduino-programming-language" target="_blank">Arduino programming language (option)</a>      


--------
**End!**    



