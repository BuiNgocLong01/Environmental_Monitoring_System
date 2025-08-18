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

// ===== TIMER =====
#define SAMPLE_MS         3    // đo cảm biến mỗi 3s (không gửi)
#define PERIODIC_MS       15000   // gửi định kỳ mỗi 15s

// ===== ON-EVENT (vượt ngưỡng) =====
#define ALARM_COOLDOWN_MS 5000   // 5s chống nhiễu tại ngưỡng
#define TEMP_HI           35.0f
#define TEMP_LO           15.0f
#define HUMI_HI           85.0f
#define HUMI_LO           30.0f
#define TEMP_HYST         1.0f    // hysteresis: nhả cảnh báo khi <34 hoặc >16
#define GAS_HI            60.0f   // 0 - 1023 --> 0 - 100
#define GAS_HYST          10.0f
#define HUMI_HYST 5

// (tuỳ chọn) Kênh riêng cho on-event (JSON giữ nguyên)
#define TOPIC_ALARM  "esp32/" NODE_NAME "/alarm"


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
#define BUZZER_PIN 16
#define BUTTON_PIN 26

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C