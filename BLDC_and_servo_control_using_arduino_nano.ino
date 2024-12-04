#include <Servo.h>

int joystickPinY = A0; // Joystick pin for ESC
int joystickPinX = A1; // Joystick pin for servo
int escPin = 3;        // ESC connected to PWM pin
int servoPin = 5;      // Servo connected to PWM pin

Servo escMotor;
Servo servoMotor;

void setup() {
  escMotor.attach(escPin);      // Attach ESC to its pin
  servoMotor.attach(servoPin);  // Attach Servo to its pin
  Serial.begin(9600);           // Start serial monitor for debugging
}

void loop() {
  // Read joystick positions
  int joystickYValue = analogRead(joystickPinY);
  int joystickXValue = analogRead(joystickPinX);

  // Map joystick values
  int pwmYValue = map(joystickYValue, 0, 1023, 1000, 2000); // ESC range
  int pwmXValue = map(joystickXValue, 0, 1023, 0, 180);     // Servo range

  // Update ESC and Servo signals
  escMotor.writeMicroseconds(pwmYValue);
  servoMotor.write(pwmXValue);

  // Debugging outputs
  Serial.print("ESC PWM: ");
  Serial.print(pwmYValue);
  Serial.print(" | Servo Angle: ");
  Serial.println(pwmXValue);

  delay(100); // Small delay to avoid overloading the loop
}
