#include <Arduino.h>
#include <ArduinoBlue.h>
#include <SoftwareSerial.h>

/// <=> PIN SETUP <=> ///

// Motor pins
const int enablePin = 11; // PWM
const int in1 = 10;
const int in2 = 9;

// HM-10 pins
const int txPin = 2;
const int rxPin = 4;

/// <=> CONTROL VARIABLES <=> ///
int throttle, throttleSpeed;

SoftwareSerial bluetooth(txPin, rxPin); // Assign HM-10 Pins
ArduinoBlue phone(bluetooth); // Reference bluetooth object

String direction; /// =!= DEBUG =!= ///

/// =>> MOTOR CONTROL <<= ///

void brake() {
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
  digitalWrite(enablePin, LOW); // Turn off
}

void motorSetForward() {
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
}

void motorSetBackward() {
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
}

/// =>> THROTTLE CONTROL <<= ///
void controlDrive() {
  // Throttle values
	// 50 = max forward throttle
	// 0 = no throttle
	// -49 = max reverse throttle

  throttle = phone.getThrottle() - 49;

  if (throttle == 0) { brake(); direction = "STOPPED"; }
  else if (throttle > 0) { motorSetForward(); direction = "FORWARD"; }
  else { motorSetBackward(); direction = "BACKWARD"; }
}

void mapSpeed(int throttleValue) {
  throttleSpeed = map(abs(throttleValue), 0, 50, 0, 255);
  // Map speed from phone -49 to 50 (negative doesn't apply since direction is set before)
}

/// =!= DEBUG =!= ///
void debug() {
  Serial.print("\n\nDirection: "); Serial.println(direction);
  Serial.print("Throttle Value: "); Serial.print(throttle);
  Serial.print("\tSpeed: "); Serial.print(throttleSpeed);
}

/// |||> MAIN FUNCTION <||| ///

void setup() {
  // Set motor control pins to output
  pinMode(enablePin, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Initial state - off
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("Setup Complete");
}

void loop() {
  controlDrive();
  mapSpeed(throttle);

  analogWrite(enablePin, throttleSpeed);

  // =!= DEBUG =!= //
  debug();
}