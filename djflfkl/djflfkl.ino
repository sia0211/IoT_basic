#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define relay 15

char buf[10];
const char* ssid = "1319-2.4G";
const char* password = "kpu123456!";
const char* mqttServer = "54.205.119.151"; //자신의 서버 주소
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(relay, OUTPUT);
   Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connectiong to WiFi...");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while(!client.connected()){
    Serial.println("Connectiong to MQTT..");

    if(client.connect("ESP8266Client")){
      Serial.println("connected");
    } else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
  }
  client.subscribe("deviceid/2016146013/evt/lamp"); // 읽어드릴 토픽

}

void loop() {
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
  Serial.print("Message:");
  for(int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
    buf[i] = (char)payload[i];
  }
  Serial.println("-------------------------");

  if(strcmp(buf, "on") == 0){
    digitalWrite(relay, HIGH);
  }
  else if(strcmp(buf, "off") == 0){
    digitalWrite(relay, LOW);
  }
}
