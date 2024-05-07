/*
 * Author: Rui Pereira
 *
 * This file generates the application's main window
 *
 * February 21, 2021
 */
#include "auth.h"
#include "profile.h"
#include "user.h"
#include "mosquitto.h"
#include "savegroup.h"
#include "adduser.h"
#include "chatroomwindow.h"
#include "mainwindow.h"
#include "editprofile.h"
#include "ui_mainwindow.h"

#include <QElapsedTimer>
#include <QMessageBox>
#include <QStringListModel>
#include <QTimer>
#include <chatroom.h>
#include <iostream>

MainWindow::MainWindow(SQL *sql, Mosquitto *mainClient, User *user, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    _sql = sql;
    _user = user;
    _mainClient = mainClient;
    ui->setupUi(this);

    ui->nameLabel->setText(_user->getDisplayName());

    /// @note Workaround because mosquitto failed when loading from the constructor
    QTimer::singleShot(100, this, SLOT(timeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeout()
{
    loadChatrooms();
}

void MainWindow::newChatroomWindow(Chatroom chatroom)
{
    ChatroomWindow *chatroomWindow = new ChatroomWindow(chatroom, _mainClient, _sql, _user);
    chatroomWindow->setWindowTitle(chatroom.name());
    ui->stackedWidget->addWidget(chatroomWindow);
}
void MainWindow::loadGroupsList()
{
    ui->groupsList->clear();
    try {
        QVector<QString> topics = _sql->getSubscriptionsByUser(_user);
        QVector<Chatroom> groups;
        for(QString topic : topics) {
            try {
                groups.append(_sql->getGroupsByTopic(topic));
            } catch (const std::domain_error &e) {
                QMessageBox messageBox;
                messageBox.setText("Error while loading group!");
                messageBox.exec();
                std::cout << e.what() << std::endl;
            }
        }

        for(auto each : groups) {
            newChatroomWindow(each);
            ui->groupsList->addItem(each.name());
        }
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while loading chatrooms!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void MainWindow::loadDmsList()
{
    ui->dmsList->clear();
    try {
        QVector<Chatroom> dms = _sql->getUserDms(_user);

        for(auto each : dms) {
            newChatroomWindow(each);
            User contact = *_user;
            QListWidgetItem *item = new QListWidgetItem;
            for(auto member : each.members()) {
                if(member.username() != _user->username()) {
                    contact = member;
                }
            }
            QVariant userData(contact.username());
            item->setData(Qt::UserRole, userData);
            item->setText(contact.getDisplayName() + " (" + contact.username() + ")");
            ui->dmsList->addItem(item);
        }
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while loading contacts!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void MainWindow::loadChatrooms()
{
    for(int i = ui->stackedWidget->count() - 1; i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
    loadGroupsList();
    loadDmsList();
}

void MainWindow::on_actionLogout_triggered()
{
    Auth *authWindow = new Auth(_sql);
    authWindow->showMaximized();
    close();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionProfile_triggered()
{
    Profile *profile = new Profile(_sql, _user, _user);
    profile->show();
}

void MainWindow::on_createGroupBtn_clicked()
{
    SaveGroup *createGroupDialog = new SaveGroup(_sql, _user);
    createGroupDialog->exec();
    loadChatrooms();
}

void MainWindow::on_addUserBtn_clicked()
{
    AddUser *addUserDialog = new AddUser(_sql, _user);
    addUserDialog->exec();
    loadChatrooms();
}

void MainWindow::on_groupsList_itemSelectionChanged()
{
    ui->stackedWidget->setCurrentIndex(ui->groupsList->currentRow());
    ui->dmsList->clearSelection();
}

void MainWindow::on_dmsList_itemSelectionChanged()
{
    ui->stackedWidget->setCurrentIndex(ui->groupsList->count() + ui->dmsList->currentRow());
    ui->groupsList->clearSelection();
}

void MainWindow::on_refreshBtn_clicked()
{
    loadChatrooms();
}

void MainWindow::on_dmsList_itemDoubleClicked(QListWidgetItem *item)
{
    try {
        User *user = _sql->getUserByUsername(item->data(Qt::UserRole).toString());
        Profile *profileDialog = new Profile(_sql, _user, user);
        profileDialog->exec();
        loadChatrooms();
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while searching for user!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void MainWindow::on_userBtn_clicked()
{
    EditProfile *editProfileDialog = new EditProfile(_sql, _user);
    editProfileDialog->exec();
    try {
        _user = _sql->getUserByUsername(_user->username());
        ui->nameLabel->setText(_user->getDisplayName());
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while loading account information!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}
