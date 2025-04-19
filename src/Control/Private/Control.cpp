#include "D:\Programming\PlatformIO\MQTT_ESP32\src\Control\Control.h"
#include "D:\Programming\PlatformIO\MQTT_ESP32\src\MQTT\MQTT.h"

// Biến lưu trạng thái nút bấm
static bool lastButtonState = LOW;

void setupControls() {
  // Khởi tạo các chân
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void handleButton() {
  bool buttonState = digitalRead(BUTTON_PIN);
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      client.publish(button_topic, "PRESSED");
      Serial.println("Button Pressed");
    }
    lastButtonState = buttonState;
  }
}

void setLedState(bool state) {
  digitalWrite(LED_PIN, state ? HIGH : LOW);
}

void setBuzzerState(bool state) {
  digitalWrite(BUZZER_PIN, state ? HIGH : LOW);
}