#include <Wire.h>
#include "D:\Programming\PlatformIO\MQTT_ESP32\src\Display\Display.h"

// Khởi tạo OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupDisplay() {
  // Khởi tạo I2C
  Wire.begin();

  // Khởi tạo OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Hiển thị thông báo khởi động
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Monitoring System"));
  display.setCursor(0, 8);
  display.println(F("Bui Ngoc Long"));
  display.display();
}

void updateDisplay(float temperature, float humidity, int gasValue) {
  // Cập nhật hiển thị trên OLED
  display.clearDisplay();

  // Vùng vàng (top 16 pixels)
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Monitoring System"));
  display.setCursor(0, 8);
  display.println(F("Bui Ngoc Long"));

  // Vùng xanh (y=16 to y=63): 3 hàng đều nhau (mỗi hàng 16 pixels)
  // Hàng 1 (y=16 to y=31): Nhiệt độ
  display.setTextSize(1);
  display.setCursor(0, 16);
  display.print(F("Temp: "));
  display.setTextSize(2);
  display.setCursor(40, 16);
  display.print(temperature, 1);
  display.print(F(" C"));

  // Hàng 2 (y=32 to y=47): Độ ẩm
  display.setTextSize(1);
  display.setCursor(0, 32);
  display.print(F("Humi: "));
  display.setTextSize(2);
  display.setCursor(40, 32);
  display.print(humidity, 1);
  display.print(F(" %"));

  // Hàng 3 (y=40 to y=63): Khí gas
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.print(F("Gas: "));
  display.setTextSize(2);
  display.setCursor(40, 48);
  display.print(gasValue);

  display.display();

  // Print to Serial port
  Serial.print("Published Temp: ");
  Serial.print(temperature);
  Serial.print(" C, Humi: ");
  Serial.print(humidity);
  Serial.print(" %, Gas: ");
  Serial.println(gasValue);
}