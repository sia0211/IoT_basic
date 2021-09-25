#include <OLED32.h>

OLED display(4, 5);

char bri[15];
char en[15]; 

int x1 = 0;
float alpha1 = 0.9;

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
  if(encoderValue > 255){
     encoderValue = 255;
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
  int x0 = analogRead(0);     
  x1 = x1 * alpha1 + (1 - alpha1) * x0;
  Serial.println(encoderValue);  
     
  sprintf(bri, "Bright : %4d", x1);
  display.print(bri, 1, 1);
  
  sprintf(en, "Rotary : %4d", encoderValue);
  display.print(en, 2, 1);
  
  if(encoderValue > x1){
    digitalWrite(15, HIGH);
  }
  else{
    digitalWrite(15, LOW);
  }
  delay(100); 
}
