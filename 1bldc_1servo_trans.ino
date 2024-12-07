#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Create an RF24 object
RF24 radio(9, 10); // CE, CSN pins

// Address for communication
const byte address[6] = "00001";

struct DataPacket {
  int throttleValue; // Throttle for the motor
  int servoValue;    // Servo control value
};

DataPacket data; // Data structure to send

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // Read throttle (A0)
  int potThrottle = analogRead(A0);
  data.throttleValue = map(potThrottle, 0, 1023, 0, 255);

  // Read servo (A1)
  int potServo = analogRead(A1);
  data.servoValue = map(potServo, 0, 1023, 0, 180); // Map to servo angle range (0-180)

  // Send the data packet
  radio.write(&data, sizeof(data));
  
  Serial.print("Sent Throttle: ");
  Serial.print(data.throttleValue);
  Serial.print(", Servo: ");
  Serial.println(data.servoValue);

  delay(100); // Short delay for smooth control
}
