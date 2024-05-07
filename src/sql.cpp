/*
 * Author: Rui Pereira
 *
 * This file contains the SQL class
 * that processes each mysql method
 *
 * March 24, 2021
 */
#include "sql.h"
#include "message.h"

#include <QCryptographicHash>
#include <mysql.h>
#include <iostream>
#include <string>
#include <chatroom.h>

const QString kSaltKey = "E1F53135E559C253";

char host[] = "127.0.0.1";
char user[] = "admin";
char password[] = "admin";
char dbName[] = "focus_db";
int port = 3306;

SQL::SQL(){
    try {
        connect();
    } catch (const std::domain_error &e) {
        std::cout << e.what() << std::endl;
    }
}

void SQL::connect() {
    _db = mysql_init(NULL);
    mysql_real_connect(_db,host,user,password,dbName,port,0,0);

    if (_db == NULL)
    {
        throw std::domain_error(mysql_error(_db));
    }
}

void SQL::disconnect()
{
    mysql_close(_db);
}

QString SQL::validateTopic(QString topic)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    QString query_string =
            "SELECT MAX(`id`)"
            "FROM `groups`;";
    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    int id = 1;
    while ((row = mysql_fetch_row(result)))
    {
        if(row[0])
            id = atoi(row[0]) + 1;
    }
    mysql_free_result(result);
    topic = topic + "_" + QString::fromStdString(std::to_string(id));
    return topic;
}

void SQL::addGroup(QString name, QString topic, User user)
{
    MYSQL_RES *result;
    QString query_string =
            "INSERT INTO `groups`(`name`, `topic`, `admin`) VALUES ('"
            + name + "', '"
            + topic + "', '"
            + user.username()
            + "')";
    int insert = mysql_query(_db, query_string.toStdString().c_str());
    if (insert != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    mysql_free_result(result);
}

void SQL::addUser(User *user, QString passwordHash)
{
    MYSQL_RES *result;
    QString query_string =
            "INSERT INTO `users`(`username`, `email`, `passwordHash`, `firstName`, `lastName`) "
            "VALUES ('"
            + user->username() + "', '"
            + user->email() + "', '"
            + passwordHash + "', '"
            + user->getFirstName() + "', '"
            + user->getLastName()
            + "');";
    int insert = mysql_query(_db, query_string.toStdString().c_str());
    if (insert != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    mysql_free_result(result);
}

void SQL::addContactByUsername(User *user, QString contactUsername)
{
    MYSQL_RES *result;
    try {
        getUserByUsername(contactUsername);
    } catch (const std::domain_error &e) {
        throw std::domain_error(e.what());
    }

    QString topic = user->username() + "_" + contactUsername;
    QString query_string =
            "INSERT INTO `contacts`(`user1`, `user2`, `topic`) "
            "VALUES ('"
            + user->username() + "', '"
            + contactUsername + "', '"
            + topic
            + "');";
    int insert = mysql_query(_db, query_string.toStdString().c_str());
    if (insert != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    mysql_free_result(result);
}

void SQL::addSubscription(QString topic, User *user)
{
    MYSQL_RES *result;
    QString query_string =
            "INSERT INTO `subscriptions`(`topic`, `username`) "
            "VALUES ('"
            + topic + "', '"
            + user->username()
            + "');";
    int insert = mysql_query(_db, query_string.toStdString().c_str());
    if (insert != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    mysql_free_result(result);
}

int SQL::addMessage(Message &message)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    QString query_string =
            "INSERT INTO `messages`(`content`, `date`, `topic`, `username`) VALUES ('"
            + message.content() + "', '"
            + message.dateTime().toString() + "', '"
            + message.topic() + "', '"
            + message.username()
            + "')";
    int insert = mysql_query(_db, query_string.toStdString().c_str());
    if (insert != 0)
    {
        std::cout << "MySQL - " << mysql_error(_db) << std::endl;
    }
    query_string =
            "SELECT MAX(`id`)"
            "FROM `messages`;";
    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    int id = 0;
    while ((row = mysql_fetch_row(result)))
    {
        if(row[0])
            id = atoi(row[0]);
    }
    mysql_free_result(result);
    return id;
}

void SQL::updateGroup(Chatroom *group)
{
    MYSQL_RES *result;
    QString query_string =
            "UPDATE `groups` "
            "SET `name` = '" + group->name() + "' "
            "WHERE `topic` = '" + group->topic() + "';";
    int update = mysql_query(_db, query_string.toStdString().c_str());
    if (update != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    mysql_free_result(result);
    try {
        removeSubscriptions(group->topic());
        for(auto member : group->members()) {
            addSubscription(group->topic(), &member);
        }
    } catch (const std::domain_error &e) {
        throw std::domain_error(e.what());
    }
}

void SQL::updateUser(User *user)
{
    MYSQL_RES *result;
    QString query_string =
            "UPDATE `users` "
            "SET `firstName` = '" + user->getFirstName() + "', "
            "`lastName` = '" + user->getLastName() + "' "
            "WHERE `username` = '" + user->username() + "';";
    int update = mysql_query(_db, query_string.toStdString().c_str());
    if (update != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    mysql_free_result(result);
}

void SQL::removeSubscriptions(QString topic)
{
    MYSQL_RES *result;
    QString query_string =
            "DELETE FROM `subscriptions` "
            "WHERE `topic` = '" + topic + "';";
    int insert = mysql_query(_db, query_string.toStdString().c_str());
    if (insert != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    mysql_free_result(result);
}


void SQL::removeContact(User *currentUser, User *user)
{
    QString topic1 = currentUser->username() + "_" + user->username();
    QString topic2 = user->username() + "_" + currentUser->username();
    MYSQL_RES *result;
    QString query_string =
            "DELETE FROM `contacts` "
            "WHERE `topic` = '" + topic1 + "' "
            "OR `topic` = '" + topic2 + "';";
    int insert = mysql_query(_db, query_string.toStdString().c_str());
    if (insert != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    mysql_free_result(result);
}

void SQL::removeGroup(Chatroom *group)
{
    MYSQL_RES *result;
    QString query_string =
            "DELETE FROM `groups` WHERE `topic` = '" + group->topic() + "';";
    int insert = mysql_query(_db, query_string.toStdString().c_str());
    if (insert != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    mysql_free_result(result);
    try {
        removeSubscriptions(group->topic());
    } catch (const std::domain_error &e) {
        throw std::domain_error(e.what());
    }
}

QVector<QString> SQL::getSubscriptionsByUser(User *user)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    QVector<QString> topics;
    QString query_string =
            "SELECT `topic`"
            "FROM `subscriptions` "
            "WHERE `username` = '" + user->username() + "';";
    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    while ((row = mysql_fetch_row(result)))
    {
       topics.append(row[0]);
    }
    mysql_free_result(result);
    return topics;
}

QVector<Chatroom> SQL::getGroupsByTopic(QString topic) {
    MYSQL_RES *result;
    MYSQL_ROW row;

    QVector<Chatroom> groups;
    QString query_string =
            "SELECT `name`, `topic` "
            "FROM `groups` "
            "WHERE `topic` = '" + topic + "';";
    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    while ((row = mysql_fetch_row(result)))
    {
        try {
            QVector<User> members = getMembersByTopic(topic);
            Chatroom group = *new Chatroom(row[0], members, row[1]);
            groups.append(group);
        } catch (const std::domain_error &e) {
            throw std::domain_error(e.what());
        }
    }
    mysql_free_result(result);
    return groups;
}

QVector<User> SQL::getMembersByTopic(QString topic) {
    MYSQL_RES *result;
    MYSQL_ROW row;

    QVector<User> members;
    QVector<QString> usernames;
    QString query_string =
            "SELECT `username`"
            "FROM `subscriptions` "
            "WHERE `topic` = '" + topic + "';";
    int selectUsers = mysql_query(_db, query_string.toStdString().c_str());
    if (selectUsers != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    while ((row = mysql_fetch_row(result)))
    {
       usernames.append(row[0]);
    }
    mysql_free_result(result);

    for(auto username : usernames) {
        QString query =
                "SELECT `username`, `email`, `firstName`, `lastName` "
                "FROM `users` "
                "WHERE `username` = '" + username + "';";
        int select = mysql_query(_db, query.toStdString().c_str());
        if (select != 0)
        {
            throw std::domain_error(mysql_error(_db));
        }
        result = mysql_store_result(_db);
        while ((row = mysql_fetch_row(result)))
        {
           User member = *new User(row[0], row[1], row[2], row[3]);
           members.append(member);
        }
    }
    mysql_free_result(result);
    return members;
}

QVector<Message> SQL::getMessagesByTopic(QString topic, int limit) {
    MYSQL_RES *result;
    MYSQL_ROW row;

    QVector<Message> messages;
    QString query_string =
            "SELECT `content`, `date`, `topic`, `username` "
            "FROM `messages` "
            "WHERE `topic` = '" + topic + "' "
            "ORDER BY `id` DESC "
            "LIMIT " + QString::number(limit) + ";";

    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    while ((row = mysql_fetch_row(result)))
    {
       Message message = *new Message(row[0], row[1], row[2], row[3]);
       messages.append(message);
    }
    mysql_free_result(result);
    std::reverse(messages.begin(), messages.end());
    return messages;
}

QVector<User> SQL::getUserContacts(User *user) {
    MYSQL_RES *result;
    MYSQL_ROW row;

    QVector<User> contacts;
    QString query_string =
            "SELECT `user1`, `user2` "
            "FROM `contacts` "
            "WHERE `user1` = '" + user->username() + "' "
            "OR `user2` = '" + user->username() + "';";
    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    QString contactUsername = "";
    while ((row = mysql_fetch_row(result)))
    {
        if(row[0] != user->username())
            contactUsername = row[0];
        else contactUsername = row[1];
        try {
            User contactUser = *getUserByUsername(contactUsername);
            contacts.append(contactUser);
        } catch (const std::domain_error &e) {
            throw std::domain_error(e.what());
        }
    }
    mysql_free_result(result);
    return contacts;
}

QVector<Chatroom> SQL::getUserDms(User *user) {
    MYSQL_RES *result;
    MYSQL_ROW row;

    QVector<Chatroom> dms;
    QString query_string =
            "SELECT `user1`, `user2`, `topic` "
            "FROM `contacts` "
            "WHERE `user1` = '" + user->username() + "' "
            "OR `user2` = '" + user->username() + "';";
    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    QString contactUsername = "";
    while ((row = mysql_fetch_row(result)))
    {
        if(row[0] != user->username())
            contactUsername = row[0];
        else contactUsername = row[1];
        QString topic = row[2];
        QString name;
        QVector<User> members;
        if(contactUsername != user->username()) {
            try {
                User *contactUser = getUserByUsername(contactUsername);
                name = contactUser->getDisplayName();
                members = {*user, *contactUser};
            } catch (const std::domain_error &e) {
                throw std::domain_error(e.what());
            }
        }
        else {
            name = user->getDisplayName();
            members = {*user};
        }
        Chatroom dm = *new Chatroom(name, members, topic);
        dms.append(dm);
    }
    mysql_free_result(result);
    return dms;
}

User *SQL::getUserByUsername(QString username)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    QString query_string =
            "SELECT `email`, `firstName`, `lastName` "
            "FROM `users` "
            "WHERE `username` = '" + username + "';";
    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    int rows = mysql_num_rows(result);
    if(rows == 0)
        throw std::domain_error("No user found for username = " + username.toStdString());
    row = mysql_fetch_row(result);
    User *user = new User(username, row[0], row[1], row[2]);
    mysql_free_result(result);
    return user;
}

User *SQL::getUserByEmail(QString email)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    QString query_string =
            "SELECT `username`, `firstName`, `lastName` "
            "FROM `users` "
            "WHERE `email` = '" + email + "';";
    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    int rows = mysql_num_rows(result);
    if(rows == 0)
        throw std::domain_error("No user found for username = " + email.toStdString());
    row = mysql_fetch_row(result);
    User *user = new User(row[0], email, row[1], row[2]);
    mysql_free_result(result);
    return user;
}

Message *SQL::getMessageById(QString id)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    QString query_string =
            "SELECT `content`, `date`, `topic`, `username` "
            "FROM `messages` "
            "WHERE `id` = " + id + ";";
    int select = mysql_query(_db, query_string.toStdString().c_str());
    if (select != 0)
    {
        throw std::domain_error(mysql_error(_db));
    }
    result = mysql_store_result(_db);
    row = mysql_fetch_row(result);
    Message *message = new Message(row[0], row[1], row[2], row[3]);
    mysql_free_result(result);
    return message;
}

bool SQL::isValidLoginByEmail(QString email, QString passwordHash)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    QString query = "SELECT * FROM `users` "
                    "WHERE `email` = '" + email + "' AND `passwordHash` = '" + passwordHash + "';";
    int select = mysql_query(_db, query.toStdString().c_str());
    if (select != 0)
    {
        std::cout <<  "MySQL - " << mysql_error(_db) << std::endl;
        return false;
    }
    result = mysql_store_result(_db);

    int rows = mysql_num_rows(result);
    if(rows == 0)
        return false;

    while ((row = mysql_fetch_row(result)))
    {
        std::cout << "User logged in: " << row[1] << std::endl;
    }
    mysql_free_result(result);
    return true;
}

bool SQL::isValidLoginByUsername(QString username, QString passwordHash)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    QString query = "SELECT * FROM `users` "
                    "WHERE `username` = '" + username + "' AND `passwordHash` = '" + passwordHash + "';";
    int select = mysql_query(_db, query.toStdString().c_str());
    if (select != 0)
    {
        std::cout <<  "MySQL - " << mysql_error(_db) << std::endl;
        return false;
    }
    result = mysql_store_result(_db);

    int rows = mysql_num_rows(result);
    if(rows == 0)
        return false;

    while ((row = mysql_fetch_row(result)))
    {
        std::cout << "User logged in: " << row[1] << std::endl;
    }
    mysql_free_result(result);
    return true;
}

bool SQL::isGroupAdmin(User *user, QString topic)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    QString query = "SELECT `admin` FROM `groups` "
                    "WHERE `topic` = '" + topic + "';";
    int select = mysql_query(_db, query.toStdString().c_str());
    if (select != 0)
    {
        std::cout <<  "MySQL - " << mysql_error(_db) << std::endl;
        return false;
    }
    result = mysql_store_result(_db);

    int rows = mysql_num_rows(result);
    if(rows == 0)
        return false;

    while ((row = mysql_fetch_row(result)))
    {
        if(row[0] == user->username())
            return true;
    }
    mysql_free_result(result);
    return false;
}

bool SQL::areContacts(User *currentUser, User *user) {
    QString topic1 = currentUser->username() + "_" + user->username();
    QString topic2 = user->username() + "_" + currentUser->username();
    MYSQL_RES *result;
    QString query = "SELECT `topic` FROM `contacts` "
                    "WHERE `topic` = '" + topic1 + "' "
                    "OR `topic` = '" + topic2 + "';";
    int select = mysql_query(_db, query.toStdString().c_str());
    if (select != 0)
    {
        std::cout <<  "MySQL - " << mysql_error(_db) << std::endl;
        return false;
    }
    result = mysql_store_result(_db);
    int rows = mysql_num_rows(result);
    if(rows != 0)
        return true;
    mysql_free_result(result);
    return false;
}
