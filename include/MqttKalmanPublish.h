#pragma once

#include <EspMQTTClient.h>
#include <SimpleKalmanFilter.h>

class MQTTKalmanPublish
{
private:
    EspMQTTClient& client;
    const char* topic;
    const bool retained;

    const int publishEveryN;
    int currentCount = 0;

    SimpleKalmanFilter kalman;
    const float kalmanInitialError;
public:
    MQTTKalmanPublish(
        EspMQTTClient& client,
        const char* topic,
        const bool retained,
        const int publishEveryN,
        const float kalmanSensitivity,
        const float kalmanInitialError,
        const float kalmanInitialQ
    );

    MQTTKalmanPublish(
        EspMQTTClient& client,
        const char* topic,
        const bool retained,
        const int publishEveryN,
        const float kalmanSensitivity
    );

    float addMeasurement(float value);
    void restart();
};
