# Restore factory function          
--------------------------
eCar in the factory has burned a multi-function code, and SD card stored in some songs, if you lost them, according to the following method can restore them!             

## Restore factory code         
-----------------------
1. Download bin files: [Click me to download!](../_static/restore_factory_function/bin/ecar_bin.zip)    

2. Install the CH340 driver: <a href="https://docs.mosiwi.com/en/latest/various_resources/ch340/ch340_driver.html" target="_blank">Link</a>
```{tip}
If you've already done this step, you can skip it!           
``` 

3. Download the burning tool:            
ESP official website download tool: <a href="https://www.espressif.com.cn/en/support/download/other-tools" target="_blank">Link1</a>       
![img](../_static/restore_factory_function/img/1img.jpg)   
 The tools we are using: [Link2](../_static/restore_factory_function/tool/flash_download_tool_3.9.6_1.zip)     

4. Connect the PC to the eCar using a Type C USB cable and turn on the power switch of the eCar.            
![img](../_static/restore_factory_function/img/6img.jpg) 

5. Start the burn tool and burn bin files downloaded above into eCar as follows:      
![img](../_static/restore_factory_function/img/5img.jpg)             
![img](../_static/restore_factory_function/img/2img.jpg)   
```{note}
The CH340 driver and proper battery must be installed and the eCar power switch turned on, otherwise the COM port will not be found!                         
``` 

## Recover the songs in the SD card   
-----------------------------------
1. Download the song file and unzip it: [Click me to download!](../_static/music/ecar_music.rar)    
```{note}   
Note that songs are for educational tests only!
```     

2. Copy the music downloaded in the previous step to the root directory of your SD card, as follows:         
![img](../_static/restore_factory_function/img/3img.jpg)              

```{tip}
You need to use the SD card reader!                   
``` 

![img](../_static/restore_factory_function/img/4img.jpg)     

Now insert the SD card into the eCar and start the eCar to remote control it: [Link](../play_ecar/play_ecar.md)

--------
**End!**    



