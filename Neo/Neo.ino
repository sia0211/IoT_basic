#include <Adafruit_NeoPixel.h>

#define ledPin 15
#define ledNum 4

Adafruit_NeoPixel pixels;

int shift = 0;
int circle[] = {0, 1, 3, 2};

void setup() {
  // put your setup code here, to run once:
  pixels = Adafruit_NeoPixel(ledNum, ledPin, NEO_GRB + NEO_KHZ800);

  Serial.begin(115200);
  pixels.begin();
  delay(500);
  Serial.println("starting");
}

void loop() {
  // put your main code here, to run repeatedly:
  int R, G, B;
  int i = shift++ % 4;
  if(i == 0){
    R = random(0, 255);
    G = random(0, 255);
    B = random(0, 255);
  }
  for(int i = 0; i < ledNum; i++){
    pixels.setPixelColor(circle[i], pixels.Color(0, 0, 0));
  }
  pixels.setPixelColor(circle[i], pixels.Color(R, G, B));
  pixels.show();
  delay(500);
}
