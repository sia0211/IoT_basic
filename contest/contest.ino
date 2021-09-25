#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const int red = 12;
const int green = 13;
const char* ssid = "AndroidHotspot5049";
//const char* password = "zxcasdqwe1";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid); //붙을 와이파이 주소
  Serial.println("");
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

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

  server.on("/",handleon);
  server.on("/red", redon);
  server.on("/green", greenon);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  MDNS.update();
  server.handleClient();
}

void handleon(){
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  String message = "<html><head><meta charset=\"utf-8\">"
                   "<meta http-equiv='refresh' content='5'/>"
                   "<title>신호등</title></head>"
                   "<body>"
                   "<script></script>"
                   "<center><p>"
                   "<head>신호등 제어를 시작하겠습니다</head>"
                   "</center>"
 "</body></html>";
 
  server.send(200, "text/html", message);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}  

void redon(){
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
  String message = "<html><head><meta charset=\"utf-8\">"
                   "<meta http-equiv='refresh' content='5'/>"
                   "<title>빨간불</title></head>"
                   "<body>"
                   "<script></script>"
                   "<center><p>"
                   "<head>차량 멈추세요</head>"
                   "</center>"
 "</body></html>";
  server.send(200, "text/html", message);
}

void greenon(){
  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
  String message = "<html><head><meta charset=\"utf-8\">"
                   "<meta http-equiv='refresh' content='5'/>"
                   "<title>녹색불</title></head>"
                   "<body>"
                   "<script></script>"
                   "<center><p>"
                   "<head>차량 출발하세요</head>"
                   "</center>"
 "</body></html>";
  server.send(200, "text/html", message);
}
