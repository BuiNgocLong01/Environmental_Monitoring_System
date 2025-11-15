#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "config.h"
#include "Display/Display.h"
#include "Sensor/Sensor.h"
#include "MQTT/MQTT.h" 
#include "Control/Control.h"
#include <WiFi.h>


// ===== TIMERS =====
static unsigned long lastSampleMs   = 0;
static unsigned long lastPeriodicMs = 0;
static unsigned long lastAlarmMs    = 0;

// Buzzer
static bool alarmHumiHi = false;
static bool alarmHumiLo = false;

// ===== TRẠNG THÁI ALARM (để hysteresis) =====
static bool alarmTempHi = false, alarmTempLo = false, alarmGas = false;

// ===== LAST VALUES FOR PERIODIC CHECK =====
static float lastTemp = NAN, lastHumi = NAN;
static int   lastGas  = -1;

void setup() 
{
    Serial.begin(115200);
    Serial.println("\n\nBooting Node: " NODE_NAME);

    Wire.begin(I2C_SDA, I2C_SCL);

#ifdef __OLED__
    setupDisplay();
#endif
    setupSensors();
    setupControls();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    // Init MQTT
    setupMQTT();

    // Time for stable system
    delay(HOLD_Time);
}

static void buildJson(char* out, size_t outsz, float t, float h, int g) {
  StaticJsonDocument<200> doc;
  doc["node"] = NODE_NAME;
  doc["temp"] = roundf(t * 10) / 10.0;
  doc["humi"] = roundf(h * 10) / 10.0;
  doc["gas"]  = g;
  serializeJson(doc, out, outsz);
}

static void logData(const char* tag, float t, float h, int g) 
{
  Serial.print("["); Serial.print(tag); Serial.print("] ");
  Serial.print("Node="); Serial.print(NODE_NAME);
  Serial.print(" | Temp="); Serial.print(t, 1); Serial.print(" °C");
  Serial.print(" | Humi="); Serial.print(h, 1); Serial.print(" %");
  Serial.print(" | Gas=");  Serial.println(g);
}


void loop()
{
  loopMQTT();

  const unsigned long now = millis();

  // 1) LẤY MẪU THEO TIMER (không gửi ở đây)
  if (now - lastSampleMs >= SAMPLE_MS) {
    lastSampleMs = now;

    float temperature = readTemperature();
    float humidity    = readHumidity();
    int   gasValue    = readGasValue();

    // Mapping ADC value
    gasValue = map(gasValue, 0, 4095, 0, 100);

    // Offset for actual systems
    if (String(NODE_NAME) == "node2") 
    { 
        gasValue -= 5;
    } 
    else if (String(NODE_NAME) == "node1") 
    {
        humidity -= 5;
    }
    else
    {}

    // =========================
    // 2) ON-EVENT (GỬI NGAY)
    // =========================
    bool overHi  = (temperature > TEMP_HI);
    bool overLo  = (temperature < TEMP_LO);
    bool overGas = (gasValue   > GAS_HI);

    // Temp High
    if (overHi && (now - lastAlarmMs > ALARM_COOLDOWN_MS)) 
    {
        alarmTempHi = true; alarmTempLo = false;
        char buf[200];
        buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

        logData("ALARM-TEMP-HI", temperature, humidity, gasValue);
        publishJsonData(buf);
        buzzerOn();
        lastAlarmMs = now;
    } 
    else if (alarmTempHi && temperature < (TEMP_HI - TEMP_HYST))
    {
        alarmTempHi = false;
        char buf[200];
        buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

        logData("RECOVERY",       temperature, humidity, gasValue);
        publishJsonData(buf);
        buzzerOff();
    }

    // Temp Low
    if (overLo && (now - lastAlarmMs > ALARM_COOLDOWN_MS)) 
    {
        alarmTempLo = true; alarmTempHi = false;
        char buf[200];
        buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

        logData("ALARM-TEMP-LOW", temperature, humidity, gasValue);
        publishJsonData(buf);
        buzzerOn();
        lastAlarmMs = now;
    } 
    else if (alarmTempLo && temperature > (TEMP_LO + TEMP_HYST)) 
    {
        alarmTempLo = false;
        char buf[200];
        buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

        logData("RECOVERY",       temperature, humidity, gasValue);
        buzzerOff();
        publishJsonData(buf);
    }

    // ===== HUMIDITY HIGH =====
    if (humidity > HUMI_HI && (now - lastAlarmMs > ALARM_COOLDOWN_MS)) 
    {
        alarmHumiHi = true; alarmHumiLo = false;
        char buf[200]; buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

        logData("ALARM-HUMI-HI", temperature, humidity, gasValue);
        publishJsonData(buf);
        buzzerOn();
        lastAlarmMs = now;
    }
    else if (alarmHumiHi && humidity < (HUMI_HI - HUMI_HYST))
    {
        alarmHumiHi = false;
        char buf[200]; buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

        logData("RECOVERY-HUMI-HI", temperature, humidity, gasValue);
        publishJsonData(buf);
        buzzerOff();
    }

    // ===== HUMIDITY LOW =====
    if (humidity < HUMI_LO && (now - lastAlarmMs > ALARM_COOLDOWN_MS)) 
    {
        alarmHumiLo = true; alarmHumiHi = false;
        char buf[200]; buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

        logData("ALARM-HUMI-LO", temperature, humidity, gasValue);
        buzzerOn();
        publishJsonData(buf);
        lastAlarmMs = now;
        }
        else if (alarmHumiLo && humidity > (HUMI_LO + HUMI_HYST)) {
        alarmHumiLo = false;
        char buf[200]; buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

        logData("RECOVERY-HUMI-LO", temperature, humidity, gasValue);
        publishJsonData(buf);
        buzzerOff();
    }

    // Gas High
    if (overGas && (now - lastAlarmMs > ALARM_COOLDOWN_MS)) 
    {
        alarmGas = true;
        char buf[200];
        buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

        logData("ALARM-GAS-HI", temperature, humidity, gasValue);
        publishJsonData(buf);
        buzzerOn();
        lastAlarmMs = now;
    } 
    else if (alarmGas && gasValue < (GAS_HI - GAS_HYST)) 
    {
        alarmGas = false;
        char buf[200];
        buildJson(buf, sizeof(buf), temperature, humidity, gasValue);
        logData("RECOVERY",       temperature, humidity, gasValue);
        publishJsonData(buf);
        buzzerOff();
    }

    // =========================
    // 3) PERIODIC
    // =========================
    if (now - lastPeriodicMs >= PERIODIC_MS) 
    {
      bool changed = false;
      char buf[200];

      if (isnan(lastTemp) || fabs(temperature - lastTemp) >= 0.2) changed = true;
      if (isnan(lastHumi) || fabs(humidity - lastHumi) >= 0.2) changed = true;
      if (lastGas < 0   || abs(gasValue - lastGas) >= 2)       changed = true;

      buildJson(buf, sizeof(buf), temperature, humidity, gasValue);

      // Cập nhật OLED
#ifdef __OLED__ 
      updateDisplay(temperature, humidity, gasValue);
#endif

      if (changed) {
        char buf[200]; buildJson(buf, sizeof(buf), temperature, humidity, gasValue);
        logData("PERIODIC", temperature, humidity, gasValue);
        publishJsonData(buf);
        lastTemp = temperature; lastHumi = humidity; lastGas = gasValue;
      }
      lastPeriodicMs = now;
    }
    }
}
