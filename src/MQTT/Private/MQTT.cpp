#include "D:\Programming\PlatformIO\MQTT_ESP32\src\MQTT\MQTT.h"
#include "D:\Programming\PlatformIO\MQTT_ESP32\src\Control\Control.h"

// Config Wi-Fi and MQTT
const char* ssid = "Lan phuong"; // Wifi namename
const char* password = "12345679"; // Wifi password
const char* mqtt_server = "192.168.1.5"; // Raspberry Pi IP AddressAddress
const int mqtt_port = 8883;
// const int mqtt_port = 8883;
const char* mqtt_user = "longbui"; // Tên người dùng MQTT (nếu có xác thực)
const char* mqtt_pass = "123456"; // Mật khẩu MQTT (nếu có xác thực)
// Các topic MQTT
const char* node_id = "node1"; // Thay đổi thành "node1" hoặc "node2"
String temp_topic = String("esp32/") + node_id + "/temperature";
String humi_topic = String("esp32/") + node_id + "/humidity";
String gas_topic = String("esp32/") + node_id + "/gas";
String led_control_topic = String("esp32/") + node_id + "/control/led";
String buzzer_control_topic = String("esp32/") + node_id + "/control/buzzer";
const char* button_topic = "esp32/button"; // Topic chung cho nút bấm 


// Chứng chỉ CA 
const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDmTCCAoGgAwIBAgIUTKrw8Ank+T0SAu2Hts0lv411bLgwDQYJKoZIhvcNAQEL\n"
"BQAwXDELMAkGA1UEBhMCVk4xDjAMBgNVBAgMBUhhbm9pMQ4wDAYDVQQHDAVIYW5v\n"
"aTEQMA4GA1UECgwHTG9uZ2J1aTEMMAoGA1UECwwDSU9UMQ0wCwYDVQQDDARteUNB\n"
"MB4XDTI1MDQxMzAzMzExN1oXDTI2MDQxMzAzMzExN1owXDELMAkGA1UEBhMCVk4x\n"
"DjAMBgNVBAgMBUhhbm9pMQ4wDAYDVQQHDAVIYW5vaTEQMA4GA1UECgwHTG9uZ2J1\n"
"aTEMMAoGA1UECwwDSU9UMQ0wCwYDVQQDDARteUNBMIIBIjANBgkqhkiG9w0BAQEF\n"
"AAOCAQ8AMIIBCgKCAQEAyle5ZzmQu7BGlW0XbaHVsUsmD6aD8n+StanXdERRPWmZ\n"
"KlVL7bOpzJoG+EFJ8XCICMRUnSmbocqSaAenXZVcBzL9vXF9h22kp48gqAruIJLu\n"
"w6Aa/t8XmeOF0jNO62FQCKWWcjC6JcPNqXyzyv0/E7YlVeuZr2mkVnaCepWTrIuf\n"
"djao2XIJ1pp8QjtRKqoC7Bg1SMlHOFbptEE5URfnMSFSET2KM5C3BK09O1HdYbI1\n"
"xkzeeKzFpWtMJKFL7WZ94DSEVsxtY3ownzMqulqYhNHveIptPn3ymEviSFXwgHPu\n"
"C99YEdliPA+UtHfT0524mnACRldiQyGQtIjqmSTKyQIDAQABo1MwUTAdBgNVHQ4E\n"
"FgQUTkdVXniJOJ0YLqNiRlIgI0eaTEwwHwYDVR0jBBgwFoAUTkdVXniJOJ0YLqNi\n"
"RlIgI0eaTEwwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAj72v\n"
"BenrUTJdyH4w1Lzq0g+mD925kRGQ4SZks5RVKulUPWKggiA9qrVMO80jH5jA6/gX\n"
"HZ4O+FKzv1fT9oSCx64JR+TsldYQ6+9sn1v07jKTIldAlcm8/sayovbreu2NiaiX\n"
"VHnSxdlP5wtWSELPGuNhlp1FN2IjL+VI2xZyO0WviCmjGYDI3Cbvs5ME9a2UU1DG\n"
"9B6SUMG4iR8EQ5TPz2bR1d34kzXXyku6xOf0gwbO1bm2wRh4WQoa/yhu8jLqenYW\n"
"S9bCUN8jVgzTLJOA/zNZL22s05hEKNTkazX+S2XsE4XvHzNa/FCZx2norixQxk9n\n"
"cOrY36uPGgmM1XEnqA==\n"
"-----END CERTIFICATE-----\n";
// Khởi tạo client MQTT
WiFiClientSecure espClient;
// WiFiClient espClient;
PubSubClient client(espClient);

// Hàm callback xử lý dữ liệu nhận được từ MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == led_control_topic) {
    if (message == "ON") {
      setLedState(true);
      Serial.println("LED ON");
    } else if (message == "OFF") {
      setLedState(false);
      Serial.println("LED OFF");
    }
  } else if (String(topic) == buzzer_control_topic) {
    if (message == "ON") {
      setBuzzerState(true);
      Serial.println("Buzzer ON");
    } else if (message == "OFF") {
      setBuzzerState(false);
      Serial.println("Buzzer OFF");
    }
  }
}

void setupMQTT() {
  // Kết nối Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Kết nối đến MQTT broker
  espClient.setCACert(ca_cert);
  // espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void reconnect() {
  // Kết nối lại nếu mất kết nối MQTT
  while (!client.connected()) 
  {
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP32-";
    clientId += node_id;
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
    // if (client.connect(clientId.c_str())){
      Serial.println("Connected to MQTT with SSL/TLS");
      client.subscribe(led_control_topic.c_str());
      client.subscribe(buzzer_control_topic.c_str());
    } 
    else 
    {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loopMQTT() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
}

void publishSensorData(float temperature, float humidity, int gasValue) 
{
  char tempStr[8];
  char humiStr[8];
  char gasStr[8];
  dtostrf(temperature, 6, 2, tempStr);
  dtostrf(humidity, 6, 2, humiStr);
  sprintf(gasStr, "%d", gasValue);

  client.publish(temp_topic.c_str(), tempStr);
  client.publish(humi_topic.c_str(), humiStr);
  client.publish(gas_topic.c_str(), gasStr);

}