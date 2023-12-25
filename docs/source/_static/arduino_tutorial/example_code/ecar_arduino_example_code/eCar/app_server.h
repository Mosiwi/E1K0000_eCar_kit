#ifndef _APP_SERVER_H
#define _APP_SERVER_H
/* 
*  Libraries. If you get errors compiling, please downgrade ESP32 by Espressif.
*  Use version 2.0.14 (Tools, Manage Libraries).
*/
#include "Arduino.h"
#include "esp_http_server.h"
#include "index/index.h"
#include "i2c_m.h"

httpd_handle_t app_httpd = NULL;
#define maxDisplayStorage 40
// It is used to store real-time data for the App.
struct mCarApp{
  // For display window
  char windowSW = 0;
  char *leftWindowStr = "0";
  char *rightWindowStr = "0";
  char *voltWindowStr = "0";

  char Fkey = 0;
  char Bkey = 0;
  char Lkey = 0;
  char Rkey = 0;
  char Pkey = 0;

  int speed = 99;
  int vol = 21;
  int light = 0;
  int Aservo = 90;
  int Bservo = 90;

  char returnKey = 0;
  char pauseKey = 0;
  char nextKey = 0;

  char Akey = 0;
  char Skey = 0;
  char Ckey = 0;
}App;

// For song 
// It is used to store real-time data for the App.
struct mCarSong{
  int sum = 0;        // Total number of songs
  int index = 0;      // The index of the currently playing song
  char state = 0;     // 0: pause, 1: play
}Song;

// Interrupt 
static esp_err_t cmd_handler(httpd_req_t *req){
    char*  buf;
    size_t buf_len;
    char variable[32] = {0,};
    char value[32] = {0,};

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
      buf = (char*)malloc(buf_len);
      if(!buf){
        httpd_resp_send_500(req);
        return ESP_FAIL;
      }
      if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
        if (httpd_query_key_value(buf, "var", variable, sizeof(variable)) != ESP_OK ||
          httpd_query_key_value(buf, "val", value, sizeof(value)) != ESP_OK) {
          free(buf);
          httpd_resp_send_404(req);
          return ESP_FAIL;
        }
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
      free(buf);
    } else {
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }

    int val = atoi(value);
    int res = 0;
    //App Control
    if(!strcmp(variable, "aservo")){        //Aservo slider
      //Serial.println("Aservo");
      App.Aservo = val;
    }else if(!strcmp(variable, "bservo")){  //Bservo slider
      //Serial.println("Bservo");
      App.Bservo = val;
    }else if(!strcmp(variable, "light")){   //Light slider
      //Serial.println("Light");
      App.light = val;
      LedBrightness(val);
    }else if(!strcmp(variable, "speed")){   //Speed slider
      //Serial.println("Speed");       
      App.speed = val;
    }else if(!strcmp(variable, "vol")){     //vol-/+ slider  
      //Serial.println("Vol-/+");    
      App.vol = val;
    }else if(!strcmp(variable, "car")){  
      if (val==1){                  //Fkey
        //Serial.println("Forward");
        App.Fkey = 1;
        CarRunForword(App.speed);
      }else if (val==2){            //Bkey
        //Serial.println("Backward");  
        App.Bkey = 1;  
        CarRunBack(App.speed);  
      }else if (val==3){            //Lkey
        //Serial.println("Lkey");
        App.Lkey = 1;
        CarTurnLeft(App.speed);  
      }else if (val==4){            //Rkey
        //Serial.println("Rkey");
        App.Rkey = 1;
        CarTurnRight(App.speed);   
      }else if (val==5){            //Pkey
        //Serial.println("Pkey"); 
        App.Pkey = 1;   
      }else{                        //None(Stop)
        //Serial.println("None");  
        App.Fkey = 0;
        App.Bkey = 0;
        App.Lkey = 0;
        App.Rkey = 0;
        App.Pkey = 0; 
        CarStop(); 
      } 
    }else if(!strcmp(variable, "music")){
      if (val==6){                  //Return
        //Serial.println("Return"); 
        App.returnKey = 1 - App.returnKey;
        if(Song.index > 0){
          Song.index --;
        }     
      } else if (val==7){           //Pause or play
        //Serial.println("Pause or play");
        App.pauseKey = 1 - App.pauseKey;
        Song.state = 1 - Song.state;     
      } else if (val==8){           //Next
        //Serial.println("Next"); 
        App.nextKey = 1 - App.nextKey;
        if(Song.index < Song.sum-1){
          Song.index ++;
        }     
      }
    }else if(!strcmp(variable, "key")){
      if (val==9){                   //Akey
        //Serial.println("Akey"); 
        App.Akey = 1 - App.Akey;  
      } else if (val==10){           //Skey
        //Serial.println("Skey");
        App.Skey = 1 - App.Skey;     
      } else if (val==11){           //Ckey
        //Serial.println("Ckey"); 
        App.Ckey = 1 - App.Ckey;     
      }
    }else{ 
      //Serial.println("variable");
      res = -1; 
    }

  if(res){ return httpd_resp_send_500(req); }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

// window: 0=left window, 1=right window, 2=voltage window; str: strlen(str)<=40
void displayWindow(char window, char *str){   
  if(strlen(str)>maxDisplayStorage){
    return;
  }
  if(window == 0){
    App.leftWindowStr = str;
  }else if(window == 1){
    App.rightWindowStr = str;
  }else if(window == 2){
    App.voltWindowStr = str;
  }
}

static esp_err_t display_handler(httpd_req_t *req){
  // windowResponse[0] is used to distinguish between left and right display Windows of the web app.
  char windowResponse[maxDisplayStorage + 1] = {};    
  if(App.windowSW == 0){
    windowResponse[0] = 'A'; 
    sprintf(&windowResponse[1], "%s", App.leftWindowStr);
  }
  if(App.windowSW == 1){  
    windowResponse[0] = 'B';
    sprintf(&windowResponse[1], "%s", App.rightWindowStr);
  }
  if(App.windowSW == 2){  
    windowResponse[0] = 'C';
    sprintf(&windowResponse[1], "%s", App.voltWindowStr);
  }
  App.windowSW += 1;  
  if(App.windowSW > 2){
    App.windowSW = 0;    
  }
  httpd_resp_set_type(req, "text/html");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, windowResponse, strlen(windowResponse));
}

static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

// Finally, if all is well with the camera, encoding, and all else, here it is, the actual camera server.
// If it works, use your new camera robot to grab a beer from the fridge using function Request.Fridge("beer","buschlite")
void startCarServer(void){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri       = "/control",
    .method    = HTTP_GET,
    .handler   = cmd_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t display_uri = {
    .uri       = "/display",
    .method    = HTTP_GET,
    .handler   = display_handler,
    .user_ctx  = NULL
  };

  Serial.printf("Starting web server on port: '%d'\n", config.server_port);
  if (httpd_start(&app_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(app_httpd, &index_uri);
    httpd_register_uri_handler(app_httpd, &cmd_uri);
    httpd_register_uri_handler(app_httpd, &display_uri);
  }
}

#endif

