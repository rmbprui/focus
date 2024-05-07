/*
 * Author: Rui Pereira
 *
 * This file generates the Chatroom class
 *
 * April 17, 2021
 */
#include "chatroom.h"

Chatroom::Chatroom(){
    _members = {};
}

Chatroom::Chatroom(QString name, QVector<User> members, QString topic)
{
    _name = name;
    _topic = topic;
    _members = members;
}

QString Chatroom::topic() const
{
    return _topic;
}

void Chatroom::setTopic(const QString &topic)
{
    _topic = topic;
}

QVector<User> Chatroom::members() const
{
    return _members;
}

void Chatroom::setMembers(const QVector<User> &members)
{
    _members = members;
}

QString Chatroom::name() const
{
    return _name;
}

void Chatroom::setName(const QString &name)
{
    _name = name;
}
