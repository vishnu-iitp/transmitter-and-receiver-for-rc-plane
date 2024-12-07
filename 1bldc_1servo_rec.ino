#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Create an RF24 object
RF24 radio(9, 10); // CE, CSN pins

// Address for communication
const byte address[6] = "00001";

const int motorPin = 3; // Pin connected to the BLDC ESC (D3)
const int servoPin = 4; // Pin connected to the servo (D4)

Servo myServo; // Create a servo object

struct DataPacket {
  int throttleValue; // Throttle for the motor
  int servoValue;    // Servo control value
};

DataPacket data; // Data structure to receive

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(motorPin, OUTPUT);
  analogWrite(motorPin, 0); // Initialize motor to 0 speed

  myServo.attach(servoPin); // Attach servo to D4
  myServo.write(90);        // Initialize servo to center position
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data)); // Receive the data packet

    // Throttle for the motor
    int throttleValue = constrain(data.throttleValue, 0, 255);
    analogWrite(motorPin, throttleValue);

    // Servo control
    int servoValue = constrain(data.servoValue, 0, 180);
    myServo.write(servoValue);

    Serial.print("Received Throttle: ");
    Serial.print(throttleValue);
    Serial.print(", Servo: ");
    Serial.println(servoValue);
  }
}
