#ifndef CONTROLS_H
#define CONTROLS_H

// Định nghĩa chân
#define BUZZER_PIN 16  // Còi trên GPIO 16
#define BUTTON_PIN 26  // Nút bấm trên GPIO 26
#define LED_PIN 12     // LED trên GPIO 12

// Hàm khởi tạo các chân điều khiển
void setupControls();

// Hàm xử lý trạng thái nút bấm
void handleButton();

// Hàm đặt trạng thái LED
void setLedState(bool state);

// Hàm đặt trạng thái còi
void setBuzzerState(bool state);

#endif