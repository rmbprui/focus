/*
 * Author: Rui Pereira
 *
 * This file generates the AddUser Dialog window
 *
 * March 28, 2021
 */
#ifndef ADDUSER_H
#define ADDUSER_H

#include <QAbstractButton>
#include <QDialog>
#include "sql.h"

namespace Ui { class AddUser; }

/**
 * @brief The AddUser class
 *
 * This class is used to generate the AddUser Dialog window
 */
class AddUser : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for AddUser class
     * @param sql
     * @param user
     * @param parent
     */
    explicit AddUser(SQL *sql, User *user, QWidget *parent = nullptr);
    ~AddUser();

private slots:

    void on_addBtn_clicked();

    void on_doneBtn_clicked();

private:
    Ui::AddUser *ui;
    SQL *_sql;
    User *_user;
};

#endif // ADDUSER_H
