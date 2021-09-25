#include <SoftwareSerial.h>
SoftwareSerial swSer(13, 12);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  swSer.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  while(swSer.available())
  {
    Serial.write(swSer.read());
  }
  while(Serial.available())
  {
     swSer.write(Serial.read());
  }
}
