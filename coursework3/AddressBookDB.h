#pragma once

#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include "HeaderQt.h"
#include "ContactDB.h"
#include "ManagerContactDB.h"


class QLabel;
class QLineEdit;
class QTextEdit;

class AddressBook : public QWidget
{
    Q_OBJECT

public:
    AddressBook(QWidget* parent = 0);

private:
    QLineEdit* FirstNameLine;
    QLineEdit* LastNameLine;
    QLineEdit* MiddleNameLine;
    QLineEdit* AddressLine;
    QLineEdit* BirthDateLine;
    QLineEdit* EmailLine;
    QTextEdit* NumbersText;

    QPushButton* addButton;
    QPushButton* addButtonDB;
    QPushButton* displayButton;
    QPushButton* displayButtonDB;
    QPushButton* deleteButton;
    QPushButton* deleteButtonDB;
    QPushButton* editButton;
    QPushButton* searchButton;
    QPushButton* sortButton;
    QPushButton* clearButton;

    ManagerContact managerContact;
    vector<Contact> contacts;


    string RemoveSpaces(const string& str);

    string CombiningStrings(const std::vector<std::string>& vec, const std::string& delimiter);

private slots:
    void showContacts();

    void showContactsDB();

    void addContact();

    void addContactDB();

    void editContact();

    void deleteContact();

    void deleteContactDB();

    void deleteContactFromDB(int contactId);

    void searchContact();

    void sortContact();

    void clearContact();


    void showCustomMessage(const QString& title, const QString& message);
};

#endif