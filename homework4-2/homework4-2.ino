#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

const char* ssid = "AndroidHotspot5049";
const char* password = "cnlove0201";

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password); //붙을 와이파이 주소
  Serial.println("");
  pinMode(2, INPUT);
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

}

void loop() {
  MDNS.update();
  WiFiClient client;
  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  if(digitalRead(2) == LOW){
    if(http.begin(client, "http://192.168.43.46/toggle")){
      Serial.print("[HTTP] GET...\n");
    
      int httpCode = http.GET();
    
      if(httpCode > 0){
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
          String payload = http.getString();
          Serial.println(payload);
        }
      } else{
        Serial.printf("[HTTP} GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    
      http.end();
    }else{
      Serial.print("[HTTP] Unable to connect\n");
    }
  }
}
