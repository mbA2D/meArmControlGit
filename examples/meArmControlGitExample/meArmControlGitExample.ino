#include "meArmControlGit.h"
#include <Servo.h>

#define basePinExample 11
#define shoulderPin 10
#define elbowPin 9
#define gripperPin 6

meArmControlGit arm(basePin, shoulderPin, elbowPin, gripperPin);

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  //arm.moveGripperServo(90);
  //arm.moveBaseServo(90);
  //arm.moveShoulderServo(1500);
  //arm.moveElbowServo(1500);
}

void loop() {
  // put your main code here, to run repeatedly:

}
