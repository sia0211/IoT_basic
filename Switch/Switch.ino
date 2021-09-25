void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  pinMode(15, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(2));
  if(digitalRead(2) == 1)
    digitalWrite(15, LOW);
  else
    digitalWrite(15, HIGH);
  delay(500);
}
