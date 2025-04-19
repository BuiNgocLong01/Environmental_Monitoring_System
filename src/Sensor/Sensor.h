/**
 * @file Sensor.h
 * @brief Header file for the MQTT module.
 * @author Bui Ngoc Long
 * @date 2025-04-05
 */

#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_BME280.h>

// Định nghĩa chân cảm biến khí gas
#define GAS_PIN 35

// Khởi tạo cảm biến BME280
extern Adafruit_BME280 bme;

// Hàm khởi tạo cảm biến
void setupSensors();

// Hàm đọc nhiệt độ
float readTemperature();

// Hàm đọc độ ẩm
float readHumidity();

// Hàm đọc giá trị khí gas
int readGasValue();

#endif