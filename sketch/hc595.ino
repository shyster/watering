const int dataPin = 12;   //Outputs the byte to transfer
const int loadPin = 13;   //Controls the internal transference of data in SN74HC595 internal registers
const int clockPin = 15;  //Generates the clock signal to control the transference of data
const byte VALVES_ON = 128;
const byte BLACKLITE = 1;
const byte valves[6] = {2, 4, 8, 16, 32, 64};
byte nextStateValve = 255;
byte stateValve = 0;
byte pressedKey = 0;

void valvesSet(byte number) {
    Serial.print("nextStateValve =");
    Serial.println(number);
  nextStateValve = number;
}

long delayBlacklite;
boolean stateBlacklite = false;
void blackLiteLoop() {
  if (pressedKey > 0) {
    stateBlacklite = true;
    delayBlacklite =  millis() + 10000;
  } else {
    if (delayBlacklite <  millis()) {
      stateBlacklite = false;
    }
  }
}

long delayMainValves;
boolean firstOff = false;
boolean firstOn = false;
void valvesLoop() {
  if (0 == stateValve && nextStateValve > 0 && nextStateValve < 6 && !firstOff) {
//    Serial.print("valveOn -> ");
//    Serial.println(nextStateValve);
    if (!firstOn) {
      Serial.print(millis());
      Serial.print(" mainValveOn ");
      byte mainValveOn = (~valves[nextStateValve - 1] & 126) | (VALVES_ON + (stateBlacklite ? BLACKLITE : 0));
      Serial.println(mainValveOn);
      writeData595(mainValveOn);
      delayMainValves =  millis() + 1000;
      firstOn = true;
      return;
    }

    if (firstOn && delayMainValves <  millis() && nextStateValve > 0 && nextStateValve < 6) {
      Serial.print(millis());
      Serial.print(" ValveOn ");
      byte valveOn = (~(valves[nextStateValve - 1] + valves[5]) & 126) | (VALVES_ON + ( stateBlacklite ? BLACKLITE : 0));
      Serial.println(valveOn);
      writeData595(valveOn);
      stateValve = nextStateValve;
      nextStateValve = 255;
      firstOn = false;
      return;
    }
  }

  if (stateValve > 0 && 0 == nextStateValve && !firstOn) {
    if (!firstOff) {
       Serial.print(millis());
      Serial.print(" mainValveOff ");
      byte mainValveOff = (~valves[stateValve - 1] & 126) | (VALVES_ON + (stateBlacklite ? BLACKLITE : 0));
       Serial.println(mainValveOff);
      writeData595(mainValveOff);
      delayMainValves =  millis() + 1000;
      firstOff = true;
      return;
    }

    if (firstOff && delayMainValves <  millis()) {
       Serial.print(millis());
      Serial.print(" allValveOff ");
      byte valveoff = stateBlacklite ? BLACKLITE : 0;
       Serial.println(valveoff);
      writeData595(valveoff);
      stateValve = 0;
      firstOff = false;
      nextStateValve = 255;
      return;
    }
  }
}

byte lastKey = 0;
int counterPressed = 0;
byte prevKey = 0;
byte getPressedKey() {
  if ((255 != nextStateValve) | firstOn | firstOff) {
    return 0;
  }
  
  if (stateValve > 0) {
    if (isPressed()) {
      if (counterPressed > 1000) {
        counterPressed = 0;
      } else {
        counterPressed++;
      }
      return lastKey;
    } else {
      return 0;
    }
  }

  lastKey++;
  if (lastKey > 5) {
    lastKey = 1;
  }

  byte data = valves[lastKey - 1] + (stateBlacklite ? BLACKLITE : 0);
  writeData595(data);
//  Serial.print("analogRead ->");
//  Serial.println(analogRead(A0));
  if (isPressed()) {
    if (counterPressed > 1000) {
      Serial.print(lastKey);
      Serial.print("-->  ");
      Serial.print(data);
      Serial.print("-->  ");
      Serial.println(analogRead(A0));
      counterPressed = 0;
      return lastKey;
    } else {
      if (prevKey == lastKey) {
        counterPressed++;
      } else {
        counterPressed = 0;
      }
      prevKey = lastKey;
      return lastKey;
    }
  }
  return 0;
}

boolean isPressed(){
  return analogRead(A0) > 1000;
  }
  
void writeData595(byte data) {
  digitalWrite(loadPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(loadPin, HIGH);
}

boolean pressedSetupMode(){
  writeData595(valves[5]);
  boolean setupOn = isPressed();
  writeData595(0);
  return setupOn;
  }

//data = 2;
//digitalWrite(loadPin, LOW);
//shiftOut(dataPin, clockPin, MSBFIRST, data);
//digitalWrite(loadPin, HIGH);
//delay(3000);
//
//int integerData = 3;
//digitalWrite(loadPin, LOW);
//shiftOut(dataPin, clockPin, MSBFIRST, integerData);
//digitalWrite(loadPin, HIGH);
//delay(3000);
//
//byte bitSpecified = 0;
//int bitNumber = 2;
//bitWrite(bitSpecified, bitNumber, HIGH);
//digitalWrite(loadPin, LOW);
//shiftOut(dataPin, clockPin, MSBFIRST, bitSpecified);
//digitalWrite(loadPin, HIGH);
//delay(3000);
//
//  data = 128;
//  writeData(data);
//  display.setCursor(0, 0); // установка цвета текста
//  display.setTextColor(BLACK, WHITE);
//  display.println("Blink");
//  display.display();
//  delay(500);
