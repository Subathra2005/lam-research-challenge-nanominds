#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

// ---------------- PIN CONFIGURATION ----------------

// L298N #1 — LEFT SIDE (Front Left + Rear Left)
#define FL_IN1 25
#define FL_IN2 26
#define FL_EN  27   // PWM pin

#define RL_IN1 32
#define RL_IN2 33
#define RL_EN  14   // PWM pin

// L298N #2 — RIGHT SIDE (Front Right + Rear Right)
#define FR_IN1 18
#define FR_IN2 19
#define FR_EN  21   // PWM pin

#define RR_IN1 22
#define RR_IN2 23
#define RR_EN  5    // PWM pin

int speedVal = 180;  // 0–255 (increase for more power)

// ---------------- MOTOR CONTROL FUNCTIONS ----------------

// LEFT SIDE FORWARD
void LeftMotorsForward() {
  digitalWrite(FL_IN1, HIGH);
  digitalWrite(FL_IN2, LOW);
  digitalWrite(RL_IN1, HIGH);
  digitalWrite(RL_IN2, LOW);

  ledcWrite(FL_EN, speedVal);
  ledcWrite(RL_EN, speedVal);
}

// LEFT SIDE BACKWARD
void LeftMotorsBackward() {
  digitalWrite(FL_IN1, LOW);
  digitalWrite(FL_IN2, HIGH);
  digitalWrite(RL_IN1, LOW);
  digitalWrite(RL_IN2, HIGH);

  ledcWrite(FL_EN, speedVal);
  ledcWrite(RL_EN, speedVal);
}

// RIGHT SIDE FORWARD
void RightMotorsForward() {
  digitalWrite(FR_IN1, HIGH);
  digitalWrite(FR_IN2, LOW);
  digitalWrite(RR_IN1, HIGH);
  digitalWrite(RR_IN2, LOW);

  ledcWrite(FR_EN, speedVal);
  ledcWrite(RR_EN, speedVal);
}

// RIGHT SIDE BACKWARD
void RightMotorsBackward() {
  digitalWrite(FR_IN1, LOW);
  digitalWrite(FR_IN2, HIGH);
  digitalWrite(RR_IN1, LOW);
  digitalWrite(RR_IN2, HIGH);

  ledcWrite(FR_EN, speedVal);
  ledcWrite(RR_EN, speedVal);
}

// STOP ALL
void StopAll() {
  digitalWrite(FL_IN1, LOW);
  digitalWrite(FL_IN2, LOW);
  digitalWrite(RL_IN1, LOW);
  digitalWrite(RL_IN2, LOW);

  digitalWrite(FR_IN1, LOW);
  digitalWrite(FR_IN2, LOW);
  digitalWrite(RR_IN1, LOW);
  digitalWrite(RR_IN2, LOW);

  ledcWrite(FL_EN, 0);
  ledcWrite(RL_EN, 0);
  ledcWrite(FR_EN, 0);
  ledcWrite(RR_EN, 0);
}

void Forward() {
  LeftMotorsForward();
  RightMotorsForward();
}

void Backward() {
  LeftMotorsBackward();
  RightMotorsBackward();
}

void Left() {
  LeftMotorsBackward();
  RightMotorsForward();
}

void Right() {
  LeftMotorsForward();
  RightMotorsBackward();
}

// ---------------- SETUP ----------------

void setup() {
  Serial.begin(115200);
  SerialBT.begin("WHEEL-ESP32-4WD");
  Serial.println("4WD ESP32 Ready!");

  // Set motor direction pins
  pinMode(FL_IN1, OUTPUT);
  pinMode(FL_IN2, OUTPUT);
  pinMode(RL_IN1, OUTPUT);
  pinMode(RL_IN2, OUTPUT);

  pinMode(FR_IN1, OUTPUT);
  pinMode(FR_IN2, OUTPUT);
  pinMode(RR_IN1, OUTPUT);
  pinMode(RR_IN2, OUTPUT);

  // Attach PWM (new API)
  ledcAttach(FL_EN, 1000, 8);  // pin, freq, resolution
  ledcAttach(RL_EN, 1000, 8);
  ledcAttach(FR_EN, 1000, 8);
  ledcAttach(RR_EN, 1000, 8);

  StopAll();
}

// ---------------- MAIN LOOP ----------------

void loop() {
  if (SerialBT.available()) {
    String cmd = SerialBT.readString();
    cmd.trim();

    Serial.println("Received: " + cmd);

    if (cmd == "CF") Forward();
    else if (cmd == "CB") Backward();
    else if (cmd == "CL") Left();
    else if (cmd == "CR") Right();
    else if (cmd == "CS") StopAll();
  }
}
