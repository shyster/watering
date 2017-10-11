#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClientSecure.h>

WiFiClientSecure client;
ESP8266WebServer server(80);
MDNSResponder mdns;

#define HOST_NAME "watering"

boolean apMode;
//boolean wiFIConnected;

void loopWIFI() {
  server.handleClient();
}

void setupWIFI(boolean ap, char *ssid, char *pass = NULL) {
  if (!ap) {
    apMode = false;
    Serial.println("Set to STA mode");
    WiFi.mode(WIFI_STA);
    Serial.print(WiFi.SSID());
    Serial.printf("\r\n signal %3i dBm\r\n", WiFi.RSSI());
    Serial.println("");
    Serial.print("Works \r\n");
    connectWiFi(ssid, pass);

  } else {
    apMode = true;
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, pass);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("\nAP IP address: ");
    Serial.print(myIP);

    server.on("/", handleRoot);
    server.on("/test", []() {
      server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("\nHTTP server started");
  }
}

boolean isConnectedWiFI(void) {
  return WiFi.status() == WL_CONNECTED;
}

void connectWiFi(char *ssid, char *pass) {
  if (isConnectedWiFI()) {
    return;
  }

  WiFi.begin(ssid, pass);
  Serial.println("WiFi Connecting");
  // Wait for connection
  double timePeriodWaitConnect =  millis() + 30000;
  while (!isConnectedWiFI()) {
    delay(500);
    Serial.print(".");
    if (timePeriodWaitConnect < millis()) {
      Serial.println("Error wifi connect");
      return;
    }
  }

  Serial.println("");
  Serial.println("WiFi Connected: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin(HOST_NAME, WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}



