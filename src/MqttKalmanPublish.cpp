#include "MqttKalmanPublish.h"

MQTTKalmanPublish::MQTTKalmanPublish(
	EspMQTTClient &client,
	const char *topic,
	const bool retained,
	const size_t publishEveryN,
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

float MQTTKalmanPublish::addMeasurement(float value) {
	float estimate = kalman.updateEstimate(value);

	currentCount++;
	if (currentCount >= publishEveryN) {
		bool success = client.publish(topic, String(estimate), retained);
		if (success) {
			currentCount = 0;
		}
	}

	return estimate;
}

void MQTTKalmanPublish::restart() {
	kalman.setEstimateError(kalmanInitialError);
	currentCount = 0;
}
