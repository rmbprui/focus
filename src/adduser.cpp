/*
 * Author: Rui Pereira
 *
 * This file generates the AddUser Dialog window
 *
 * March 28, 2021
 */
#include "adduser.h"
#include "sql.h"
#include "ui_adduser.h"

#include <QMessageBox>
#include <iostream>

AddUser::AddUser(SQL *sql, User *user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);

    _sql = sql;
    _user = user;
}

AddUser::~AddUser()
{
    delete ui;
}

void AddUser::on_addBtn_clicked()
{
    QString message;
    QString contactUsername = ui->usernameInput->text().simplified();
    if(contactUsername != "") {
        QVector<User> contacts = {};
        try {
            QVector<User> contacts = _sql->getUserContacts(_user);

            bool add = true;
            for(auto each : contacts) {
                if(each.username() == contactUsername) {
                    add = false;
                }
            }
            if(add) {
                _sql->addContactByUsername(_user, contactUsername);
                message = "User added successfully!";
            } else message = "You are already connected with: " + contactUsername;
        } catch (const std::domain_error &e) {
            message = "Error adding user: " + contactUsername;
            std::cout << e.what() << std::endl;
        }
    } else message = "Please provide a username";

    QMessageBox messageBox;
    messageBox.setText(message);
    messageBox.exec();
}

void AddUser::on_doneBtn_clicked()
{
    close();
}
