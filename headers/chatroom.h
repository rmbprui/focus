/*
 * Author: Rui Pereira
 *
 * This file generates the Chatroom class
 *
 * April 17, 2021
 */
#ifndef CHATROOM_H
#define CHATROOM_H

#include "user.h"

#include <QString>

/**
 * @brief The Chatroom class
 *
 * This class is used to store the chatroom information
 */
class Chatroom
{

public:
    /**
     * @brief Constructor for Chatroom class
     * @param name
     * @param members
     * @param topic
     */
    Chatroom(QString name, QVector<User> members, QString topic);
    Chatroom();

    // Getters/Setters

    QString name() const;
    void setName(const QString &name);

    QString topic() const;
    void setTopic(const QString &topic);

    QVector<User> members() const;
    void setMembers(const QVector<User> &members);

private:
    QString _name;
    QVector<User> _members;
    QString _topic;
};

#endif // CHATROOM_H
