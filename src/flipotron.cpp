#include "flipotron.h"
#include <Arduino.h>

constexpr int servoForward = 80;
constexpr int servoStop = 90;

void FlipDigit::begin(int servoPin, int switchPin, int forwardSignal) {
  this->servo.attach(servoPin);
  this->switchPin = switchPin;
  pinMode(this->switchPin, INPUT_PULLUP);
}

void FlipDigit::next() {
  this->servo.write(forwardSignal);
  while (digitalRead(this->switchPin) == HIGH) delay(10);
  delay(100);
  this->servo.write(servoStop);
}

void FlipDigit::markZero() { this->value = 0; }

void FlipDigit::set(int newValue) {
  newValue = newValue % maximumValue;

  if (this->value <= newValue) {
    for (int i = 0; i < newValue - this->value; i++) next();
  } else {
    for (int i = 0; i < maximumValue - this->value + newValue; i++) next();
  }
  this->value = newValue;
}

// ----

Flipotron &Flipotron::instance() {
  static Flipotron instance;
  return instance;
}

void Flipotron::begin() {
  unitsDigit.begin(D8, D7, -10);
  tensDigit.begin(D6, D5, 10);
}

void Flipotron::set(int value) {
  value = value % 100;
  unitsDigit.set(value % 10);
  tensDigit.set(value / 10);
}

void Flipotron::markZero() {
  unitsDigit.markZero();
  tensDigit.markZero();
}

void Flipotron::nextTens() { tensDigit.next(); }
void Flipotron::nextUnits() { unitsDigit.next(); }
