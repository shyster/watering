#include "SparkFunHTU21D.h" // set
HTU21D myHumidity;

void setupHTU21D() {
  Serial.println("");
  Serial.println("setupHTU21D");
  myHumidity.begin();
}

float getHumidity() {
  return myHumidity.readHumidity();
}

float getTemperature() {
  return myHumidity.readTemperature();
}
