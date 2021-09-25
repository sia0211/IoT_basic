#include <DHTesp.h>
#include <OLED32.h>

OLED display(4, 5);

DHTesp dht;
int interval = 2000;
unsigned long lastDHTReadMillis = 0;
float humidity = 0;
float temperature = 0;

char Temp[10];
char Humi[10];

void setup() {
  // put your setup code here, to run once:
  display.begin();
  Serial.begin(115200);
  dht.setup(14, DHTesp::DHT22);
  delay(1000);
  Serial.println();
  Serial.println("Humidity (%)\tTemperature (C)");
}

void loop() {
  // put your main code here, to run repeatedly:
  readDHT22();

  Serial.printf("%.1f\t %.1f\n", humidity, temperature);
  
  sprintf(Temp, "Temp : %.1f", temperature);
  display.print(Temp, 1, 1);
  display.print("C", 1, 12);
  
  sprintf(Humi, "Humi : %.1f", humidity);   
  display.print(Humi, 2, 1);
  display.print("%", 2, 12);
  delay(1000);
}

void readDHT22(){
  unsigned long currentMillis = millis();
  
  if(currentMillis - lastDHTReadMillis >= interval){
    lastDHTReadMillis = currentMillis;
    
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
  }
}    
