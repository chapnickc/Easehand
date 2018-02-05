/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/
#include <bluefruit.h>
#include <Servo.h>

#define SERVO_PIN 7

int OPEN = 0x4F;                          // O in utf8
int CLOSED = 0x43;                        // C in utf8
int pos = 0;                              // variable to store the servo position
int lenMicroSecondsOfPeriod = 20 * 1000; // 20 milliseconds (ms)
int lenMicroSecondsOfPulse = 1.0 * 1000; // 1.0 ms is 0 degrees
float userPulseLen = 0.0;



Servo servo;

// BLE Service
BLEDis  bledis;
BLEUart bleuart;
BLEBas  blebas;

// Software Timer for blinking RED LED
SoftwareTimer blinkTimer;


void setup(){
  Serial.begin(115200);
  Serial.println("Bluefruit52 BLEUART Example");
  Serial.println("---------------------------\n");

  // Initialize blinkTimer for 1000 ms and start it
  blinkTimer.begin(1000, blink_timer_callback);
  blinkTimer.start();

  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(true);

  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Bluefruit52");
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.setConnectCallback(connect_callback);
  Bluefruit.setDisconnectCallback(disconnect_callback);

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();

  Serial.println("Configuring servo motor");
  servo.attach(7, 750, 2200);  // attaches the pin to the servo object
  /*pinMode(SERVO_PIN, OUTPUT);*/

  Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");
  Serial.println("Once connected, enter character(s) that you wish to send");
}

void startAdv(void){
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void finger_close(){
  for (pos = servo.read(); pos <= 180; pos += 1) {
    servo.write(pos);
    delay(15);                    // waits 15ms for the servo to reach the position
  }
}


void finger_open(){
  for (pos = servo.read(); pos >= 1; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}


void run_servo(){
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


void loop(){
  // Forward from BLEUART to HW Serial
  uint8_t ch;
  while ( bleuart.available() ){
    ch = (uint8_t) bleuart.read();
    Serial.write(ch);

    if (ch == 0x4F){
      lenMicroSecondsOfPulse = 1.0 * 1000;
      finger_open();
    }
    else if (ch == 0x43) {
      lenMicroSecondsOfPulse = 2.0 * 1000;
      finger_close();
    }
  }


/*  // Forward data from HW Serial to BLEUART*/
  /*while (Serial.available()){*/
    /*// Delay to wait for enough input, since we have a limited transmission buffer*/
    /*delay(2);*/

    /*uint8_t buf[64];*/
    /*int count = Serial.readBytes(buf, sizeof(buf));*/
    /*bleuart.write( buf, count );*/
/*  }*/

  /*run_servo();*/

  // Request CPU to enter low-power mode until an event/interrupt occurs
  waitForEvent();
}

void connect_callback(uint16_t conn_handle){
  char central_name[32] = { 0 };
  Bluefruit.Gap.getPeerName(conn_handle, central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.println("Disconnected");
}

/**
 * Software Timer callback is invoked via a built-in FreeRTOS thread with
 * minimal stack size. Therefore it should be as simple as possible. If
 * a periodically heavy task is needed, please use Scheduler.startLoop() to
 * create a dedicated task for it.
 * 
 * More information http://www.freertos.org/RTOS-software-timer.html
 */
void blink_timer_callback(TimerHandle_t xTimerID)
{
  (void) xTimerID;
  digitalToggle(LED_RED);
}

/**
 * RTOS Idle callback is automatically invoked by FreeRTOS
 * when there are no active threads. E.g when loop() calls delay() and
 * there is no bluetooth or hw event. This is the ideal place to handle
 * background data.
 * 
 * NOTE: FreeRTOS is configured as tickless idle mode. After this callback
 * is executed, if there is time, freeRTOS kernel will go into low power mode.
 * Therefore waitForEvent() should not be called in this callback.
 * http://www.freertos.org/low-power-tickless-rtos.html
 * 
 * WARNING: This function MUST NOT call any blocking FreeRTOS API 
 * such as delay(), xSemaphoreTake() etc ... for more information
 * http://www.freertos.org/a00016.html
 */
void rtos_idle_callback(void)
{
  // Don't call any other FreeRTOS blocking API()
  // Perform background task(s) here
  /*run_servo();*/
}

