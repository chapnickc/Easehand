// Manual servo control 
// Copyright 2012 by Zipwhip. 
// You are free to use and modify this code in your own software.

#define SERVO_PIN       7  // Any pin on the Arduino or Mega will work.

int lenMicroSecondsOfPeriod = 20 * 1000; // 20 milliseconds (ms)
int lenMicroSecondsOfPulse = 1.0 * 1000; // 1.0 ms is 0 degrees
float userPulseLen = 0.0;
String readString;

void setup()
{
  pinMode(SERVO_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("serial servo incremental test code");
  Serial.println("Set the pulse width in milliseconds");
  Serial.println("use strings like 1.0 or 2.0 for new servo position");
  Serial.println();
}


void loop()
{

  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    userPulseLen = readString.toFloat();
    lenMicroSecondsOfPulse = userPulseLen*1000;

    Serial.println(lenMicroSecondsOfPulse);
  }
  readString="";

 // Servos work by sending a 20 ms pulse.  
 // 1.0 ms at the start of the pulse will turn the servo to the 0 degree position
 // 1.5 ms at the start of the pulse will turn the servo to the 90 degree position 
 // 2.0 ms at the start of the pulse will turn the servo to the 180 degree position 
 // Turn voltage high to start the period and pulse
 digitalWrite(SERVO_PIN, HIGH);

 // Delay for the length of the pulse
 delayMicroseconds(lenMicroSecondsOfPulse);

 // Turn the voltage low for the remainder of the pulse
 digitalWrite(SERVO_PIN, LOW);

 // Delay this loop for the remainder of the period so we don't
 // send the next signal too soon or too late
 delayMicroseconds(lenMicroSecondsOfPeriod - lenMicroSecondsOfPulse); 

}
