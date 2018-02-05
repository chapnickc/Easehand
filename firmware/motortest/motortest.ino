#include <Servo.h>

#define SERVO_PIN 11

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(SERVO_PIN, 750, 2250);  // attaches the servo on pin 9 to the servo object

  /*myservo.write(0);*/
}

void loop() {
/*  myservo.write(180);*/
  /*delay(5000);*/
  /*myservo.write(0);*/
  for (pos = 1; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }


  for (pos = 180; pos >= 1; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
}
