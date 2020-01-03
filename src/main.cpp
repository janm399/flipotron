#include <Arduino.h>
#include <ESP32Servo.h>

Servo servo;

void setup() {
  Serial.begin(115200);
  servo.attach(18);
}

void loop() {
  static int angle = 0;
  angle++;
  auto s = Serial.readString();
  if (!s.isEmpty()) {
    Serial.println("Moving to " + s);
    servo.write(s.toInt());
  }
}
