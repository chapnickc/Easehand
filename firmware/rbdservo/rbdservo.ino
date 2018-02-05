#include <RBD_Servo.h>

RBD::Servo servo(11, 1000, 2000); //pin 2, 1ms - 2ms pulse


void setup() {
  /*servo.setPulseInterval(20); //22 ms*/
  servo.moveToDegrees(90);
  servo.update();
}

void loop() {
  servo.moveToDegrees(90);
  delay(500);
  servo.moveToDegrees(0);
  delay(500);
}


