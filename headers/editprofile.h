/*
 * Author: Rui Pereira
 *
 * This file generates the EditProfile dialog window
 *
 * May 01, 2021
 */
#ifndef EDITPROFILE_H
#define EDITPROFILE_H

#include <QDialog>
#include "sql.h"
#include "user.h"

namespace Ui { class EditProfile; }

/**
 * @brief The EditProfile class
 *
 * This class is used to create the EditProfile dialog window
 */
class EditProfile : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for EditProfile class
     * @param user
     * @param parent
     */
    explicit EditProfile(SQL *sql, User *user, QWidget *parent = nullptr);
    ~EditProfile();

private slots:
    void on_saveBtn_clicked();

private:
    Ui::EditProfile *ui;
    SQL *_sql;
    User *_user;
};

#endif // EDITPROFILE_H
