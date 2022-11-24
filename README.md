# MQTT Kalman Publish

This is a small library for Arduino Projects to publish `float` values over MQTT.
Sensors like Temperature (DHT) are not perfect and are sometimes above and below a value.
A Kalman filter helps with that.

The general approach is to record multiple values (for example every second) and add that value to the Kalman filter.
Every once in a while (for example every 10 measurements) send that value out via MQTT.

This is easily done with one or two sensors but tends to accumulate source code with more sensors.
So this library just does that combined without much code to read.

```c
#include <EspMQTTClient.h>
#include <MqttKalmanPublish.h>

const bool MQTT_RETAINED = false;
const int SEND_EVERY_RSSI = 10;
const float KALMAN_SENSITIVITY_RSSI = 10;

EspMQTTClient mqttClient(
  WIFI_SSID,
  WIFI_PASSWORD,
  MQTT_SERVER,
  MQTT_BASE_TOPIC,
  1883
);

MQTTKalmanPublish kalmanPublishRssi(mqttClient, "topic/rssi", MQTT_RETAINED, SEND_EVERY_RSSI, KALMAN_SENSITIVITY_RSSI);

void setup() {
  Serial.begin(115200);
  Serial.println();
}

void loop() {
  mqttClient.loop();

  delay(1000);

  if (mqttClient.isWifiConnected()) {
    int8_t rssi = WiFi.RSSI();
    float avgRssi = kalmanPublishRssi.addMeasurement(rssi);
    Serial.printf("RSSI in dBm: %4d  Average: %8.2f\n", rssi, avgRssi);
  }
}
```
