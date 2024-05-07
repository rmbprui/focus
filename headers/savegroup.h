/*
 * Author: Rui Pereira
 *
 * This file generates the SaveGroup dialog window
 *
 * March 28, 2021
 */
#ifndef SAVEGROUP_H
#define SAVEGROUP_H

#include <QAbstractButton>
#include <QDialog>
#include <QListWidget>

#include "sql.h"
#include "user.h"

namespace Ui { class SaveGroup; }

/**
 * @brief The Profile class
 *
 * This class is used to create the SaveGroup dialog window
 */
class SaveGroup : public QDialog
{
    Q_OBJECT

public:

    bool _deleted = false;
    /**
     * @brief Constructor for SaveGroup class
     * @param sql
     * @param user
     * @param group
     * @param parent
     */
    explicit SaveGroup(SQL *sql, User *user, Chatroom *group = nullptr, QWidget *parent = nullptr);
    ~SaveGroup();

    /**
     * @brief createNewGroup - creates a new group
     */
    void createNewGroup();
    /**
     * @brief updateGroup - updates the group's information
     */
    void updateGroup();

    /**
     * @brief loadContactsList - loads the user's contacts
     */
    void loadContactsList();
    /**
     * @brief loadMembersList - loads the group's members
     */
    void loadMembersList();

    // Getters/Setters

    Chatroom *group() const;
    void setGroup(Chatroom *group);

private slots:

    void on_addMemberBtn_clicked();

    void on_removeMemberBtn_clicked();

    void on_saveBtn_clicked();

    void on_deleteBtn_clicked();

private:
    Ui::SaveGroup *ui;
    SQL *_sql;
    User *_user;
    Chatroom *_group = nullptr;
    QVector<User> _members;
    QVector<User> _contactsList;
};

#endif // SAVEGROUP_H
