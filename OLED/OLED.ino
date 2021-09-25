#include <OLED32.h>
OLED display(4,5);

int x1 = 0;
float alpha1 = 0.9;

int x2 = 0;
float alpha2 = 0.2;
char buf[10];

void setup()
{
    Serial.begin(115200);
    display.begin();
}

void loop()
{
    int x0 = analogRead(A0);
    x1 = x1 * alpha1 + (1 - alpha1) * x0;
    x2 = x2 * alpha2 + (1 - alpha2) * x0;
    Serial.printf("%d %d %d \n", x0, x1, x2);
    sprintf(buf, "Bright : %4d", x1);
    display.print(buf,1,1);
    delay(500);
}
