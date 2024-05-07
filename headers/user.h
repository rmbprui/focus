/*
 * Author: Rui Pereira
 *
 * This file contains the User class to manage users
 *
 * February 24, 2021
 */
#ifndef USER_H
#define USER_H

#include "mosquitto.h"

#include <QString>
/**
 * @brief The User class
 */
class User
{
public:
    User();
    /**
     * @brief Constructor for User class
     * @param username
     * @param email
     * @param firstName
     * @param lastName
     */
    explicit User(QString username, QString email, QString firstName, QString lastName);

    // Getters/Setters

    int getId() const;

    QString username() const;
    void setUsername(const QString &username);

    QString email() const;
    void setEmail(const QString &email);

    QString getFirstName() const;
    void setFirstName(const QString &firstName);

    QString getLastName() const;
    void setLastName(const QString &lastName);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

private:
    int _id;
    QString _username;
    QString _email;
    QString _firstName;
    QString _lastName;
    QString _displayName;
};

#endif // USER_H
