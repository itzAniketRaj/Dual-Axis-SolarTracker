#include <Arduino.h>
#include <Servo.h>

// Servo objects
Servo servoHorizontal;  // Horizontal servo
Servo servoVertical;    // Vertical servo

// Servo angle limits
int servoHorizontalMin = 20;
int servoHorizontalMax = 160;
int servoVerticalMin = 50;
int servoVerticalMax = 110;

// LDR pins
int ldrTopLeft = A1;    // Top left LDR
int ldrTopRight = A2;   // Top right LDR
int ldrBottomLeft = A0; // Bottom left LDR
int ldrBottomRight = A3; // Bottom right LDR

// Variables to hold LDR readings
int ldrTopLeftValue = 0;
int ldrTopRightValue = 0;
int ldrBottomLeftValue = 0;
int ldrBottomRightValue = 0;

void setup() {
  Serial.begin(9600);

  // Attach servos to pins
  servoHorizontal.attach(10);
  servoVertical.attach(9);

  // Initialize servos to their initial positions
  servoHorizontal.write(90); // Center position
  servoVertical.write(90);   // Center position
  delay(500);
}

void loop() {
  // Read analog values from LDRs
  ldrTopLeftValue = analogRead(ldrTopLeft);
  ldrTopRightValue = analogRead(ldrTopRight);
  ldrBottomLeftValue = analogRead(ldrBottomLeft);
  ldrBottomRightValue = analogRead(ldrBottomRight);

  // Calculate averages
  int avgTop = (ldrTopLeftValue + ldrTopRightValue) / 2;
  int avgBottom = (ldrBottomLeftValue + ldrBottomRightValue) / 2;
  int avgLeft = (ldrTopLeftValue + ldrBottomLeftValue) / 2;
  int avgRight = (ldrTopRightValue + ldrBottomRightValue) / 2;

  // Determine vertical servo movement
  if (abs(avgTop - avgBottom) > 10) { // Threshold to ignore small changes
    if (avgTop > avgBottom && servoVertical.read() > servoVerticalMin) {
      servoVertical.write(servoVertical.read() - 2); // Move up (decrease angle)
    } else if (avgBottom > avgTop && servoVertical.read() < servoVerticalMax) {
      servoVertical.write(servoVertical.read() + 2); // Move down (increase angle)
    }
  }

  // Determine horizontal servo movement
  if (abs(avgLeft - avgRight) > 10) { // Threshold to ignore small changes
    if (avgLeft > avgRight && servoHorizontal.read() < servoHorizontalMax) {
      servoHorizontal.write(servoHorizontal.read() + 2); // Move left (increase angle)
    } else if (avgRight > avgLeft && servoHorizontal.read() > servoHorizontalMin) {
      servoHorizontal.write(servoHorizontal.read() - 2); // Move right (decrease angle)
    }
  }

  // Clamp servo positions within limits
  servoHorizontal.write(constrain(servoHorizontal.read(), servoHorizontalMin, servoHorizontalMax));
  servoVertical.write(constrain(servoVertical.read(), servoVerticalMin, servoVerticalMax));

  // Print LDR values to Serial Monitor for debugging
  Serial.print("LDR Top Left: ");
  Serial.print(ldrTopLeftValue);
  Serial.print("\tTop Right: ");
  Serial.print(ldrTopRightValue);
  Serial.print("\tBottom Left: ");
  Serial.print(ldrBottomLeftValue);
  Serial.print("\tBottom Right: ");
  Serial.println(ldrBottomRightValue);

  delay(100); // Delay for smoother servo movements
}