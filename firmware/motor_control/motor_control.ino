#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


#define SERVO_MIN  200 // 'minimum' pulse length count (out of 4096)
#define SERVO_MAX  700 // 'maximum' pulse length count (out of 4096)

#define OPEN        0x4F
#define CLOSED      0x43
#define STOP        0x53

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t servonum = 0;
uint16_t pulselen = SERVO_MIN;

void finger_close(){
  for (pulselen = SERVO_MIN; pulselen < SERVO_MAX; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
    delay(3);
  }
}



void finger_open(){
  for (pulselen = SERVO_MAX; pulselen > SERVO_MIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
    delay(3);
  }
}




void setup(){
  Serial.begin(115200);
  Serial.println("Configuring servo motor");

  pwm.begin(); 
  pwm.begin();
  pwm.setPWMFreq(50);  // Analog servos run at ~60 Hz updates

  delay(10);


  Serial.println("Once connected, enter character(s) that you wish to send");
}


void loop(){
  while (Serial.available()){
    // Delay to wait for enough input, since we have a limited transmission buffer
    uint8_t ch;
    ch = Serial.read();
    if (ch == OPEN){
      finger_open();
      delay(500);
    }
    else if (ch == CLOSED) {
      finger_close();
      delay(500);
    }
    else if (ch == 0x52){
      pwm.setPWM(0, 0, 205);
    }
  }
}
