#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

int count = 0;
const int relaypin = 15;
const char* ssid = "AndroidHotspot5049";
const char* password = "cnlove0201";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password); //붙을 와이파이 주소
  Serial.println("");
  pinMode(2, INPUT);
  pinMode(relaypin, OUTPUT);

  while(WiFi.status() != WL_CONNECTED) // connected가 아닐동안 돌아라
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("Connected to : ");
  Serial.println(ssid);
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("SN")) {
    Serial.println("MDNS responder started");
  }

  server.on("/on", relayon);
  server.on("/off", relayoff);
  server.on("/toggle", relaytoggle);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  MDNS.update();
  server.handleClient();
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void relayon(){
  digitalWrite(relaypin, HIGH);
  Serial.println("Relay on 상태입니다");
  server.send(200, "text/plain", "Relay on");
}

void relayoff(){
  digitalWrite(relaypin, LOW);
  Serial.println("Relay off 상태입니다");
  server.send(200, "text/plain", "Relay off");
}

void relaytoggle(){
  if(digitalRead(relaypin) == HIGH){
    digitalWrite(relaypin, LOW);
  }
  else if(digitalRead(relaypin) == LOW){
    digitalWrite(relaypin, HIGH);
  } 
  server.send(200, "text/plain", "Toggle");
}
