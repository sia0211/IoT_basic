#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

const char* ssid = "1319-2.4G";
const char* password = "kpu123456!";
const char* mqttServer = "54.205.119.151";
const int mqttPort = 1883;
int interval = 5000;
int interval1= 2000;
unsigned long lastPublished = - interval;
/*------------------------------*/
int val = 0;
DHTesp  dht;
unsigned long lastDHTReadMillis = 0;
float humidity =0;
float temperature = 0;
char val1[20]="";
char humidity1[20]="";
char temperature1[20]="";

WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
    }
  Serial.print("Connected to the WiFi network :");
  Serial.println(WiFi.localIP());
  client.setServer(mqttServer, mqttPort);
  //client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client1")) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state "); Serial.println(client.state());
      delay(2000);
      }
    }
  dht.setup(14,DHTesp::DHT22);  
}
void loop() {
  //client.loop();
  readDHT22();
  unsigned long currentMillis = millis();
  val = analogRead(0);
  sprintf (val1, "%2d", val);
  sprintf (humidity1, "%.2f", humidity);
  sprintf (temperature1, "%.2f",temperature);

  client.publish("deviceid/2018146024/evt/light", val1);
  delay(300);
  client.publish("deviceid/2018146024/evt/humidity", humidity1);
  delay(300);
  client.publish("deviceid/2018146024/evt/temperature", temperature1);
  delay(300);
  Serial.print("val");
  Serial.println(val);
  Serial.print("humidity");
  Serial.println(humidity);
  Serial.print("temperature");
  Serial.println(temperature);

}

void readDHT22(){
  unsigned long currentMillis = millis();
  if (currentMillis - lastDHTReadMillis >= interval1){
    lastDHTReadMillis = currentMillis;

    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
    
  }
}
