/*
 * Author: Rui Pereira
 *
 * This file generates the application's main window
 *
 * February 21, 2021
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sql.h"
#include "user.h"
#include "mosquitto.h"
#include "chatroom.h"

#include <QElapsedTimer>
#include <QListWidget>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class
 *
 * This class is used to create the application's main window page using QMainWindow
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for MainWindow class
     * @param sql
     * @param mainClient
     * @param user
     * @param parent
     */
    MainWindow(SQL *sql, Mosquitto *mainClient, User *user, QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief newChatroomWindow - creates new ChatroomWindow Widgets
     * @param chatroom
     */
    void newChatroomWindow(Chatroom chatroom);
    /**
     * @brief loadGroupsList - loads the user's groups
     */
    void loadGroupsList();
    /**
     * @brief loadDmsList - loads the user's direct messages
     */
    void loadDmsList();
    /**
     * @brief loadChatrooms - loads all the Chatroom classes (groups and dms)
     */
    void loadChatrooms();

private slots:

    void timeout();

    void on_actionLogout_triggered();

    void on_actionExit_triggered();

    void on_actionProfile_triggered();

    void on_createGroupBtn_clicked();

    void on_addUserBtn_clicked();

    void on_groupsList_itemSelectionChanged();

    void on_dmsList_itemSelectionChanged();

    void on_refreshBtn_clicked();

    void on_dmsList_itemDoubleClicked(QListWidgetItem *item);

    void on_userBtn_clicked();

private:
    Ui::MainWindow *ui;
    SQL *_sql;
    User *_user;
    Mosquitto *_mainClient;
};
#endif // MAINWINDOW_H
