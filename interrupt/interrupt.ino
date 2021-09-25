int count = 0;
int pin = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  Serial.println("Starting");
  pinMode(pin, INPUT);
  attachInterrupt(pin, ISR00, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Count : ");
  Serial.println(count);
  delay(100);
}

ICACHE_RAM_ATTR void ISR00()
{
   count++;
}
