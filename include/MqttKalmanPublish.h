#pragma once

#include <EspMQTTClient.h>
#include <SimpleKalmanFilter.h>

class MQTTKalmanPublish
{
private:
    EspMQTTClient& client;
    const char* topic;
    const bool retained;

    const size_t publishEveryN;
    size_t currentCount = 0;

    SimpleKalmanFilter kalman;
    const float kalmanInitialError;
public:
    MQTTKalmanPublish(
        EspMQTTClient& client,
        const char* topic,
        const bool retained,
        const size_t publishEveryN,
        const float kalmanSensitivity,
        const float kalmanInitialError,
        const float kalmanInitialQ
    );

    MQTTKalmanPublish(
        EspMQTTClient& client,
        const char* topic,
        const bool retained,
        const size_t publishEveryN,
        const float kalmanSensitivity
    );

    float addMeasurement(float value);
    void restart();
};
