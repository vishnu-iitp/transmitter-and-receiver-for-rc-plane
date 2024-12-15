#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define the pins for the joysticks
const int joy1XPin = A0;
const int joy1YPin = A1;
const int joy2XPin = A2;
const int joy2YPin = A3;

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

// Function to read and smooth joystick values
int readJoystick(int pin) {
  const int numReadings = 10;
  int readings[numReadings];
  int total = 0;

  for (int i = 0; i < numReadings; i++) {
    readings[i] = analogRead(pin);
    total += readings[i];
  }

  return total / numReadings;
}

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Initialize the NRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH); // Set power level to high for maximum range
  radio.setDataRate(RF24_250KBPS); // Set data rate to 250kbps for better range
  radio.setAutoAck(false); // Disable acknowledgment
  radio.stopListening();

  // Initialize the joystick pins
  pinMode(joy1XPin, INPUT);
  pinMode(joy1YPin, INPUT);
  pinMode(joy2XPin, INPUT);
  pinMode(joy2YPin, INPUT);
}

void loop() {
  // Read and smooth the joystick values
  data.joy1X = readJoystick(joy1XPin);
  data.joy1Y = readJoystick(joy1YPin);
  data.joy2X = readJoystick(joy2XPin);
  data.joy2Y = readJoystick(joy2YPin);

  // Send the data
  radio.write(&data, sizeof(Data_Package));

  // Debugging: Print the joystick values to the serial monitor
  Serial.print("Joy1 X: ");
  Serial.print(data.joy1X);
  Serial.print(" | Joy1 Y: ");
  Serial.print(data.joy1Y);
  Serial.print(" | Joy2 X: ");
  Serial.print(data.joy2X);
  Serial.print(" | Joy2 Y: ");
  Serial.println(data.joy2Y);

  // Small delay to avoid flooding the serial monitor
  delay(100);
}