/*
 * Author: Rui Pereira
 *
 * This file generates the SaveGroup dialog window
 *
 * March 28, 2021
 */
#include "savegroup.h"
#include "sql.h"
#include "user.h"
#include "ui_savegroup.h"

#include <QMessageBox>
#include <iostream>

SaveGroup::SaveGroup(SQL *sql, User *user, Chatroom *group, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveGroup)
{
    ui->setupUi(this);

    _sql = sql;
    _user = user;
    _contactsList = {};
    _members = {};

    ui->deleteBtn->hide();
    if(group != nullptr) {
        ui->deleteBtn->show();
        _group = group;
        ui->nameInput->setText(_group->name());
        _members = group->members();
    } else {
        _members.append(*_user);
    }
    loadMembersList();
    loadContactsList();
}

SaveGroup::~SaveGroup()
{
    delete ui;
}

void SaveGroup::loadContactsList()
{
    _contactsList.clear();
    ui->contactsList->clear();
    try {
        QVector<User> contacts = _sql->getUserContacts(_user);

        for(auto each : contacts) {
            bool addItem = true;
            for(auto member : _members) {
                if(member.username() == each.username())
                    addItem = false;
            }
            if (addItem)
            {
                ui->contactsList->addItem(each.getDisplayName() + " (" + each.username() + ")");
                _contactsList.append(each);
            }
        }
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while loading contacts!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void SaveGroup::loadMembersList()
{
    ui->membersList->clear();
    for(auto member : _members) {
        ui->membersList->addItem(member.getDisplayName() + " (" + member.username() + ")");
    }
}

void SaveGroup::createNewGroup()
{
    QString name = ui->nameInput->text().simplified();
    if(name == "") {
        QMessageBox messageBox;
        messageBox.setText("Please provide a name!");
        messageBox.exec();
        return;
    }

    QString topic = name.simplified().remove(' ').toLower();
    try {
        topic = _sql->validateTopic(topic);
    } catch (const std::domain_error &e) {
        std::cout << e.what() << std::endl;
    }
    try {
        _sql->addGroup(name, topic, *_user);
        for (auto member : _members) {
            User *contactUser = _sql->getUserByUsername(member.username());
            _sql->addSubscription(topic, contactUser);
        }
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while creating group!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void SaveGroup::updateGroup()
{
    QString name = ui->nameInput->text().simplified();
    if(name == "") {
        QMessageBox messageBox;
        messageBox.setText("Please provide a name!");
        messageBox.exec();
        return;
    }
    _group->setName(name);
    _group->setMembers(_members);
    try {
        _sql->updateGroup(_group);
    } catch (const std::domain_error &e) {
        QMessageBox messageBox;
        messageBox.setText("Error while updating group!");
        messageBox.exec();
        std::cout << e.what() << std::endl;
    }
}

void SaveGroup::on_saveBtn_clicked()
{
    if(_group == nullptr)
    {
        createNewGroup();
        close();
    } else {
        updateGroup();
        close();
    }
}

Chatroom *SaveGroup::group() const
{
    return _group;
}

void SaveGroup::setGroup(Chatroom *group)
{
    _group = group;
}

void SaveGroup::on_addMemberBtn_clicked()
{
    if(ui->contactsList->currentRow() != -1) {
        User contactObj = _contactsList.at(ui->contactsList->currentRow());
        _members.append(contactObj);
        loadMembersList();
        loadContactsList();
    }
}

void SaveGroup::on_removeMemberBtn_clicked()
{
    if(ui->membersList->currentRow() != -1) {
        if(_members.at(ui->membersList->currentRow()).username() != _user->username()) {
            _members.removeAt(ui->membersList->currentRow());
            loadMembersList();
            loadContactsList();
        }
    }
}

void SaveGroup::on_deleteBtn_clicked()
{
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this,
                                    "Delete Group", "Are you sure you want to delete the group?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            _sql->removeGroup(_group);
        } catch (const std::domain_error &e) {
            QMessageBox messageBox;
            messageBox.setText("Error while deleting group!");
            messageBox.exec();
            std::cout << e.what() << std::endl;
        }
        _deleted = true;
        close();
    }
}
