#include <SoftwareSerial.h>

SoftwareSerial swSer(13, 12);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  swSer.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char transmitdata;
 
  while(Serial.available()){
    transmitdata = Serial.read();
    swSer.write(transmitdata);
    if(transmitdata == '1'){
      Serial.println("relay on");
    }
    else if(transmitdata == '2'){
      Serial.println("relay off");
    }
  }
}
