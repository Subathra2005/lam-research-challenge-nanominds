#include "BluetoothSerial.h"
#include <ESP32Servo.h>

BluetoothSerial SerialBT;

// ---------- Servo Objects ----------
Servo Base, Shoulder, Elbow, Wrist, Gripper;

// Current Angles
int baseAngle = 90, shoulderAngle = 90, elbowAngle = 90, wristAngle = 90, gripperAngle = 60;

// Speed per update
int servoStep = 4;  // Increase for faster movement

// Movement state variables
char baseMove = 'S';
char shoulderMove = 'S';
char elbowMove = 'S';
char wristMove = 'S';
char gripMove = 'S';

void setup() {
  Serial.begin(115200);
  SerialBT.begin("SARM_ESP32");  // Bluetooth Name

  Serial.println("Bluetooth Ready! Connect from App");

  // Attach servos to ESP32 pins
  Base.attach(21);
  Shoulder.attach(22);
  Elbow.attach(23);
  Wrist.attach(32);
  Gripper.attach(33);

  // Move to initial position
  Base.write(baseAngle);
  Shoulder.write(shoulderAngle);
  Elbow.write(elbowAngle);
  Wrist.write(wristAngle);
  Gripper.write(gripperAngle);
}

void loop() {

  if (SerialBT.available() >= 2) {
    char joint = SerialBT.read();  // B,S,E,W,G
    char action = SerialBT.read(); // F,R,S

    switch (joint) {
      case 'B': baseMove = action; break;
      case 'S': shoulderMove = action; break;
      case 'E': elbowMove = action; break;
      case 'W': wristMove = action; break;
      case 'G': gripMove = action; break;
    }
  }

  // ------------------- Continuous Motion Control -------------------

  if (baseMove == 'F') {
    baseAngle = constrain(baseAngle + servoStep, 0, 180);
    Base.write(baseAngle);
  }
  else if (baseMove == 'R') {
    baseAngle = constrain(baseAngle - servoStep, 0, 180);
    Base.write(baseAngle);
  }

  if (shoulderMove == 'F') {
    shoulderAngle = constrain(shoulderAngle + servoStep, 5, 175);
    Shoulder.write(shoulderAngle);
  }
  else if (shoulderMove == 'R') {
    shoulderAngle = constrain(shoulderAngle - servoStep, 5, 175);
    Shoulder.write(shoulderAngle);
  }

  if (elbowMove == 'F') {
    elbowAngle = constrain(elbowAngle + servoStep, 0, 180);
    Elbow.write(elbowAngle);
  }
  else if (elbowMove == 'R') {
    elbowAngle = constrain(elbowAngle - servoStep, 0, 180);
    Elbow.write(elbowAngle);
  }

  if (wristMove == 'F') {
    wristAngle = constrain(wristAngle + 2, 60, 120);
    Wrist.write(wristAngle);
  }
  else if (wristMove == 'R') {
    wristAngle = constrain(wristAngle - 2, 60, 120);
    Wrist.write(wristAngle);
  }

  if (gripMove == 'F') {
    gripperAngle = constrain(gripperAngle + 2, 30, 100);
    Gripper.write(gripperAngle);
  }
  else if (gripMove == 'R') {
    gripperAngle = constrain(gripperAngle - 2, 30, 100);
    Gripper.write(gripperAngle);
  }

  // Smooth + limits overload
  delay(20);
}