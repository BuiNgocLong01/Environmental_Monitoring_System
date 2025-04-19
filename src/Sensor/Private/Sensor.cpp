#include "Sensor\Sensor.h"

// Khởi tạo cảm biến BME280
Adafruit_BME280 bme;

void setupSensors() {
  // Khởi tạo cảm biến BME280
  if (!bme.begin(0x76)) {
    Serial.println(F("Could not find a valid BME280 sensor!"));
    for (;;);
  }
}

float readTemperature() {
  return bme.readTemperature();
}

float readHumidity() {
  return bme.readHumidity();
}

int readGasValue() {
  return analogRead(GAS_PIN);
}