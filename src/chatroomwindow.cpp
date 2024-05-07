/*
 * Author: Rui Pereira
 *
 * This file generates the chatroom layouts
 *
 * February 21, 2021
 */
#include "sql.h"
#include "mainwindow.h"
#include "chatroomwindow.h"
#include "mosquitto.h"
#include "ui_chatroomwindow.h"

#include <QtMqtt/QMqttClient>
#include <QMessageBox>
#include <iostream>
#include <profile.h>
#include <savegroup.h>
#include <string>

ChatroomWindow::ChatroomWindow(Chatroom chatroom, Mosquitto *mainClient, SQL *sql, User *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatroomWindow)
{
    ui->setupUi(this);

    _sql = sql;
    _user = user;
    _chatroom = chatroom;
    _mainClient = mainClient;
    _subscription = mainClient->subscribe(_chatroom.topic());
    ui->nameLabel->setText(_chatroom.name());

    connect(_subscription, &QMqttSubscription::messageReceived, this, &ChatroomWindow::onMessageReceived);
    // connect(_subscription, &QMqttSubscription::stateChanged, this, &Chatroom::onStateChanged);

    loadMessages();
    loadMembers();

    ui->editGroupBtn->hide();
    if(_sql->isGroupAdmin(_user, _chatroom.topic())) {
        ui->editGroupBtn->show();
    }
}

ChatroomWindow::~ChatroomWindow()
{
    delete ui;
}

void ChatroomWindow::loadMessages()
{
    try {
        QVector<Message> _messages = _sql->getMessagesByTopic(_chatroom.topic());
        for(Message message : _messages) {
            const QString content = message.dateTime().toString("dd/MM/yyyy hh:mm:ss")
                        + " " + message.username() + ": "
                        + message.content()
                        + QLatin1Char('\n');
            ui->messagesTextEdit->insertPlainText(content);
        }
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error loading messages!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void ChatroomWindow::loadMembers()
{
    ui->membersList->clear();
    for(auto each : _chatroom.members()) {
        QListWidgetItem *item = new QListWidgetItem;
        QVariant userData(each.username());
        item->setData(Qt::UserRole, userData);
        item->setText(each.getDisplayName() + " (" + each.username() + ")");
        ui->membersList->addItem(item);
    }
}

void ChatroomWindow::loadChatroom(Chatroom chatroom)
{
    _chatroom = chatroom;
    ui->nameLabel->setText(_chatroom.name());
    loadMembers();
}

void ChatroomWindow::onMessageReceived(const QMqttMessage &msg)
{
    QString payload = msg.payload();
    int length = payload.indexOf("*");
    QString id = payload.mid(0, length);
    try {
        Message *message = _sql->getMessageById(id);
        QString messageContent = payload.right(length);
        const QString content = message->dateTime().toString("dd/MM/yyyy hh:mm:ss")
                    + " " + message->username() + ": "
                    + message->content()
                    + QLatin1Char('\n');
        ui->messagesTextEdit->insertPlainText(content);
        ui->messageTextEdit->clear();
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while loading message!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void ChatroomWindow::on_sendMessageBtn_clicked()
{
    if(ui->messageTextEdit->toPlainText() != "")
    {
        Message message = *new Message(ui->messageTextEdit->toPlainText(),
                                       QDateTime::currentDateTime().toString(),
                                       _chatroom.topic(), _user->username());
        int id = 0;
        try {
            id = _sql->addMessage(message);
        } catch (const std::domain_error &e) {
            QMessageBox messageBox;
            messageBox.setText("Error while sending message!");
            messageBox.exec();
            std::cout << e.what() << std::endl;
        }
        if(id != 0)
        {
            QString payload = QString::number(id)
                    + "*"
                    + _user->username()
                    + ui->messageTextEdit->toPlainText();
            _mainClient->publish(payload, _chatroom.topic());
        } else {
            QMessageBox messageBox;
            messageBox.setText("Error sending message!");
            messageBox.exec();
        }
    }
}

void ChatroomWindow::on_membersList_itemDoubleClicked(QListWidgetItem *item)
{
    try {
        User *user = _sql->getUserByUsername(item->data(Qt::UserRole).toString());
        Profile *profileDialog = new Profile(_sql, _user, user);
        profileDialog->exec();
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while searching for user!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void ChatroomWindow::on_editGroupBtn_clicked()
{
    SaveGroup *updateGroupDialog = new SaveGroup(_sql, _user, &_chatroom);
    updateGroupDialog->exec();
    if(updateGroupDialog->_deleted)
        loadChatroom(*new Chatroom("Chatroom Deleted", {}, "deleted"));
    else
        loadChatroom(*updateGroupDialog->group());
}
