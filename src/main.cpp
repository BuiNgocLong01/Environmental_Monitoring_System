#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "config.h"
#include "Display/Display.h"
#include "Sensor/Sensor.h"
#include "MQTT/MQTT.h" 
#include "Control/Control.h"
#include <WiFi.h>

const unsigned long updateInterval = 5000; // Gửi dữ liệu mỗi 5 giây
unsigned long lastUpdateTime = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\nBooting Node: " NODE_NAME);

    Wire.begin(I2C_SDA, I2C_SCL);

    setupDisplay();
    setupSensors();
    setupControls();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    setupMQTT();
}

void loop() {
    loopMQTT();

    if (handleButton()) {
        publishButtonPress();
    }

    if (millis() - lastUpdateTime >= updateInterval) 
    {
        float temperature = readTemperature();
        float humidity = readHumidity();
        int gasValue = readGasValue();
        gasValue = map(gasValue, 0, 4095, 0, 100);
        if(NODE_NAME == "node1")
        {
          gasValue += 15;
        }

        // Update display
        updateDisplay(temperature, humidity, gasValue);

        StaticJsonDocument<200> doc;
        doc["node"] = NODE_NAME;
        doc["temp"] = round(temperature * 10) / 10.0;
        doc["humi"] = round(humidity * 10) / 10.0;
        doc["gas"] = gasValue;

        char jsonBuffer[200];
        serializeJson(doc, jsonBuffer);

        publishJsonData(jsonBuffer);
        Serial.printf("Published data: %s\n", jsonBuffer);

        lastUpdateTime = millis();
    }
}