/*
 * Author: Rui Pereira
 *
 * This file contains the Mosquitto class
 * that processes each mosquitto method
 *
 * February 27, 2021
 */
#include "mosquitto.h"
#include "message.h"
#include "user.h"

#include <iostream>
#include <QVector>
#include <QtMqtt/QMqttClient>

Mosquitto::Mosquitto(QString host, int port, QString username)
{
    _client = new QMqttClient(this);
    _client->setHostname(host);
    _client->setPort(port);
    _client->setClientId(username);

    connectClient();
}

void Mosquitto::connectClient()
{
    _client->connectToHost();
    if(_client->state()) {
        std::cout << "Mqtt connected with client: " << _client->clientId().toStdString() << std::endl;
    }
}

void Mosquitto::publish(QString message, QString topic)
{
    if (_client->publish(topic, message.toUtf8()) == -1)
    {
        std::cerr << "Error while publishing to topic!" << std::endl;
    }
}

QMqttSubscription *Mosquitto::subscribe(QString topic)
{
    auto subscription = _client->subscribe(topic);
    if (!subscription)
    {
        std::cerr << "Error while subscribing to topic " + topic.toStdString() << std::endl;
    }

    return subscription;
}

QMqttClient *Mosquitto::client() const
{
    return _client;
}
