#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "flipotron.h"

#define WITHCLIENT 0

ESP8266WebServer server;
WiFiClientSecure client;

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
              <form action='/set'><input name='v'/><input type='submit' value='Go'/></form>\
              <br/><br/><br/><br/>\
              <a href='/off'>OFF</a>");
}

void handleZero() {
  if (server.arg("d") == "1")
    Flipotron::instance().zeroUnits();
  else if (server.arg("d") == "10")
    Flipotron::instance().zeroTens();
  redirectIndex();
  value = 0;
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

void handleOff() { ESP.deepSleep(0, RF_DISABLED); }

void logSetup() {
  Serial.begin(115200);
  while (!Serial)
    ;
}

void hardwareSetup() {
  // Core Flipotron
  pinMode(LED_BUILTIN, OUTPUT);
  Flipotron::instance().begin();
}

void wifiSetup() {
// WiFi & Local AP if needed
#if WITHCLIENT
  WiFi.begin("EspressoMate", "qwertyuiopoiuytrewq");
  WiFi.waitForConnectResult();
  if (WiFi.status() != WL_CONNECTED) {
#endif
    WiFi.softAP("Flipotron Debug");
    Serial.println(WiFi.status());
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
    server.on("/", HTTPMethod::HTTP_GET, handleRoot);
    server.on("/zero", HTTPMethod::HTTP_GET, handleZero);
    server.on("/go", HTTPMethod::HTTP_GET, handleGo);
    server.on("/set", HTTPMethod::HTTP_GET, handleSet);
    server.on("/off", HTTPMethod::HTTP_GET, handleOff);
    server.begin(80);
#if WITHCLIENT
  } else {
    Serial.println("Connected to EspressoMate = ");
    Serial.println(WiFi.localIP());
  }
#endif
}

void setup() {
  logSetup();
  hardwareSetup();
  wifiSetup();
  Serial.println("Flipotron Ready.");
}

void sleep() {
  Serial.println("Sleep enter");
  WiFi.forceSleepBegin();
  delay(60000);
  WiFi.forceSleepWake();
  Serial.println("Sleep leave");
}

void loop() {
#if WITHCLIENT
  static String host = "raw.githubusercontent.com";
  static String url = "/janm399/flipotron/master/display.txt";

  if (WiFi.softAPgetStationNum() == 0) {
    client.setInsecure();
    if (WiFi.status() == WL_CONNECTED && client.connect(host, 443)) {
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host +
                   "\r\n" + "User-Agent: Flipotron\r\n" +
                   "Connection: close\r\n\r\n");
      String body = client.readString();
      int bodyIndex = body.lastIndexOf("\r\n\r\n");
      Serial.println("Raw body >>>");
      Serial.println(body);
      Serial.println("<<<");
      if (bodyIndex != -1) {
        body = body.substring(bodyIndex);
        body.trim();
        int receivedValue = body.toInt();
        Serial.println("Body>>>");
        Serial.println(body);
        Serial.println("<<<");
        Serial.println(receivedValue);
        Flipotron::instance().set(receivedValue);
      } else
        Serial.println(body);
    } else {
      Serial.println("Not connected");
    }
    sleep();
  } else
#endif
  {
    server.handleClient();
    digitalWrite(LED_BUILTIN, millis() % 1000 > 500);
    delay(100);

    if (go) {
      Flipotron::instance().set(value);
      value++;
      delay(1000);
    }
  }
}
