#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

const char* ssid = "1319-2.4G";
const char* password = "kpu123456!";
const char* mqttServer = "54.205.119.151"; //자신의 서버 주소
const int mqttPort = 1883;

int interval = 10000;
unsigned long lastPublished = -interval;
unsigned long lastDHTReadMillis = 0;


char temp[10];
char humi[10];
char light[10];

float humidity = 0;
float temperature = 0;

int Light = 0;
float alpha1 = 0.9;

DHTesp dht;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  dht.setup(14, DHTesp::DHT22);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connectiong to WiFi...");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);

  while(!client.connected()){
    Serial.println("Connectiong to MQTT..");

    if(client.connect("ESP8266Client")){
      Serial.println("connected");
    } else{
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
    }
  }
  
}

void loop() {
  client.loop();
  
  unsigned long currentMillis = millis();
  if(currentMillis - lastPublished >= interval){
    readsensor();
    lastPublished = currentMillis;
  }
}

void readsensor(){
  unsigned long currentMillis = millis();

  if(currentMillis - lastDHTReadMillis >=interval){
    lastDHTReadMillis = currentMillis;  
  
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    int x0 = analogRead(A0);
    Light = x0 * alpha1 + (1 - alpha1) * Light;

    sprintf(temp, "%f", temperature);
    sprintf(humi, "%f", humidity);
    sprintf(light, "%d", Light);

    client.publish("deviceid/2016146013/evt/light", temp);  
    client.publish("deviceid/2016146013/evt/temperature", humi);
    client.publish("deviceid/2016146013/evt/humidity", light);
  }
}
