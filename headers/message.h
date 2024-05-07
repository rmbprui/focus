/*
 * Author: Rui Pereira
 *
 * This file contains the Message class that processes the messages
 *
 * February 23, 2021
 */
#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QDate>

/**
 * @brief The Message class
 *
 * This class manages the messages' information
 */
class Message
{
public:
    /**
     * @brief Constructor for Message class
     * @param content
     * @param dateString
     * @param topic
     * @param username
     */
    Message(QString content, QString dateString, QString topic, QString username);
    Message();

    // Getters/Setters

    QString content() const;
    void setContent(const QString &content);

    QString topic() const;
    void setTopic(const QString &topic);

    QString username() const;
    void setUsername(const QString &username);

    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dateTime);

private:
    QString _content;
    QDateTime _dateTime;
    QString _topic;
    QString _username;
};

#endif // MESSAGE_H
