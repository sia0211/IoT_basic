#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#define EEPROM_LENGTH 1024
char eRead[30];
byte len;
char ssid[30];
char password[30];
bool captive = true;
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
void GPIO0() {
 SaveString(0, ""); // blank out the SSID field in EEPROM
 ESP.restart();
}

String responseHTML = ""
 "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body><center>"
 "<p>Captive Sample Server App</p>"
 "<form action='/button'>"
 "<p><input type='text' name='ssid' placeholder='SSID' onblur='this.value=removeSpaces(this.value);'></p>"
 "<p><input type='text' name='password' placeholder='WLAN Password'></p>"
 "<p><input type='submit' value='Submit'></p></form>"
 "<p>This is a captive portal example</p></center></body>"
 "<script>function removeSpaces(string) {"
 " return string.split(' ').join('');"
 "}</script></html>";

void setup() {
 Serial.begin(115200);
 EEPROM.begin(EEPROM_LENGTH);
 ReadString(0, 30);
 if (!strcmp(eRead, "")) {
 setup_captive();
 } else {
 captive = false;
 strcpy(ssid, eRead);
 ReadString(30, 30);
 strcpy(password, eRead);
 setup_runtime();
 }
}

void setup_runtime() {
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 Serial.println("");
 // Wait for connection
 int i = 0;
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 if(i++ > 15) {
 captive = true;
 setup_captive();
 return;
 }
 }
 Serial.println("");
 Serial.print("Connected to "); Serial.println(ssid);
 Serial.print("IP address: "); Serial.println(WiFi.localIP());
 if (MDNS.begin("YourNameHere")) {
 Serial.println("MDNS responder started");
 }

  // This is an example of inline function for a handler
 webServer.on("/", [](){
 webServer.send(200, "text/plain", "Working???\n");
 });
 webServer.onNotFound(handleNotFound);
 webServer.begin();
 attachInterrupt(0,GPIO0,FALLING);
 Serial.println("HTTP server started");
}

void setup_captive() {
 WiFi.mode(WIFI_AP);
 WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
 WiFi.softAP("KSH");

 dnsServer.start(DNS_PORT, "*", apIP);
 webServer.on("/button", button);
 webServer.onNotFound([]() {
 webServer.send(200, "text/html", responseHTML);
 });
 webServer.begin();
 Serial.println("Captive Portal Started");
}
void loop() {
 if (captive) {
 dnsServer.processNextRequest();
 }
 webServer.handleClient();
}

void button(){
 Serial.println("button pressed");
 Serial.println(webServer.arg("ssid"));
 SaveString( 0, (webServer.arg("ssid")).c_str());
 SaveString(30, (webServer.arg("password")).c_str());
 webServer.send(200, "text/plain", "OK");
 ESP.restart();
}
// Saves string to EEPROM
void SaveString(int startAt, const char* id) {
 for (byte i = 0; i <= strlen(id); i++) {
 EEPROM.write(i + startAt, (uint8_t) id[i]);
 }
 EEPROM.commit();
}
// Reads string from EEPROM
void ReadString(byte startAt, byte bufor) {
 for (byte i = 0; i <= bufor; i++) {
 eRead[i] = (char)EEPROM.read(i + startAt);
 }
 len = bufor;
}

void handleNotFound(){
 String message = "File Not Found\n\n";
 webServer.send(404, "text/plain", message);
}
