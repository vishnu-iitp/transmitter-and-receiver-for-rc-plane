#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(9, 10); // CE, CSN pins

const byte address[6] = "00001";

struct Data_Package {
  int joystick1X;
  int joystick1Y;
  int joystick2X;
  int joystick2Y;
};

Data_Package data;

Servo servo1;
Servo servo2;

const int bldcPin = 3; // PWM pin for BLDC motor

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH); // Ensure consistent range
  radio.startListening();

  servo1.attach(5); // Servo 1 pin
  servo2.attach(6); // Servo 2 pin

  pinMode(bldcPin, OUTPUT);

  Serial.println("Receiver ready");
}

void loop() {
  if (radio.available()) {
    // Read the incoming data
    radio.read(&data, sizeof(Data_Package));

    // Debugging output
    Serial.print("Joystick1X: ");
    Serial.println(data.joystick1X);
    Serial.print("Joystick1Y: ");
    Serial.println(data.joystick1Y);
    Serial.print("Joystick2X: ");
    Serial.println(data.joystick2X);
    Serial.print("Joystick2Y: ");
    Serial.println(data.joystick2Y);

    // Map and control BLDC motor speed
    int bldcSpeed = map(constrain(data.joystick1Y, 0, 1023), 0, 1023, 0, 255);
    analogWrite(bldcPin, bldcSpeed);

    // Map and control Servo 1 position
    int servo1Pos = map(constrain(data.joystick2X, 0, 1023), 0, 1023, 0, 180);
    servo1.write(servo1Pos);

    // Map and control Servo 2 position
    int servo2Pos = map(constrain(data.joystick2Y, 0, 1023), 0, 1023, 0, 180);
    servo2.write(servo2Pos);

  } else {
    // Print a message if no data is received (with a delay to reduce noise)
    static unsigned long lastMessageTime = 0;
    if (millis() - lastMessageTime > 1000) {
      Serial.println("No data received");
      lastMessageTime = millis();
    }
  }
}
