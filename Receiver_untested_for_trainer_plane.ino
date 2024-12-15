#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Define the pins for the NRF24L01
const int CE_PIN = 9;
const int CSN_PIN = 10;

// Create an RF24 object
RF24 radio(CE_PIN, CSN_PIN);

// Define the address for the NRF24L01 communication
const byte address[6] = "00001";

// Structure to hold the joystick values
struct Data_Package {
  int joy1X;
  int joy1Y;
  int joy2X;
  int joy2Y;
};

Data_Package data;

// Define the pins for the servos and BLDC motor
const int elevatorServoPin = 3;
const int aileronServo1Pin = 4;
const int aileronServo2Pin = 5;
const int rudderServoPin = 6;
const int bldcMotorPin = 7;

// Create Servo objects
Servo elevatorServo;
Servo aileronServo1;
Servo aileronServo2;
Servo rudderServo;

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Initialize the NRF24L01
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH); // Set power level to high for maximum range
  radio.setDataRate(RF24_250KBPS); // Set data rate to 250kbps for better range
  radio.setAutoAck(false); // Disable acknowledgment
  radio.startListening();

  // Attach the servos to their respective pins
  elevatorServo.attach(elevatorServoPin);
  aileronServo1.attach(aileronServo1Pin);
  aileronServo2.attach(aileronServo2Pin);
  rudderServo.attach(rudderServoPin);

  // Initialize the BLDC motor pin
  pinMode(bldcMotorPin, OUTPUT);
}

void loop() {
  // Check if data is available
  if (radio.available()) {
    // Read the data
    radio.read(&data, sizeof(Data_Package));

    // Debugging: Print the received joystick values to the serial monitor
    Serial.print("Joy1 X: ");
    Serial.print(data.joy1X);
    Serial.print(" | Joy1 Y: ");
    Serial.print(data.joy1Y);
    Serial.print(" | Joy2 X: ");
    Serial.print(data.joy2X);
    Serial.print(" | Joy2 Y: ");
    Serial.println(data.joy2Y);

    // Map the joystick values to servo positions and motor speed
    int elevatorPos = map(data.joy1Y, 0, 1023, 0, 180);
    int aileronPos1 = map(data.joy1X, 0, 1023, 0, 180);
    int aileronPos2 = map(data.joy1X, 0, 1023, 180, 0); // Inverse for the second aileron
    int rudderPos = map(data.joy2X, 0, 1023, 0, 180);
    int motorSpeed = map(data.joy2Y, 0, 1023, 0, 255);

    // Set the servo positions
    elevatorServo.write(elevatorPos);
    aileronServo1.write(aileronPos1);
    aileronServo2.write(aileronPos2);
    rudderServo.write(rudderPos);

    // Set the motor speed
    analogWrite(bldcMotorPin, motorSpeed);
  }
}