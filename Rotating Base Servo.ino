#include "Servo.h"

Servo myServo;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(11);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 180; i++){
    myServo.write(i);
    delay(10);
  }
  for(int i = 180; i > 0; i--){
    myServo.write(i);
    delay(10);
  }
}
