#include <DHTesp.h>
#include <OLED32.h>

OLED display(4, 5);

DHTesp dht;
int interval = 2000;
unsigned long lastDHTReadMillis = 0;
float humidity = 0;
float temperature = 0;

char Temp[15];
char en[15]; 

const int pulseA = 12;
const int pulseB = 13;
const int pushSW = 2;
volatile int lastEncoded = 0;
volatile long encoderValue = 0;

  
ICACHE_RAM_ATTR void handleRotary(){
  int MSB = digitalRead(pulseA);
  int LSB = digitalRead(pulseB);
  
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    encoderValue++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    encoderValue--;
  lastEncoded = encoded;
  if(encoderValue > 60){
     encoderValue = 60;
  }
  else if(encoderValue < 0){
    encoderValue = 0;
  }
}

ICACHE_RAM_ATTR void buttonClicked(){
  Serial.println("pushed");
}

void setup() 
{   
  display.begin();  
  Serial.begin(115200);
  dht.setup(14, DHTesp::DHT22);       
  pinMode(15, OUTPUT);
  pinMode(pushSW, INPUT_PULLUP);
  pinMode(pulseA, INPUT_PULLUP);
  pinMode(pulseB, INPUT_PULLUP);
  attachInterrupt(pushSW, buttonClicked, FALLING);
  attachInterrupt(pulseA, handleRotary, CHANGE);  
  attachInterrupt(pulseB, handleRotary, CHANGE);
} 

void loop() 
{  
  readDHT22();
  
  Serial.println(encoderValue);     
  sprintf(Temp, "Temp : %.1f", temperature);
  display.print(Temp, 1, 1);
  sprintf(en, "Rotary : %4d", encoderValue);
  display.print(en, 2, 1);
  if(encoderValue > temperature){
    digitalWrite(15, HIGH);
  }
  else{
    digitalWrite(15, LOW);
  }
  delay(100); 
}

void readDHT22(){
  unsigned long currentMillis = millis();
  
  if(currentMillis - lastDHTReadMillis >= interval){
    lastDHTReadMillis = currentMillis;
    
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
  }
}
