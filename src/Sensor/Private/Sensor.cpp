#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Sensor/Sensor.h"
#include "config.h"

Adafruit_BME280 bme;

void setupSensors() {
    if (!bme.begin(0x76, &Wire)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    pinMode(GAS_PIN, INPUT);
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