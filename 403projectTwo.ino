
#include <WiFi.h>
#include "time.h"

const char* ssid       = "EXAMPLE_SSID";
const char* password   = "EXAMPLE_PASSWORD";

void setup()
{
  Serial.begin(115200);
  /*
   create two tasks;
    one to update the internal time value once every hour
    and another to output time once every 10 miliseconds
  */

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
  const long  gmtOffset_sec = 3600 * (-5);
  const int   daylightOffset_sec = 0;
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop()
{

}

//get time once every hour; runs in an infinite loop
void updateTimeTask( void * parameter){
  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = 3600 * (-5);
  const int   daylightOffset_sec = 0;

  for(;;){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(3600000); 
  }
  
  vTaskDelete( NULL ); 
}

//use the internally stored time and output it to the serial channel; runs in an infinite loop
void printTimeTask( void * parameter){
  for(;;){
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return;
    }
    Serial.println(&timeinfo, "%Y/%m/%d %H:%M:%S");
    delay(10);
  }

  vTaskDelete( NULL );
}
