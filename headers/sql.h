/*
 * Author: Rui Pereira
 *
 * This file contains the SQL class
 * that processes each mysql method
 *
 * March 24, 2021
 */
#pragma once

#ifndef SQL_H
#define SQL_H

#include "user.h"
#include "chatroom.h"

#include <mysql.h>
#include <QString>

/**
 * @brief The SQL class
 *
 * This class is used to process mysql features
 */
class SQL
{

public:
    /**
     * @brief Constructor for SQL class
     */
    SQL();

    /**
     * @brief validateTopic - validates the topic name
     * @param topic
     * @return QString
     */
    QString validateTopic(QString topic);

    /**
     * @brief addGroup - adds a new group to the database
     * @param name
     * @param topic
     * @param user
     */
    void addGroup(QString name, QString topic, User user);
    /**
     * @brief addUser - adds a new user to the database
     * @param user
     * @param passwordHash
     */
    void addUser(User *user, QString passwordHash);
    /**
     * @brief addContactByUsername - connects the user to the requested contact
     * @param user
     * @param contactUsername
     * @return bool (true if successfull)
     */
    void addContactByUsername(User *user, QString contactUsername);
    int addMessage(Message &message);
    /**
     * @brief addSubscription - stores subscription in database
     * @param topic
     * @param user
     */
    void addSubscription(QString topic, User *user);

    /**
     * @brief updateGroup - updates the group information
     * @param group
     */
    void updateGroup(Chatroom *group);
    /**
     * @brief updateUser - updates the user information
     * @param user
     */
    void updateUser(User *user);

    /**
     * @brief removeSubscriptions - removes the stored subscription from the database
     * @param topic
     */
    void removeSubscriptions(QString topic);
    /**
     * @brief removeContact - removes the contact for respective user
     * @param currentUser
     * @param user
     */
    void removeContact(User *currentUser, User *user);
    /**
     * @brief removeGroup - removes group information from database
     * @param group
     */
    void removeGroup(Chatroom *group);

    /**
     * @brief getSubscriptionsByUser - returns subscriptions for respective user
     * @param user
     * @return QVector<QString>
     */
    QVector<QString> getSubscriptionsByUser(User *user);
    /**
     * @brief getMessagesByTopic - returns the last 30 messages for respective topic
     * @param topic
     * @param limit (30 messages)
     * @return QVector<Message>
     */
    QVector<Message> getMessagesByTopic(QString topic, int limit = 30);
    /**
     * @brief getUserContacts - returns vector with the user's contacts
     * @param user
     * @return QVector<User>
     */
    QVector<User> getUserContacts(User *user);
    /**
     * @brief getUserDms - returns the direct messages of the respective user
     * @param user
     * @return QVector<Chatroom>
     */
    QVector<Chatroom> getUserDms(User *user);
    /**
     * @brief getGroupsByTopic - returns the repective user's groups
     * @param topic
     * @return QVector<Chatroom>
     */
    QVector<Chatroom> getGroupsByTopic(QString topic);
    /**
     * @brief getMembersByTopic - returns the members subscribe to the respective topic
     * @param topic
     * @return QVector<User>
     */
    QVector<User> getMembersByTopic(QString topic);

    /**
     * @brief getUserByUsername - returns the user's information by username
     * @param username
     * @return User
     */
    User *getUserByUsername(QString username);
    /**
     * @brief getUserByEmail - returns the user's information by email
     * @param email
     * @return User
     */
    User *getUserByEmail(QString email);
    /**
     * @brief getMessageById - returns the message information by id
     * @param id
     * @return Message
     */
    Message *getMessageById(QString id);

    /**
     * @brief isValidLoginByEmail - checks if the login by email is valid
     * @param email
     * @param passwordHash
     * @return  bool
     */
    bool isValidLoginByEmail(QString email, QString passwordHash);
    /**
     * @brief isValidLoginByUsername - checks if the login by username is valid
     * @param username
     * @param passwordHash
     * @return bool
     */
    bool isValidLoginByUsername(QString username, QString passwordHash);
    /**
     * @brief isGroupAdmin - returns true if the respective user is the group's admin
     * @param user
     * @param topic
     * @return bool
     */
    bool isGroupAdmin(User *user, QString topic);
    /**
     * @brief areContacts - checks if the users are connected
     * @param currentUser
     * @param user
     * @return bool
     */
    bool areContacts(User *currentUser, User *user);

private:
    /**
     * @brief connect - connects to the database
     */
    void connect();
    /**
     * @brief disconnect - disconnects from the database
     */
    void disconnect();
    MYSQL *_db;
};


#endif // SQL_H
