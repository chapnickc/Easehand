#include <Servo.h>

#define SERVO_PIN   9
#define OPEN        0x4F
#define CLOSED      0x43
#define STOP        0x53

Servo servo;

void finger_close(){
  for (int pos = servo.read(); pos <= 180; pos += 1) {
    servo.write(pos);
    delay(15);                    // waits 15ms for the servo to reach the position
  }
}


void finger_open(){
  for (int pos = servo.read(); pos >= 1; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}




void setup(){
  Serial.begin(115200);
  Serial.println("Configuring servo motor");
  servo.attach(SERVO_PIN);  // attaches the pin to the servo object
  Serial.println("Once connected, enter character(s) that you wish to send");
}


void loop(){
  while (Serial.available()){
    // Delay to wait for enough input, since we have a limited transmission buffer
    uint8_t ch;
    ch = Serial.read();
    if (ch == OPEN){
      finger_open();
    }
    else if (ch == CLOSED) {
      finger_close();
    }
    else if (ch == 0x52){
      servo.write(0);
    }
  }
}
