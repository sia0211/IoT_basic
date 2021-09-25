#include<OLED32.h>
OLED display(4,5);
char buf[10]; 

int x1 = 0;
float alpha1 = 0.9;

int x2 = 0;
float alpha2 = 0.2;

void setup() 
{   
  display.begin();  
  Serial.begin(115200);       
  pinMode(15, OUTPUT);
} 

void loop() 
{ 
  display.print("Control Relay"); 
  int x0 = analogRead(0);     
  x1 = x1 * alpha1 + (1 - alpha1) * x0;
  x2 = x2 * alpha2 + (1 - alpha2) * x0; 
  Serial.printf("%d %d %d \n", x0, x1, x2);     
  sprintf(buf, "%4d", x1);
  display.print(buf, 2, 1);  
   
  if(x1 < 100)
  {
    digitalWrite(15, HIGH);
    display.print("Relay ON ", 2, 6);
  }
  else
  {
    digitalWrite(15, LOW);
    display.print("Relay OFF", 2, 6);
  }
  delay(100); 
}
