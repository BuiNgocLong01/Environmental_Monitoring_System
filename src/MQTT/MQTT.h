#ifndef MQTT_H
#define MQTT_H

void setupMQTT();
void loopMQTT();
void publishJsonData(const char* jsonData);
void publishButtonPress();
void publishJsonAlarm(const char* jsonData);


#endif