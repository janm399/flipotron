#include <Arduino.h>
#include "flipotron.h"

StatefulFlipDigit left, right;

void setup() {
  Serial.begin(115200);
  left.begin(18, 19);
  right.begin(15, 17);
}

void loop() {
  left.markZero();
  right.markZero();

  for (int i = 0; i < StatefulFlipDigit::maximumValue; i++) {
    left.next();
    delay(1000);
  }
  right.next();
  delay(1000);
}
