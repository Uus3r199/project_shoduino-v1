#include <Shoduino.h>

// ================= HARDWARE WIRING =================
const int SENSOR_PINS[8] = {A7, A6, A5, A4, A3, A2, A1, A0};
const int EMITTER_PIN = 13;
const int AIN1 = 4, AIN2 = 5, PWMA = 6;
const int BIN1 = 9, BIN2 = 8, PWMB = 7;
const int BUTTON_PIN = 2;
const int BUZZER_PIN = 10;
// ===================================================

// ================= TUNING SETTINGS =================
float kP = 0.15;
float kI = 0;
float kD = 0.15;
int baseSpeed = 60;
// ===================================================

Shoduino robot;

void setup() {
  // Initialize robot
  robot.begin(BUTTON_PIN, BUZZER_PIN);

  // Initialize Sub-systems
  robot.motorLeft.begin(AIN1, AIN2, PWMA);
  robot.motorRight.begin(BIN1, BIN2, PWMB);
  robot.sensors.begin(SENSOR_PINS, EMITTER_PIN);

  // Set PID constants
  robot.pid.setConstants(kP, kI, kD);

  // Smart Calibration
  robot.runCalibration(BUTTON_PIN);

  // Wait to start racing
  robot.waitForButton();
  robot.beep();
  delay(1000);
}

void loop() {
  // Get robot position
  int error = robot.sensors.getPos();

  // Line Lost Recovery
  if (robot.sensors.isOut()) {
    robot.handleRecovery(error);
    return;
  }

  // Get PID corrections
  float correction = robot.pid.output(error);
  int leftSpeed = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;
  robot.motorLeft.setSpeed(baseSpeed + correction);
  robot.motorRight.setSpeed(baseSpeed - correction);
}