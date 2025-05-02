/**
 * @file Main program - main.cpp
 * @brief Environment Monitor System
 * @author Bui Ngoc Long
 * @date 2025-03-17
 */
#include <Arduino.h>
#include "Display\Display.h"
#include "Sensor\Sensor.h"
#include "MQTT\MQTT.h"
#include "Control\Control.h"

// Global variables
float temperature = 0.0;
float humidity = 0.0;
int gasValue = 0;

// Time update the data
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 1000; // Update each 1s

// SETUP Function
void setup() 
{
  Serial.begin(115200);

  // Init OLED
  setupDisplay();

  // Init sensor
  setupSensors();

  // Init control PIN (Led, Buzzer, Button)
  setupControls();

  // Connect to Wi-Fi and MQTT
  setupMQTT();
}
/* LOOP Function - Main */
void loop() 
{
  // Reconnect if disconnect to MQTT
  loopMQTT();

  unsigned long currentTime = millis();

  // Handle button
  handleButton();

  // Update data sensor and send to MQTT every (set time)
  if (currentTime - lastUpdateTime >= updateInterval) {
    // Read data sensor values
    temperature = readTemperature();
    humidity = readHumidity();
    gasValue = readGasValue();
    gasValue = map(gasValue, 0, 4095, 0, 100);
    if(node_id == "node2")
    {
      gasValue += 14;
    }

    // Send data via MQTT
    publishSensorData(temperature, humidity, gasValue);

    // Update data on Oled display
    updateDisplay(temperature, humidity, gasValue);

    lastUpdateTime = currentTime;
  }
}