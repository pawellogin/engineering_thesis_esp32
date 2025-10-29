#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Client ESP32 Heartbeat Started");
}

void loop()
{
  Serial.println("Client Heartbeat");
  delay(1000);
}
