/*
 * Author: Rui Pereira
 *
 * This file contains the Message class that processes the messages
 *
 * February 23, 2021
 */
#include "message.h"

Message::Message()
{
}

Message::Message(QString content, QString dateString, QString topic, QString username)
{
    _topic = topic;
    _content = content;
    _username = username;
    QDateTime dateTime = QDateTime::fromString(dateString);
    _dateTime = dateTime;
}

QString Message::content() const
{
    return _content;
}

void Message::setContent(const QString &content)
{
    _content = content;
}

QString Message::topic() const
{
    return _topic;
}

void Message::setTopic(const QString &topic)
{
    _topic = topic;
}

QString Message::username() const
{
    return _username;
}

void Message::setUsername(const QString &username)
{
    _username = username;
}

QDateTime Message::dateTime() const
{
    return _dateTime;
}

void Message::setDateTime(const QDateTime &dateTime)
{
    _dateTime = dateTime;
}
