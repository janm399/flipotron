#include "flipotron.h"
#include <Arduino.h>

constexpr int servoForward = 100;
constexpr int servoStop = 90;

void FlipDigit::begin(int servoPin, int switchPin) {
  this->servo.attach(servoPin);
  this->switchPin = switchPin;
  pinMode(this->switchPin, INPUT_PULLUP);
}

void FlipDigit::next() {
  this->servo.write(servoForward);
  while (digitalRead(this->switchPin) == HIGH) delay(1);
  delay(10);
  this->servo.write(servoStop);
}

// ---

void StatefulFlipDigit::markZero() { this->value = 0; }

void StatefulFlipDigit::set(int newValue) {
  newValue = newValue % maximumValue;

  if (this->value <= newValue) {
    for (int i = 0; i < newValue - this->value; i++) next();
  } else {
    for (int i = 0; i < maximumValue - this->value + newValue; i++) next();
  }
  this->value = newValue;
}
