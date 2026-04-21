// ============================================
//   8 SENSOR ANALOG PID LINE FOLLOWER + ULTRASONIC STOP
//   (Auto Speed Control + Smooth Turning)
// ============================================

// SENSOR PINS
int IR[8] = {A0, A1, A2, A3, A4, A5, A6, A7};

// MOTOR PINS
#define ENA 10
#define IN1 9
#define IN2 8
#define ENB 5
#define IN3 7
#define IN4 6

// ULTRASONIC PINS
#define TRIG 3
#define ECHO 4

// GLOBAL VARIABLES
int sensorVal[8];
int weights[8] = {-4, -3, -2, -1, 1, 2, 3, 4};

float Kd = 10.0;

int lastError = 0;
int baseSpeed = 130;


// ============================================
// MOTOR CONTROL
// ============================================
void setMotor(int left, int right) {

  left = constrain(left, -255, 255);
  right = constrain(right, -255, 255);

  // LEFT MOTOR
  if (left >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, left);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, -left);
  }

  // RIGHT MOTOR
  if (right >= 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, right);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, -right);
  }
}


// ============================================
// ULTRASONIC DISTANCE
// ============================================
long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 25000);
  return duration * 0.034 / 2;
}


// ============================================
// READ ANALOG SENSORS
// ============================================
void readSensors() {
  for (int i = 0; i < 8; i++) {
    sensorVal[i] = analogRead(IR[i]);   // HIGH on black, LOW on white
  }
}


// ============================================
// WEIGHTED ERROR CALCULATION (MOST IMPORTANT PART)
// ============================================
int getError() {
  long weightedSum = 0;
  long total = 0;

  for (int i = 0; i < 8; i++) {

    int val = sensorVal[i];

    // White values < 200 should be ignored
    if (val < 200) val = 0;

    weightedSum += (long)val * weights[i];
    total += val;
  }

  // No line detected
  if (total < 50) return lastError;

  return weightedSum / total;
}


// ============================================
// SETUP
// ============================================
void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 8; i++) pinMode(IR[i], INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  delay(1200);
}


// ============================================
// MAIN LOOP
// ============================================
void loop() {

  // --------- ULTRASONIC STOP ----------
  long dist = getDistance();
  if (dist > 0 && dist <= 15) {
    setMotor(0, 0);
    return;
  }

  // --------- READ SENSORS ----------
  readSensors();
  int error = getError();

  // --------- AUTO SPEED REDUCTION ----------
  int absE = abs(error);

  if (absE >= 3)      baseSpeed = 75;   // sharp turn
  else if (absE == 2) baseSpeed = 85;   // medium turn
  else                baseSpeed = 130;   // straight


  // --------- DYNAMIC KP ----------
  float Kp_base = 30;   // normal
  float Kp_turn = 37;   // strong cornering
  float Kp;

  if (absE >= 3) Kp = Kp_turn;
  else Kp = Kp_base;


  // --------- PID ----------
  float P = Kp * error;
  float D = Kd * (error - lastError);
  int correction = P + D;

  int leftMotor  = baseSpeed - correction;
  int rightMotor = baseSpeed + correction;

  setMotor(leftMotor, rightMotor);

  lastError = error;
}