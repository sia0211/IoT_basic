const int numReadings = 10;

int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

int inputPin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for(int thisReading = 0; thisReading < numReadings; thisReading++){
    readings[thisReading] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(inputPin);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if(readIndex >= numReadings){
    readIndex = 0;
  }

  average =total / numReadings;
  Serial.println(average);
  delay(100);
}
