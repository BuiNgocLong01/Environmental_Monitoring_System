#include <Arduino.h>
#include "Control/Control.h"
#include "config.h"

static bool lastButtonState = HIGH;
static unsigned long lastDebounceTime = 0;
static const unsigned long debounceDelay = 50;

void setupControls() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}

bool handleButton() {
    bool reading = digitalRead(BUTTON_PIN);
    bool event = false;

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading == LOW && lastButtonState == HIGH) {
            event = true;
        }
    }

    lastButtonState = reading;
    return event;
}

void setLedState(bool state) {
    digitalWrite(LED_PIN, state ? HIGH : LOW);
}

void setBuzzerState(bool state) {
    digitalWrite(BUZZER_PIN, state ? HIGH : LOW);
}