// include/config.h

#pragma once

// =================================================================
// ===               Project Configuration Management            ===
// =================================================================

#define NODE_ID 1
// #define NODE_ID 2

// WIFI Configuration
#define WIFI_SSID "Ngoc Long"
#define WIFI_PASSWORD "12345679"


// HiveMQ Cloud Configuration
#define HIVEMQ_HOST "1ccb3cb0c5104246a2f3023fc5c5d41e.s1.eu.hivemq.cloud" // HOSTNAME
#define HIVEMQ_PORT 8883
#define HIVEMQ_USER "admin"     
#define HIVEMQ_PASS "Ngoclong@1703" 


// =================================================================
// ===                    System Configuration                   ===
// =================================================================

// Config node
#if NODE_ID == 1
  #define NODE_NAME "node1"
#else
  #define NODE_NAME "node2"
#endif

#define TOPIC_DATA "esp32/" NODE_NAME "/data"
#define TOPIC_STATUS "esp32/" NODE_NAME "/status"
#define TOPIC_BUTTON "esp32/" NODE_NAME "/button"
#define TOPIC_CONTROL_LED "esp32/" NODE_NAME "/control/led"
#define TOPIC_CONTROL_BUZZER "esp32/" NODE_NAME "/control/buzzer"

// --- Config PIN IO ---
// I2C Pins
#define I2C_SDA 21
#define I2C_SCL 22

// Sensor & Actuator Pins
#define GAS_PIN 35
#define LED_PIN 12
#define BUZZER_PIN 26
#define BUTTON_PIN 26

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C