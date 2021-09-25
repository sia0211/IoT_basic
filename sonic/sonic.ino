#include <OLED32.h>

OLED display(4, 5);

const int trigPin = 13;
const int echoPin = 12;

long duration;
float distance;

char dur[15];

void setup() {
  // put your setup code here, to run once:
  display.begin();
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(200);
  Serial.println("starting...");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  Serial.print("Duration : ");
  Serial.println(duration);

  sprintf(dur, "%.1f", duration);
  display.print("Duration", 1, 1);
  display.print(dur, 2, 1);
  display.print("cm", 2, 6);

  distance = duration * 0.017;

  Serial.print(distance);
  Serial.println("cm");

  delay(1000);
}
