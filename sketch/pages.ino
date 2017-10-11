//======== Pages begin ============
void handleRoot() {
  if (server.args() > 0) {
    eeprom_data1.detect = false;
  }
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "ssid") {
      server.arg(i).toCharArray(eeprom_data1.ssid, server.arg(i).length() + 1);
    }
    if (server.argName(i) == "pass") {
      server.arg(i).toCharArray(eeprom_data1.pass, server.arg(i).length() + 1);
    }
    if (server.argName(i) == "h") {
      eeprom_data1.hour = server.arg(i).toInt();
    }
    if (server.argName(i) == "m") {
      eeprom_data1.minutes = server.arg(i).toInt();
    }
    if (server.argName(i) == "tz") {
      eeprom_data1.timezone = server.arg(i).toInt();
    }
    if (server.argName(i) == "dt" && server.arg(i) == "enable") {
      eeprom_data1.detect = true;
    }
    if (server.argName(i) == "tw") {
      eeprom_data1.timeWatering = server.arg(i).toInt();
    }
    writeSettingsESP(&eeprom_data1);
  }

  String page = "<html><body><table><tbody><form method=\"GET\" action=\"\">";
  page += "<tr><td>Settings</td></tr><tr><td>wifi ssid:</td><td><input type=\"text\" name=\"ssid\" size = 32 value=\"";
  page += eeprom_data1.ssid;
  page += "\"/></td></tr>";
  page += "<tr><td>wifi password:</td><td><input type=\"text\" name=\"pass\" size = 32 value=\"";
  page += eeprom_data1.pass;
  page += "\"/></td></tr>";

  page += "<tr><td>TimeZone:<input type=\"text\" name=\"tz\"/ size = 2 value=\"";
  page += eeprom_data1.timezone;
  page += "\"></td>";
  page += "<td>Time:<input type=\"text\" name=\"h\"/ size = 2 value=\"";
  page += eeprom_data1.hour;
  page += "\">:<input type=\"text\" name=\"m\"/ size = 2 value=\"";
  page += eeprom_data1.minutes;
  page += "\"></td></tr>";

  page += "<tr><td>Time Watering:</td><td><input type=\"text\" name=\"tw\"/ size = 2 value=\"";
  page += eeprom_data1.timeWatering;
  page += "\"></td></tr>";

  page += "<tr><td>Detect:</td><td><input type=\"checkbox\" name=\"dt\"/ value=\"enable\"";
  page += eeprom_data1.detect ? "checked" : "";
  page += "></td></tr>";
  page += "<tr><td> </td><td><input type=\"submit\" value=\"Submit\"/></td></tr>";
  page += "</form></tbody></table><body></html>";
  server.send(200, "text/html", page);
}
