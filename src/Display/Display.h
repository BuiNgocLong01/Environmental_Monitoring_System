/**
 * @file Display.h
 * @brief Display header file
 * @author Bui Ngoc Long
 * @date 2025-03-17
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Init Oled
extern Adafruit_SSD1306 display;

// Init function
void setupDisplay();

// Update display function on Oled
void updateDisplay(float temperature, float humidity, int gasValue);

#endif