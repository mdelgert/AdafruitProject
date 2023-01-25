//https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide
//https://randomnerdtutorials.com/esp8266-web-server/
//https://www.mischianti.org/2020/05/16/how-to-create-a-rest-server-on-esp8266-and-esp32-startup-part-1/
//https://mancusoa74.blogspot.com/2018/02/simple-http-rest-server-on-esp8266.html
//https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

//https://microcontrollerslab.com/esp32-rest-api-web-server-get-post-postman/
//#include <WebServer.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

// Load Wi-Fi library
#include <ESP8266WiFi.h>

StaticJsonDocument<250> jsonDocument;
char buffer[250];

// Replace with your network credentials
const char* ssid     = "CHANGEME";
const char* password = "CHANGEME!";

//Get set device statis
bool deviceStatus = false;

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

ESP8266WebServer server(80);
 

void setup() {
  Serial.begin(115200);

  Serial.println("128x64 OLED FeatherWing test");
  display.begin(0x3C, true); // Address 0x3C default

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);

  // Connect to Wi-Fi network with SSID and password

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane esp8266.local
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
 
  // Start server
  server.on("/getstatus", handle_getStatus);
  server.on("/setstatus", handle_setStatus);
  server.begin();
  
  display.println("HTTP server started!");
  
  // Print local IP address and start web server
  display.println("WiFi connected!");
  display.print("SSID: ");
  display.println(ssid);
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display(); // actually display all of the above
  
}

void handle_getStatus() {
  jsonDocument.clear();  
  jsonDocument["status"] = deviceStatus;
  serializeJson(jsonDocument, buffer); 
  server.send(200, "application/json", buffer);
}

void handle_setStatus() {
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  deviceStatus = jsonDocument["status"];
  serializeJson(jsonDocument, buffer); 

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Status: ");
  display.println(deviceStatus);
  display.display();
  
  server.send(200, "application/json", buffer);
}

void loop() {
  delay(10);
  yield();
  display.display();
  server.handleClient();
}
