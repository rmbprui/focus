/*
 * Author: Rui Pereira
 *
 * This file generates the profile dialog window
 *
 * February 21, 2021
 */
#ifndef PROFILE_H
#define PROFILE_H

#include <QDialog>
#include "sql.h"
#include "user.h"

namespace Ui { class Profile; }

/**
 * @brief The Profile class
 *
 * This class is used to create the profile dialog window
 */
class Profile : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for Profile class
     * @param sql
     * @param currentUser
     * @param user
     * @param parent
     */
    explicit Profile(SQL *sql, User *currentUser, User *user, QWidget *parent = nullptr);
    ~Profile();

private slots:
    void on_removeBtn_clicked();

    void on_addBtn_clicked();

private:
    Ui::Profile *ui;
    SQL *_sql;
    User *_currentUser;
    User *_user;
};

#endif // PROFILE_H
