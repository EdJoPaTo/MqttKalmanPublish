#include "MqttKalmanPublish.h"

const float INITIAL_ESTIMATION_ERROR = 1000;
const float INITIAL_Q = 0.02;

MQTTKalmanPublish::MQTTKalmanPublish(
    EspMQTTClient& client,
    const char* topic,
    const bool retained,
    const int publishEveryN,
    const float kalmanSensitivity,
    const float kalmanInitialError,
    const float kalmanInitialQ
) :
    client(client),
    topic(topic),
    retained(retained),
    publishEveryN(publishEveryN),
    kalman(kalmanSensitivity, kalmanInitialError, kalmanInitialQ),
    kalmanInitialError(kalmanInitialError)
{ }

MQTTKalmanPublish::MQTTKalmanPublish(
    EspMQTTClient& client,
    const char* topic,
    const bool retained,
    const int publishEveryN,
    const float kalmanSensitivity
) :
    client(client),
    topic(topic),
    retained(retained),
    publishEveryN(publishEveryN),
    kalman(kalmanSensitivity, INITIAL_ESTIMATION_ERROR, INITIAL_Q),
    kalmanInitialError(INITIAL_ESTIMATION_ERROR)
{ }

float MQTTKalmanPublish::addMeasurement(float value) {
    float estimate = kalman.updateEstimate(value);

    currentCount++;
    if (currentCount >= publishEveryN) {
        currentCount = 0;
        client.publish(topic, String(estimate), retained);
    }

    return estimate;
}

void MQTTKalmanPublish::restart() {
    kalman.setEstimateError(kalmanInitialError);
    currentCount = 0;
}
