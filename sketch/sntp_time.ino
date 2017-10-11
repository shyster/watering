#include "SNTPTime.h"
#include "SNTPClock.h"

//======== NTP Begin ==========
//CallBackFunction
#define retriveQuery 300UL
extern SNTPClock Clock;
extern "C" {
#include <sntp.h>
}

void ClockCbFunction()
{
  
  tm cbLocalTimeStruct;
  memcpy((void*)&cbLocalTimeStruct, (const void*)Clock.getTimeStruct(), sizeof(tm));
  //add some time and recalculate cbLocalTimeStruct
  Serial.printf("%s \r\n", Clock.getTimeStr());
  if (cbLocalTimeStruct.tm_year > 0){
    //set time to rtc
    setTimeToRTC(cbLocalTimeStruct.tm_sec, cbLocalTimeStruct.tm_min, cbLocalTimeStruct.tm_hour, cbLocalTimeStruct.tm_mday, cbLocalTimeStruct.tm_mon + MONTH_START, cbLocalTimeStruct.tm_year - 2000);
    addTimeSeconds(&cbLocalTimeStruct, 43200UL); //get time each 12hours
  } else {
    addTimeSeconds(&cbLocalTimeStruct, 300UL);  //get time each 5min
    }
  Clock.attachCb(&cbLocalTimeStruct, (SNTPClock::callback_t)ClockCbFunction);
  // to stop it detach the function
  //Clock.detachCb();
}//ClockCbFunction

int callNTP() {
  return Clock.begin("de.pool.ntp.org", 3, 1);
}

void setupNTP(int timezone) {
  Serial.println("");
  Serial.println("setupNTP");
  sntp_init();
  sntp_setservername(0, (char*)"de.pool.ntp.org");
  sntp_setservername(1, (char*)"time.windows.com");
  sntp_setservername(2, (char*)"time.nist.gov");
  sntp_set_timezone(2);//eeprom_data.timezone

  if (!isConnectedWiFI()) {
    Serial.println("Could not connect to WIFI");
  } else {
     Serial.println("callNTP");
    callNTP();
    ClockCbFunction();   
  }
}

//======== NTP end ============
