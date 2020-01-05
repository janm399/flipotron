#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "flipotron.h"

ESP8266WebServer server;
static volatile bool go = false;
static volatile int value = 0;

void redirectIndex() {
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void handleRoot() {
  server.send(
      200, "text/html",
      "<a href='/zero?d=1'>Tens</a> | <a href='/zero?d=10'>Units</a><br/>\
              <a href='/go'>Go</a><br/>\
              <form action='/set'><input name='v'/><input type='submit' value='Go'/></form>");
}

void handleZero() {
  Serial.println("zero begin");
  if (server.arg("d") == "1")
    Flipotron::instance().zeroUnits();
  else if (server.arg("d") == "10")
    Flipotron::instance().zeroTens();
  redirectIndex();
  value = 0;
  Serial.println("zero end");
}

void handleSet() {
  if (server.arg("v").toInt() > 0) {
    go = false;
    value = server.arg("v").toInt();
    Flipotron::instance().set(value);
  }
  redirectIndex();
}

void handleGo() {
  go = !go;
  redirectIndex();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hi");
  delay(1000);

  Flipotron::instance().begin();
  WiFi.softAP("Flipotron");
  server.on("/", HTTPMethod::HTTP_GET, handleRoot);
  server.on("/zero", HTTPMethod::HTTP_GET, handleZero);
  server.on("/go", HTTPMethod::HTTP_GET, handleGo);
  server.on("/set", HTTPMethod::HTTP_GET, handleSet);
  server.begin(80);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Done");
}

void loop() {
  server.handleClient();

  if (go) {
    Flipotron::instance().set(value);
    value++;
    delay(1000);
  }
}
