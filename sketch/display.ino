#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
/******************************************************************
  == Connection ==

  USB TTL     Huzzah      Nokia 5110  Description
            ESP8266     PCD8544

            GND         GND         Ground
            3V          VCC         3.3V from Huzzah to display
            14-0          CLK         Output from ESP SPI clock
            13-1(tx)          DIN         Output from ESP SPI MOSI to display data input
            12-2          D/C         Output from display data/command to ESP
            #5-3(rx)          CS          Output from ESP to chip select/enable display
            #4          RST         Output from ESP to reset display
                        LED         3.3V to turn backlight on

******************************************************************/
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 13, 12, 5, 4);//14, 13, 12, 5, 4
String state = "";
void displaySec() {

  float humd = getHumidity();
  float temp = getTemperature();
  state = "H ";
  state += humd;
  state += "%,T ";
  state += temp;
  state += "C\n";
  state += getTimeString();

  display.clearDisplay();
  display.setCursor(0, 0); // установка цвета текста
  display.setTextColor(BLACK, WHITE);
  display.println(state);
  display.display();
}

void setupDisplay() {
  Serial.println("");
  Serial.println("setupDisplay");
  ////display
  display.begin();
  display.clearDisplay();
  display.display();

  display.setContrast(50); // установка контраста
  display.setTextSize(0);  // установка размера шрифта
  display.setTextColor(BLACK); // установка цвета текста
  display.setCursor(0, 0);

  display.println("Hello, world!");
  display.display();
  //end display

}
