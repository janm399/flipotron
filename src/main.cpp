#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "flipotron.h"

ESP8266WebServer server;

void handleRoot() {
  server.send(200, "text/html",
              "<a href='/zero?d=1'>Tens</a> | <a href='/zero?d=10>Units</a>");
}

void handleZero() {
  if (server.arg("d") == "1")
    Flipotron::instance().nextUnits();
  else if (server.arg("d") == "10")
    Flipotron::instance().nextUnits();
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hi");
  delay(1000);

  Flipotron::instance().begin();
  WiFi.softAP("Flipotron");
  server.on("/", HTTPMethod::HTTP_GET, handleRoot);
  server.begin(80);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Done");
}

void loop() {
  static int value = 0;
  value++;
  server.handleClient();

  // Flipotron::instance().set(value);
  delay(1000);
}
