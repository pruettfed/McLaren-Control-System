#include <Arduino.h>
#include <ArduinoBlue.h>
#include <SoftwareSerial.h>

// Motor pins
const int enablePin = 11; // PWM
const int in1 = 10;
const int in2 = 9;

// HM-10 pins
const int tx = 1;
const int rx = 0;

int throttle, throttleSpeed;

SoftwareSerial bluetooth(tx, rx); // Assign HM-10 Pins
ArduinoBlue phone(bluetooth); // Reference bluetooth object

// Configures the motor controller to stop the motors.
void brake() {
	digitalWrite(enablePin, LOW);
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(enablePin, HIGH);
}

// Configures the motor controller to move forward.
void motorSetForward() {
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
}

// Configures the motor controller to move backwards.
void motorSetBackward() {
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
}

void controlDrive() {
  // throttle values after subtracting 49:
	//     50 = max forward throttle
	//     0 = no throttle
	//     -49 = max reverse throttle

  throttle = phone.getThrottle() - 49;

  if (throttle == 0) {
    brake();
    return;
  }
  else if (throttle > 0) { motorSetForward(); } // Forward
  else { motorSetBackward(); } // Backward

  throttleSpeed = map(abs(throttle), 0, 50, 0, 255); // Map speed from phone -49 to 50 (negative doesn't apply since direction is set before)
}


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
  analogWrite(enablePin, throttleSpeed);

  // Debug
  Serial.print(throttle); Serial.print(" Speed: "); Serial.println(throttleSpeed);
}


/// IN1, IN2 - LOW, LOW = OFF
/// IN1, IN2 - HIGH, HIGH = OFF

/// IN1, IN2 - HIGH, LOW = ON-FORWARD
/// IN1, IN2 - LOW, HIGH = ON-BACKWARD