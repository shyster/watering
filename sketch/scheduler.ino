#include <Ticker.h>

Ticker tick;

struct timeSheduled_t {
  byte hour;
  byte minute;
} timeSheduled;

timeSheduled_t sheduled[5];

void checkScheduler()
{
  byte hour, minute;
  getHourMinutesFromRTC(&hour, &minute);
  Serial.printf("%02i:%02i\r\n", hour, minute);
  displaySec();
  tick.attach(60, checkScheduler);
  // tick.detach();

}

void checkWIFI()
{
  connectWiFi();
  tick.attach(120, checkWIFI);
  // tick.detach();

}

void setupScheduler() {
  checkWIFI();
  checkScheduler();
}

