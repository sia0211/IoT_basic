#include <DHTesp.h>
DHTesp dht;
String packet;
unsigned long lastSend = 0;
int     interval = 2000;
unsigned long lastDHTReadMillis = 0;


int x0 = 0;
float alpha = .9;

int x01=0;
float alpha1 = .2;
char buf[10];

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

// MQTT 설정

const char* ssid = "1319-2.4G";
const char* password = "kpu123456!";
const char* mqtt_server = "54.208.105.157";
const char* mqtt_topic = "sensor";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


////////////////////////////////////

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
   String msg = "";
  for (int i = 0; i < length; i++) {
    msg +=(char)payload[i];
  }
  Serial.println(msg);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
    dht.setup(14, DHTesp::DHT22);

  //SERIAL BAUDRATE 

  Serial.begin(115200);

// MQTT SETUP 

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void Get_th(){
  unsigned long currentMillis = millis();

  if(currentMillis - lastDHTReadMillis >=interval){
    lastDHTReadMillis = currentMillis;  
  
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  int x1 = analogRead(A0);
  x0 = x0 * alpha + (1 - alpha) *x1;
  x01 = x01 * alpha1 + (1-alpha1) *x1;
  client.publish("deviceid/2018146024/evt/light", (char*) String(x0).c_str());  
  client.publish("deviceid/2018146024/evt/temperature",   (char*) String(temperature).c_str());
  client.publish("deviceid/2018146024/evt/humidity",  (char*) String(humidity).c_str());
  
}
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
    if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    Get_th();
 

    lastSend = millis(); 
  }

  }
