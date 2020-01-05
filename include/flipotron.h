#pragma once
#include <Servo.h>

/**
 * FlipDigit drives one digit with a servo and an associated flip switch. The
 * servo moves until the switch goes to LOW; its input is configured to be
 * internal pull-up.
 */
class FlipDigit {
 private:
  static constexpr int maximumValue = 10;
  Servo servo;
  int switchPin;
  int value;
  int forwardSignal;

  void next(int factor);

 public:
  void markZero();
  void set(int newValue);

  /**
   * Start the interface, attach the servo and the switch PIN
   * @param servoPin the [output] IO pin for PWM 360 degree servo
   * @param switchPin the [input] IO pin for the flip detection switch
   * @param forward (value from neutral); i.e. +10, +20; or -10 or -20.
   */
  void begin(int servoPin, int switchPin, int forwardSignal);

  /**
   * Move to the next position
   */
  void next();
};

class Flipotron {
 private:
  Flipotron() = default;
  FlipDigit unitsDigit;
  FlipDigit tensDigit;

 public:
  static Flipotron &instance();
  void begin();
  void set(int value);

  void zeroTens();
  void zeroUnits();
};
