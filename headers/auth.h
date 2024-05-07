/*
 * Author: Rui Pereira
 *
 * This file generates the authentication window
 *
 * February 21, 2021
 */
#ifndef AUTH_H
#define AUTH_H

#include "sql.h"

#include <QDialog>

namespace Ui { class Auth; }

/**
 * @brief The Auth class
 *
 * This class is used to create the authentication page
 */
class Auth : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for Auth
     * @param sql
     * @param parent
     */
    explicit Auth(SQL *sql, QWidget *parent = nullptr);
    ~Auth();

    /**
     * @brief validateLogin - validates the credentials provided by the user
     * and checks if it is a valid login
     * @param email
     * @param password
     * @param encryptedPassword
     * @return
     */
    QString validateLogin(QString email, QString password, QString encryptedPassword);
    /**
     * @brief validateRegister - validates the information provided by the user
     * @param user
     * @param password
     * @param confirmPassword
     * @return
     */
    QString validateRegister(User *user, QString password, QString confirmPassword);

private slots:
    void on_loginBtn_clicked();

    void on_registerLinkBtn_clicked();

    void on_loginLinkBtn_clicked();

    void on_registerBtn_clicked();

private:
    Ui::Auth *ui;
    SQL *_sql;
};

#endif // AUTH_H
