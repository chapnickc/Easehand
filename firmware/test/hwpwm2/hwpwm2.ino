#include <Arduino.h>


void setup(){
  Serial.begin(115200);
  HwPWMx[0]->addPin(A0);
  HwPWMx[0]->addPin(LED_RED);

  // Enable PWM0 module with 15-bit resolutions (max)
  HwPWM0.begin();
  HwPWM0.setResolution(15);
  HwPWM0.setClockDiv(PWM_PRESCALER_PRESCALER_DIV_1); // default : freq = 16Mhz
}


void loop(){
  const int maxValue = bit(15) - 1;
  bool inverted;

  // 90 deg
  HwPWMx[0]->writePin(A0, 20000, 1);
  delay(1000);

  // -90 deg
  HwPWMx[0]->writePin(A0, 1024, 1);
  delay(5000);

}
