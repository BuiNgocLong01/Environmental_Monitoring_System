/**
 * @file Sensor.h
 * @brief Header file for the MQTT module.
 * @author Bui Ngoc Long
 * @date 2025-04-05
 */

#ifndef SENSOR_H
#define SENSOR_H

void setupSensors();
float readTemperature();
float readHumidity();
int readGasValue();

#endif