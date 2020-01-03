#pragma once
#include <ESP32Servo.h>

/**
 * FlipDigit drives one digit with a servo and an associated flip switch. The
 * servo moves until the switch goes to LOW; its input is configured to be
 * internal pull-up.
 */
class FlipDigit {
 protected:
  Servo servo;
  int switchPin;

 public:
  /**
   * Start the interface, attach the servo and the switch PIN
   * @param servoPin the [output] IO pin for PWM 360 degree servo
   * @param switchPin the [input] IO pin for the flip detection switch
   */
  void begin(int servoPin, int switchPin);

  /**
   * Move to the next position
   */
  void next();
};

class StatefulFlipDigit : public FlipDigit {
 private:
  int value;

 public:
  void markZero();
  void set(int newValue);

  static constexpr int maximumValue = 15;
};
