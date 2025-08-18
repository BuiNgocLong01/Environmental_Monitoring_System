#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Display/Display.h"
#include "config.h"

// Khởi tạo đối tượng display với các thông số từ config.h
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupDisplay() {
    // Lưu ý: Wire.begin() đã được gọi trong main.cpp

    // Khởi tạo màn hình OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Vòng lặp vô tận nếu không tìm thấy màn hình
    }

    // Hiển thị thông điệp khởi động
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Monitoring System"));
    display.setCursor(0, 10);
    display.println(F("Bui Ngoc Long"));
    display.display();
    delay(2000); // Tạm dừng 2 giây để hiển thị thông điệp
}

void updateDisplay(float temperature, float humidity, int gasValue) {
    display.clearDisplay();

    // Dòng tiêu đề
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15, 0); // Căn giữa một chút
    display.println(F("ENV MONITORING"));
    display.drawFastHLine(0, 10, display.width(), SSD1306_WHITE);


    // --- Hiển thị Nhiệt độ (Temp) ---
    display.setTextSize(1);
    display.setCursor(0, 16);
    display.print(F("Temp: "));
    display.setTextSize(2);
    display.setCursor(45, 16);
    display.print(temperature, 1);
    display.print(F(" C"));

    // --- Hiển thị Độ ẩm (Humidity) ---
    display.setTextSize(1);
    display.setCursor(0, 34);
    display.print(F("Humi: "));
    display.setTextSize(2);
    display.setCursor(45, 34);
    display.print(humidity, 1);
    display.print(F(" %"));

    // --- Hiển thị Khí Gas (Gas) ---
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print(F("Gas:  "));
    display.setTextSize(2);
    display.setCursor(45, 50);
    display.print(gasValue);

    // Cập nhật lên màn hình
    display.display();
}