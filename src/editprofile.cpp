/*
 * Author: Rui Pereira
 *
 * This file generates the EditProfile dialog window
 *
 * May 01, 2021
 */
#include "editprofile.h"
#include "user.h"
#include "ui_editprofile.h"

#include <QMessageBox>
#include <iostream>

EditProfile::EditProfile(SQL *sql, User *user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditProfile)
{
    ui->setupUi(this);
    _sql = sql;
    _user = user;
    ui->firstNameEdit->setText(_user->getFirstName());
    ui->lastNameEdit->setText(_user->getLastName());
}

EditProfile::~EditProfile()
{
    delete ui;
}

void EditProfile::on_saveBtn_clicked()
{
    QString firstName = ui->firstNameEdit->text().simplified();
    QString lastName = ui->lastNameEdit->text().simplified();
    if(firstName == "" || lastName == "") {
        QMessageBox messageBox;
        messageBox.setText("Empty fields!");
        messageBox.exec();
        return;
    }
    _user->setFirstName(firstName);
    _user->setLastName(lastName);
    try {
        _sql->updateUser(_user);
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while updating account!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
    close();
}
