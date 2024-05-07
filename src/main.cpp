/*
 * Author: Rui Pereira
 *
 * This file contains the main() funtion, it is called when the program runs
 *
 * February 21, 2021
 */
#include "mainwindow.h"
#include "auth.h"
#include "sql.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    SQL *sql = new SQL();

    QApplication a(argc, argv);
    Auth *auth = new Auth(sql);
    auth->show();
    return a.exec();
}
