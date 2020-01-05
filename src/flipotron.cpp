#include "flipotron.h"
#include <Arduino.h>

constexpr int servoStop = 90;

void FlipDigit::begin(int servoPin, int switchPin, int forwardSignal) {
  this->servo.attach(servoPin);
  this->switchPin = switchPin;
  this->forwardSignal = forwardSignal;
  pinMode(this->switchPin, INPUT_PULLUP);
}

void FlipDigit::next(int factor) {
  auto signal = servoStop + (forwardSignal * factor);
  Serial.println(signal);
  this->servo.write(signal);
  while (digitalRead(this->switchPin) == HIGH) delay(1);
  delay(100 - (factor * 5));
  this->servo.write(servoStop);
}

void FlipDigit::next() { next(1); }

void FlipDigit::markZero() { this->value = 0; }

void FlipDigit::set(int newValue) {
  newValue = newValue % maximumValue;

  int nextCount = 0;
  if (this->value <= newValue) {
    nextCount = newValue - this->value;
  } else {
    nextCount = maximumValue - this->value + newValue;
  }

  if (nextCount == 1)
    next(1);
  else {
    for (int i = 0; i < nextCount / 2; i++) next(2);
    for (int i = 0; i < nextCount / 2; i++) next(1);
  }

  this->value = newValue;
}

// ----

Flipotron &Flipotron::instance() {
  static Flipotron instance;
  return instance;
}

void Flipotron::begin() {
  unitsDigit.begin(D8, D7, -25);
  tensDigit.begin(D6, D5, 25);
}

void Flipotron::set(int value) {
  value = value % 100;
  unitsDigit.set(value % 10);
  tensDigit.set(value / 10);
}

void Flipotron::zeroTens() {
  tensDigit.next();
  unitsDigit.markZero();
}
void Flipotron::zeroUnits() {
  unitsDigit.next();
  tensDigit.markZero();
}
