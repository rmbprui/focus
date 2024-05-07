/*
 * Author: Rui Pereira
 *
 * This file contains the User class to manage users
 *
 * February 24, 2021
 */
#include "user.h"
#include "mosquitto.h"

User::User(){};

User::User(QString username,QString email, QString firstName, QString lastName)
{
    _username = username;
    _email = email;
    _firstName = firstName;
    _lastName = lastName;
    _displayName = _firstName + " " + _lastName;
}

int User::getId() const
{
    return _id;
}

QString User::username() const
{
    return _username;
}

void User::setUsername(const QString &username)
{
    _username = username;
}

QString User::email() const
{
    return _email;
}

void User::setEmail(const QString &email)
{
    _email = email;
}

QString User::getFirstName() const
{
    return _firstName;
}

void User::setFirstName(const QString &firstName)
{
    _firstName = firstName;
}

QString User::getLastName() const
{
    return _lastName;
}

void User::setLastName(const QString &lastName)
{
    _lastName = lastName;
}

QString User::getDisplayName() const
{
    return _displayName;
}

void User::setDisplayName(const QString &displayName)
{
    _displayName = displayName;
}
