# MQTT Kalman Publish

This is a small library for Arduino Projects to publish `float` values over MQTT.
Sensors like Temperature (DHT) are not perfect and are sometimes above and below a value.
A Kalman filter helps with that.

The general approach is to record multiple values (for example every second) and add that value to the kalman filter.
Every once in a while (for example every 10 measurements) send that value out via mqtt.

This is easily done with one or two sensors but tends to accumulate sourcecode with more sensors.
So this library just does that combined without much code to read.

```c
#include <EspMQTTClient.h>
#include <MqttKalmanPublish.h>

const bool MQTT_RETAINED = false;
const int SEND_EVERY_RSSI = 10;
const float KALMAN_SENSITIVITY_RSSI = 10;

EspMQTTClient client(
  WIFI_SSID,
  WIFI_PASSWORD,
  MQTT_SERVER,
  MQTT_BASE_TOPIC,
  1883
);

MQTTKalmanPublish kalmanPublishRssi(client, "topic/rssi", MQTT_RETAINED, SEND_EVERY_RSSI, KALMAN_SENSITIVITY_RSSI);

void setup() {
  Serial.begin(115200);
  Serial.println();
}

void loop() {
  client.loop();

  delay(1000);

  long rssi = WiFi.RSSI();
  float avgRssi = kalmanPublishRssi.addMeasurement(rssi);
  Serial.print("RSSI        in dBm:     ");
  Serial.print(String(rssi).c_str());
  Serial.print("   Average: ");
  Serial.println(String(avgRssi).c_str());
}
```
