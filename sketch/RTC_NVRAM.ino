#include "RTC_DS1307.h"

RTC_DS1307 rtc;

void setupRTC() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("setupRTC");

  //Wire.begin();

  //byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year
  //Setting the time on DS1307
  //rtc.SetTime( 00, 40, 13, 1, 06, 03, 17 );

  //rtc.SQW( f32768hz );
}

String getTimeString() {
  return rtc.GetTimeToString();
}

void setTimeToRTC(byte second, byte minute, byte hour, byte dayOfMonth, byte month, byte year){
//    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
//  GetTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  //tm *currentTime
  Serial.println("set time");
      Serial.printf("%i-%02i-%02i %02i:%02i:%02i\r\n",
                  year, month, dayOfMonth, hour, minute, second);
   rtc.SetTime(second, minute, hour, dayofweek(year, month, dayOfMonth), dayOfMonth, month, year);
  }

int  dayofweek(int y, byte m, byte d)  /* 1 <= m <= 12,  y > 1752 (in the U.K.) */
    {
        static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
        y -= m < 3;
        return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
    }
void getHourMinutesFromRTC(byte *hour, byte *minute){
  byte second, dayOfWeek, dayOfMonth, month, year;
  rtc.GetTime(&second, minute, hour, &dayOfWeek, &dayOfMonth, &month, &year);
  }    
/*
  void loop() {
  //Read time from DS1307 and display (You must be connected to the serial port to see results)
  rtc.DisplayTime();
  //Convert time to unix timestamp
  long uts = rtc.UnixTimestamp();
  Serial.println( uts );
  delay(1000);
      //Write in NV SRAM
  rtc.WriteRAMStr(0, "http://geek.adachsoft.com/");
  delay(500);

  //Read from NV SRAM
  String str = rtc.ReadRAMStr(0);
  Serial.println("RAM: " +  str);
  }
*/
