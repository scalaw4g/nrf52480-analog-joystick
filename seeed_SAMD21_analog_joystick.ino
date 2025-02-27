/*  Adafruit 2-axis Mini Joystick controller-style analog input
 *   Uses Adafruit P/N: 2765, 3246
 *   Uses Seeeduino XIAO board
 *   X-axis connected to Pin 10, HIGH towards user
 *   Y-axis connected to Pin 9, HIGH towards USB port
 *   
 *  Author: Glitz
 *  Date: 1-22-2022
 */
#include <HID-Project.h>
#include <HID-Settings.h>
#include "HID.h"

// Pin definitions
const int XPin = 1;
const int YPin = 2;
const int buttonPin = 8;

const int deadzone = 5; // Deadzone percentage
const bool invertX = false; // Inverted X-axis
const bool invertY = false; // Inverted Y-axis

void setup() {
  // Initialize HID as a gamepad
  Gamepad.begin(); // Added this line to initialize the Gamepad instance
  Serial.begin(9600);
}

void loop() {
  int16_t xValue = analogRead(XPin);
  int16_t yValue = analogRead(YPin);
  int16_t xInput = 0;
  int16_t yInput = 0;
  
  // Check for deadzone
  if ( abs(map(xValue, 0, 1023, -50, 50)) < deadzone ) {
    Serial.println("Deadzone applied for X");
    xValue = 512;
  }
  if ( abs(map(yValue, 0, 1023, -50, 50)) < deadzone ) {
    Serial.println("Deadzone applied for Y");
    yValue = 512;
  }

  // Invert axis if configured, and then map to HID values
  if (invertX == true) {
    xInput = map(xValue, 1023, 0, -32768, 32767);
  } else {
    xInput = map(xValue, 0, 1023, -32768, 32767);
  }
    if (invertY == true) {
    yInput = map(yValue, 1023, 0, -32768, 32767);
  } else {
    yInput = map(yValue, 0, 1023, -32768, 32767);
  }
    
  // Send HID gamepad data
  Gamepad.xAxis(xInput);
  Gamepad.yAxis(yInput);
  Gamepad.write();

  // Optional serial output for debugging
  Serial.print("X Raw: ");
  Serial.print(analogRead(XPin));
  Serial.print(", X Input: ");
  Serial.print(xInput);
  Serial.print(", Y Raw: ");
  Serial.print(analogRead(YPin));
  Serial.print(", Y Input: ");
  Serial.println(yInput);
}
