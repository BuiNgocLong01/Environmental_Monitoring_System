#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "MQTT/MQTT.h"
#include "Control/Control.h"
#include "config.h"

WiFiClientSecure espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) { message += (char)payload[i]; }

    Serial.printf("Message arrived on topic: %s\n", topic);
    Serial.printf("Message: %s\n", message.c_str());

    if (String(topic) == TOPIC_CONTROL_LED) {
        if (message == "ON") setLedState(true); else setLedState(false);
    } else if (String(topic) == TOPIC_CONTROL_BUZZER) {
        if (message == "ON") setBuzzerState(true); else setBuzzerState(false);
    }
}

void setupMQTT() {
    espClient.setInsecure();
    client.setServer(HIVEMQ_HOST, HIVEMQ_PORT);
    client.setCallback(callback);
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection to HiveMQ Cloud...");
        String clientId = "ESP32_";
        clientId += NODE_NAME;

        const char* lwt_message = "offline";
        if (client.connect(clientId.c_str(), HIVEMQ_USER, HIVEMQ_PASS, TOPIC_STATUS, 1, true, lwt_message)) {
            Serial.println("connected!");
            client.publish(TOPIC_STATUS, "online", true);
            client.subscribe(TOPIC_CONTROL_LED);
            client.subscribe(TOPIC_CONTROL_BUZZER);
        } else {
            Serial.printf("failed, rc=%d. Try again in 5 seconds\n", client.state());
            delay(5000);
        }
    }
}

void loopMQTT() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

void publishJsonData(const char* jsonData) {
    if (client.connected()) {
        client.publish(TOPIC_DATA, jsonData, false); // Gửi với retain=false
    }
}

void publishButtonPress() {
    if (client.connected()) {
        client.publish(TOPIC_BUTTON, "pressed", false);
    }
}

void publishJsonAlarm(const char* jsonData) {
  if (client.connected()) {
    client.publish(TOPIC_ALARM, jsonData, false);
  }
}
