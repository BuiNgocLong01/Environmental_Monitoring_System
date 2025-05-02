#ifndef MQTT_H
#define MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// #define MQTT_SECURE

// Cấu hình Wi-Fi và MQTT
extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const int mqtt_port;
extern const char* mqtt_user;
extern const char* mqtt_pass;

// Các topic MQTT
extern const char* node_id;
extern String temp_topic;
extern String humi_topic;
extern String gas_topic;
extern String led_control_topic;
extern String buzzer_control_topic;
extern const char* button_topic;

// Khởi tạo client MQTT
extern PubSubClient client;

#ifdef MQTT_SECURE
extern WiFiClientSecure espClient;
#else
extern WiFiClient espClient;
#endif

// Hàm khởi tạo MQTT
void setupMQTT();

// Hàm xử lý vòng lặp MQTT
void loopMQTT();

// Hàm gửi dữ liệu cảm biến qua MQTT
void publishSensorData(float temperature, float humidity, int gasValue);

#endif