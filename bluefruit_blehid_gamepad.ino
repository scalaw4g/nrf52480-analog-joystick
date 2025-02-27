#include <bluefruit.h>

BLEDis bledis;
BLEHidGamepad blegamepad;

// defined in hid.h from Adafruit_TinyUSB_Arduino
hid_gamepad_report_t gp;

#define VRX_PIN  A1 // Arduino pin connected to VRX pin
#define VRY_PIN  A2 // Arduino pin connected to VRY pin
#define BAT A3
int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis

const int16_t analogRange = 940;         // analog input range. default: 4096 for 3.3v ADC
const int Xdeadzone = 20;                  // Deadzone percentage
const int Ydeadzone = 20;                  // Deadzone percentage
const int delayBetweenHIDReports = 8;     // Additional delay in milliseconds between HID reports

void setup()
{
  Serial.begin(115200);

#if CFG_DEBUG
  // Blocking wait for connection when debug mode is enabled via IDE
  while ( !Serial ) delay(10);
#endif

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values

  // Configure and Start Device Information Service
  bledis.setManufacturer("scalaw4g");
  bledis.setModel("Seeed XIAO nrf52");
  bledis.begin();

  // Start BLE HID
  blegamepad.begin();

  /* Set connection interval (min, max) to your perferred value.
   * Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
   * min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms
   */
  /* Bluefruit.Periph.setConnInterval(9, 12); */

  // Set up and start advertising
  startAdv();
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_GAMEPAD);

  // Include BLE HID service
  Bluefruit.Advertising.addService(blegamepad);

  // There is enough room for the dev name in the advertising packet
  Bluefruit.Advertising.addName();

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

void loop()
{
  // nothing to do if not connected or
  if ( !Bluefruit.connected() ) return;

      int xReading = analogRead(VRX_PIN);
      int yReading = analogRead(VRY_PIN);
      
// Check for deadzone
      if ( abs(map(xReading, 0, analogRange, -50, 50)) < Xdeadzone ) {
        Serial.println("Deadzone applied for X");
        xReading = analogRange/2;
      }
      if ( abs(map(yReading, 0, analogRange, -50, 50)) < Ydeadzone ) {
        Serial.println("Deadzone applied for Y");
        yReading = analogRange/2;
      }

      //Map joystick value
      int16_t xValue = map(xReading, analogRange, 0, -127, 127);
      int16_t yValue = map(yReading, 0, analogRange, -127, 127);

      // print data to Serial Monitor on Arduino IDE
      Serial.print("X Raw: ");
      Serial.print(xReading);
      Serial.print(", X Input: ");
      Serial.print(xValue);
      Serial.print(", Y Raw: ");
      Serial.print(yReading);
      Serial.print(", Y Input: ");
      Serial.println(yValue);

      // write to Characteristic as byte data
      gp.x = xValue;
      gp.y = yValue;
      blegamepad.report(&gp);
      delay:(delayBetweenHIDReports);
}