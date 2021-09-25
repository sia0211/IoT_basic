#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <DHTesp.h>
#include <ESP8266HTTPClient.h>

#define EEPROM_LENGTH 1024

DHTesp dht;
HTTPClient http;

int interval = 2000;
unsigned long lastDHTReadMillis = 0;
float humidity = 0;
float temperature = 0;

int Light = 0;
float alpha1 = 0.9;
char eRead[30];
byte len;

char ssid[30];
char password[30];

int relaypin = 15;

bool captive = true;
const byte DNS_PORT = 53;
IPAddress apIP(172,30,1,254);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

ICACHE_RAM_ATTR void GPIO0(){
  SaveString(0, "");
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
  pinMode(relaypin, OUTPUT);
  
  ReadString(0, 30);
  if(!strcmp(eRead, "")){
    setup_captive();
  }else{
    captive = false;
    strcpy(ssid, eRead);
    ReadString(30, 30);
    strcpy(password, eRead);
    setup_runtime();
  }
  http.begin("http://172.30.1.25:8086/write?db=mydb");
}

void setup_runtime(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  int i = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    if(i++ > 15){
      captive = true;
      setup_captive();
      return;
    }
  }
  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());

  if(MDNS.begin("YJ")){
    Serial.println("MDNS responder started");
  }

  webServer.on("/", [](){
    webServer.send(200, "text/plain", "Working???\n");
  });
  webServer.on("/on", relayon);
  webServer.on("/off", relayoff);
  
  webServer.onNotFound(handleNotFound);

  webServer.begin();
  attachInterrupt(0, GPIO0, FALLING);
  Serial.println("HTTP server started");
}

void setup_captive(){
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("JYJ");
  
  dnsServer.start(DNS_PORT, "*", apIP);
  
  webServer.on("/button", button);
 
  webServer.onNotFound([](){
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();
  Serial.println("Captive Portal Started");
}

void loop(){
  if(captive){
    dnsServer.processNextRequest();
  }
  webServer.handleClient();

  readDHT22(); //temperature, humidity

  int x0 = analogRead(0);     
  Light = x0 * alpha1 + (1 - alpha1) * Light;  //Light
  

  http.addHeader("Content-Type", "text/plain");
  int httpCode1 = http.POST("show measurements, host=server01, region=us-west value=" + String(temperature));
  int httpCode2 = http.POST("show measurements, host=server02, region=us-west value=" + String(humidity));
  int httpCode3 = http.POST("show measurements, host=server03, region=us-west value=" + String(Light));
  String payload = http.getString();
  Serial.println(httpCode1);
  Serial.println(httpCode2);
  Serial.println(httpCode3);
  Serial.println(payload);
  http.end();

  delay(10000);
}

void button(){
  Serial.println("button pressed");
  Serial.println(webServer.arg("ssid"));
  SaveString(0, (webServer.arg("ssid")).c_str());
  SaveString(30, (webServer.arg("password")).c_str());
  webServer.send(200, "text/plain", "OK");
  ESP.restart();
}

void SaveString(int startAt, const char* id){
  for(byte i = 0; i <= strlen(id); i++){
    EEPROM.write(i + startAt, (uint8_t) id[i]);
   }
   EEPROM.commit();
}

void ReadString(byte startAt, byte bufor){
  for(byte i = 0; i <= bufor; i++){
    eRead[i] = (char)EEPROM.read(i + startAt);
   }
   len = bufor;
}

void relayon(){
  digitalWrite(relaypin, HIGH);
  Serial.println("Relay on 상태입니다");
  webServer.send(200, "text/plain", "Relay on");
}

void relayoff(){
  digitalWrite(relaypin, LOW);
  Serial.println("Relay off 상태입니다");
  webServer.send(200, "text/plain", "Relay off");
}

void readDHT22(){
  unsigned long currentMillis = millis();
  
  if(currentMillis - lastDHTReadMillis >= interval){
    lastDHTReadMillis = currentMillis;
    
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
  }
}
void handleNotFound(){
  String message = "File Not Found\n\n";
  webServer.send(404, "text/plain", message);
}
