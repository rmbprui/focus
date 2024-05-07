/*
 * Author: Rui Pereira
 *
 * This file generates the authentication window
 *
 * February 21, 2021
 */
#include "auth.h"
#include "mainwindow.h"
#include "ui_auth.h"

#include <QCryptographicHash>
#include <QMessageBox>
#include <iostream>

const QString kSaltKey = "E1F53135E559C253";

const QString kDefaultHost = "localhost";
const int kDefaultPort = 1883;

Auth::Auth(SQL *sql, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Auth)
{
    _sql = sql;
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

Auth::~Auth()
{
    delete ui;
}

bool validateName(QString firstName, QString lastName)
{
    QRegExp firstNameREX("^[^\\^~`!@#\\$%^*_+\\[\\]{}\\\\:;?|><=]+$");
    firstNameREX.setPatternSyntax(QRegExp::RegExp);
    return firstNameREX.exactMatch(firstName) && firstNameREX.exactMatch(lastName);
}

bool validateUsername(QString username)
{
    QRegExp usernameREX("^(?!.*\\.\\.)(?!.*\\.$)[^\\W][\\w.]{0,29}$");
    usernameREX.setCaseSensitivity(Qt::CaseInsensitive);
    usernameREX.setPatternSyntax(QRegExp::RegExp);
    return usernameREX.exactMatch(username);
}


bool validatePassword(QString password)
{
    QRegExp passwordREX("((?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[\\W]).{8,64})");
    passwordREX.setPatternSyntax(QRegExp::RegExp);
    return passwordREX.exactMatch(password);
}

bool validateEmail(QString email)
{
    QRegExp mailREX("\\b[\\w\\.-]+@[\\w\\.-]+\\.\\w{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);
    return mailREX.exactMatch(email);
}

QString Auth::validateLogin(QString login, QString password, QString encryptedPassword)
{
    if(login == "" || password == "") {
        return "Empty fields";
    }
    if(!validateEmail(login) && !validateUsername(login))
        return "Invalid Username/Email";

    if(validateEmail(login)) {
        if(!_sql->isValidLoginByEmail(login, encryptedPassword)) {
            return "Invalid login";
        }
    } else {
        if(!_sql->isValidLoginByUsername(login, encryptedPassword)) {
            return "Invalid login";
        }
    }

    return "valid";
}

QString Auth::validateRegister(User *user, QString password, QString confirmPassword)
{
    if(user->username() == ""
            || user->email() == ""
            || password == ""
            || confirmPassword == "") {
        return "Empty fields";
    }

    if(!validateName(user->getFirstName(), user->getLastName()))
        return "Invalid name";
    if(!validateUsername(user->username()))
        return "Invalid username";
    if(!validateEmail(user->email()))
        return "Invalid email";
    if(!validatePassword(password))
        return "Password must contain:"
               "\n - at least 8 characters (max 64)"
               "\n - a mix of uppercase and lowercase letters"
               "\n - at least 1 number"
               "\n - at least 1 special characters";

    if(confirmPassword != password) {
        return "Passwords don't match";
    }

    return "valid";
}

void Auth::on_loginBtn_clicked()
{
    QString login = ui->loginEdit->text().toLower();
    QString password = ui->passwordLoginEdit->text();
    QByteArray passwordSalting(password.toStdString().c_str());
    passwordSalting.append(kSaltKey);
    QString encryptedPassword = QCryptographicHash::hash(passwordSalting, QCryptographicHash::Sha256).toHex() ;

    QString message = validateLogin(login, password, encryptedPassword);

    if(message == "valid")
    {
        User *user;
        try {
            if(validateEmail(login))
                user = _sql->getUserByEmail(login);
            else
                user = _sql->getUserByUsername(login);
            Mosquitto *mainClient = new Mosquitto(kDefaultHost, kDefaultPort, user->username());
            MainWindow *w = new MainWindow(_sql, mainClient, user);
            w->showMaximized();
            close();
        } catch (const std::domain_error &e) {
            QMessageBox messageBox;
            messageBox.setText("Error while searching for user!");
            messageBox.exec();
            std::cout << e.what() << std::endl;
        }
    }
    else
    {
        QMessageBox messageBox = QMessageBox();
        messageBox.setText(message);
        messageBox.exec();
    }
}

void Auth::on_registerBtn_clicked()
{
    QString firstName = ui->firstNameEdit->text();
    QString lastName = ui->lastNameEdit->text();
    QString username = ui->usernameEdit->text().toLower();
    QString email = ui->emailEdit->text().toLower();
    QString password = ui->passwordEdit->text();
    QByteArray passwordSalting(password.toStdString().c_str());
    passwordSalting.append(kSaltKey);
    QString encryptedPassword = QCryptographicHash::hash(passwordSalting, QCryptographicHash::Sha256).toHex() ;
    QString confirmPassword = ui->confirmEdit->text();

    User *user = new User(username, email, firstName, lastName);
    QString check = validateRegister(user, password, confirmPassword);
    if(check == "valid") {
        try {
            _sql->addUser(user, encryptedPassword);
        } catch (const std::domain_error &e) {
            QMessageBox messageBox;
            messageBox.setText("Error while adding user!");
            messageBox.exec();
            std::cout << e.what() << std::endl;
        }
        ui->stackedWidget->setCurrentIndex(0);
    } else {
        QMessageBox messageBox = QMessageBox();
        messageBox.setText(check);
        messageBox.exec();
    }
}

void Auth::on_registerLinkBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Auth::on_loginLinkBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
