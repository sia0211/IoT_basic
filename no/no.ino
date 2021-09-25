#include <Servo.h>  
  
Servo myservo;  // create servo object to control a servo  
  
void setup() {  
  Serial.begin(9600);  
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object  
  myservo.write(90);  
  Serial.println("Enter speed (0 - 180, 0 is fast right, 180 is fast left, 90 = stop)");  
}  
  
void loop() {  
  // if there's any serial available, read it:  
  while (Serial.available() > 0) {  
    int iSpeed = Serial.parseInt();  
    iSpeed = constrain(iSpeed, 0, 180);  
    myservo.write(iSpeed);  
    delay(5000);  
    myservo.write(90);  
    Serial.println("Enter speed (0 - 180, 0 is fast right, 180 is fast left, 90 = stop)");  
  } 
} 
