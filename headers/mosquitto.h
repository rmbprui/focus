/*
 * Author: Rui Pereira
 *
 * This file contains the Mosquitto class
 * that processes each mosquitto method
 *
 * February 27, 2021
 */
#ifndef MOSQUITTO_H
#define MOSQUITTO_H

#include "message.h"

#include <QVector>
#include <QMqttClient>

/**
 * @brief The Mosquitto class
 *
 * This class is used to process mqtt's features
 */
class Mosquitto : public QObject
{

public:
    /**
     * @brief Constructor for Mosquitto class
     * @param port
     * @param username
     */
    Mosquitto(QString host, int port, QString username);

    QMqttClient *client() const;

    /**
     * @brief publish - publishes a message in a specific topic with mqtt
     *
     * @param message
     * @param topic
     */
    void publish(QString message, QString topic);

    /**
     * @brief subscribe - subscribes to a topic with mqtt
     * @param topic
     */
    QMqttSubscription *subscribe(QString topic);



private:
    /**
     * @brief connectClient - connects the mqtt client
     */
    void connectClient();

    QMqttClient *_client;
};

#endif // MOSQUITTO_H
