#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Create an RF24 object
RF24 radio(9, 10); // CE, CSN pins

// Address for communication
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int potValue = analogRead(A0); // Read potentiometer value
  int throttleValue = map(potValue, 0, 1023, 0, 255); // Map to 8-bit PWM range
  radio.write(&throttleValue, sizeof(throttleValue)); // Send throttle value
  Serial.print("Sent Throttle: ");
  Serial.println(throttleValue);
  delay(100); // Short delay for smooth control
}
