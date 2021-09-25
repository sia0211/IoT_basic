#include <SoftwareSerial.h>

const int relaypin = 15;

SoftwareSerial swSer(13, 12);

void setup() {
  // put your setup code here, to run once:
  swSer.begin(9600);
  pinMode(relaypin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  char transmitdata;
  while(swSer.available()){
    transmitdata = swSer.read();
    if(transmitdata== '1'){
      digitalWrite(relaypin, HIGH);
    }
    else if(transmitdata == '2'){
      digitalWrite(relaypin, LOW);
    }
  }
}
