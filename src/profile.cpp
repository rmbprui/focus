/*
 * Author: Rui Pereira
 *
 * This file generates the profile dialog window
 *
 * February 21, 2021
 */
#include "profile.h"
#include "user.h"
#include "ui_profile.h"

#include <QMessageBox>
#include <iostream>

Profile::Profile(SQL *sql, User *currentUser, User *user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Profile)
{
    ui->setupUi(this);

    _sql = sql;
    _currentUser = currentUser;
    _user = user;
    ui->nameLabel->setText(_user->getDisplayName());
    ui->usernameLabel->setText("@" + _user->username());
    ui->emailLabel->setText(_user->email());
    ui->removeBtn->hide();
    if(_sql->areContacts(_currentUser, _user)) {
        ui->addBtn->hide();
        ui->removeBtn->show();
    }
}

Profile::~Profile()
{
    delete ui;
}

void Profile::on_removeBtn_clicked()
{
    try {
        _sql->removeContact(_currentUser, _user);
        this->close();
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while removing contact!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void Profile::on_addBtn_clicked()
{
    try {
        _sql->addContactByUsername(_currentUser, _user->username());
        this->close();
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while adding contact!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}
