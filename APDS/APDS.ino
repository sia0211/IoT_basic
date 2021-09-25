#include <SparkFun_APDS9960.h>

SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting APDS-9960 Gesture");

  if(apds.init()){
    Serial.println("APDS-9960 initialization complete");
  } else{
    Serial.println("Something went wrong during APDS-9960 init!");
  }

  if(apds.enableGestureSensor(true)){
    Serial.println("Gesture sensor is now running");
  } else{
    Serial.println("Something went wrong during gesture sensor init!");
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  handleGesture();
}

void handleGesture(){
  if(apds.isGestureAvailable()){
    switch(apds.readGesture()){
      case DIR_UP:
        Serial.println("UP");
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      default:
        Serial.println("NONE");
    }
  }
}
