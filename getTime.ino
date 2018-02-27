#include <WiFi.h>
#include "time.h"

const char* ssid       = "YOUR_SSID";
const char* password   = "YOUR_PASS";

void setup()
{
  Serial.begin(115200);

  xTaskCreate(
    updateTimeTask, /* Task function. */
    "update Time Task", /* name of task. */
    10000, /* Stack size of task */
    NULL, /* parameter of the task */
    1, /* priority of the task */
    NULL);

  xTaskCreate(
    printTimeTask, /* Task function. */
    "print Time Task", /* name of task. */
    10000, /* Stack size of task */
    NULL, /* parameter of the task */
    1, /* priority of the task */
    NULL);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = 3600;
  const int   daylightOffset_sec = 3600;
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{

}

void updateTimeTask( void * parameter){
  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = 3600;
  const int   daylightOffset_sec = 3600;

  for(;;){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(3600000); 
  }
  
  vTaskDelete( NULL ); 
}

void printTimeTask( void * parameter){
  for(;;){
    struct tm* timeinfo;
    time_t timer;
    char buffer[26];

    time(&timer);
    timeinfo = localtime(&timer);

    
    strftime(buffer, 26, "%Y/%m/%d %H:%M:%S", timeinfo);
  }

  vTaskDelete( NULL );
}
