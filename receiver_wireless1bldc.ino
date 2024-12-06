#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Create an RF24 object
RF24 radio(9, 10); // CE, CSN pins

// Address for communication
const byte address[6] = "00001";

const int motorPin = 3; // Pin connected to the BLDC ESC (D3)

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(motorPin, OUTPUT);
  analogWrite(motorPin, 0); // Initialize motor to 0 speed
}

void loop() {
  if (radio.available()) {
    int throttleValue = 0;
    radio.read(&throttleValue, sizeof(throttleValue)); // Receive throttle value
    throttleValue = constrain(throttleValue, 0, 255); // Ensure it's in range
    analogWrite(motorPin, throttleValue); // Set PWM signal to control motor
    Serial.print("Received Throttle: ");
    Serial.println(throttleValue);
  }
}
