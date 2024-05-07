/*
 * Author: Rui Pereira
 *
 * This file generates the chatroom layouts
 *
 * February 21, 2021
 */
#ifndef CHATROOMWINDOW_H
#define CHATROOMWINDOW_H

#include "sql.h"
#include "message.h"
#include "mosquitto.h"
#include "chatroom.h"
#include <QListWidget>
#include <QWidget>

namespace Ui { class ChatroomWindow; }

/**
 * @brief The Chatroom class
 *
 * This class is used to create the chatroom windows with QWidget
 */
class ChatroomWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for ChatroomWindow class
     * @param chatroom
     * @param mainClient
     * @param sql
     * @param user
     * @param parent
     */
    explicit ChatroomWindow(Chatroom chatroom, Mosquitto *mainClient, SQL *sql, User *user, QWidget *parent = nullptr);
    ~ChatroomWindow();

    /**
     * @brief loadMessages - loads the messages for the respective chatroom
     */
    void loadMessages();
    /**
     * @brief loadMembers - loads the members for the respective chatroom
     */
    void loadMembers();
    /**
     * @brief loadChatroom - loads the chatroom information
     * @param chatroom
     */
    void loadChatroom(Chatroom chatroom);

    void onMessageReceived(const QMqttMessage &msg);

private slots:

    void on_sendMessageBtn_clicked();

    void on_membersList_itemDoubleClicked(QListWidgetItem *item);

    void on_editGroupBtn_clicked();

private:
    Ui::ChatroomWindow *ui;
    Mosquitto *_mainClient;
    SQL *_sql;
    User *_user;
    QMqttSubscription *_subscription;
    Chatroom _chatroom;
    QVector<Message> _messages;
};

#endif // CHATROOMWINDOW_H
