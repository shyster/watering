#include <Wire.h>

boolean setupMode = false;



struct eeprom_data_t {
  char ssid[16];
  char pass[16];
  long timezone;
  int hour;
  int minutes;
  boolean detect;
  int timeWatering;
} eeprom_data1;

void setup() {
  Wire.begin(0, 2);
  Serial.begin(115200);
  pinMode(dataPin, OUTPUT);
  pinMode(loadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(A0, INPUT);
  setupMemory(&eeprom_data1);
  delay(200);
  setupHTU21D();
  setupRTC();
  setupDisplay();

  if (pressedSetupMode()) {
    setupMode = true;
    setupWIFI(setupMode, (char*)HOST_NAME, (char*)"");
    return;
  } 
  
  setupScheduler();
  setupNTP(eeprom_data1.timezone);

}

void connectWiFi() {
  if (isConnectedWiFI()) {
    return;
  }
  setupMode = false;
  setupWIFI(setupMode, eeprom_data1.ssid, eeprom_data1.pass);
}

void loop() {

  if (setupMode) {
    loopWIFI();
    return;
  }
  pressedKey = getPressedKey();
  if (pressedKey > 0) {
    if (0 == stateValve) {
      valvesSet(pressedKey);
    } else {
      Serial.println("valvesSet(0)");
      valvesSet(0);
    }
  }

  blackLiteLoop();
  valvesLoop();
}
