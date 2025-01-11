#pragma once
#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include "HeaderQt.h"
#include "Contact.h"
#include "ManagerContact.h"

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
    QPushButton* displayButton;
    QPushButton* displayButton2;
    QPushButton* deleteButton;
    QPushButton* editButton;
    QPushButton* searchButton;
    QPushButton* sortButton;

    ManagerContact managerContact;
    vector<Contact> contacts;


    string RemoveSpaces(const string& str);

    string CombiningStrings(const std::vector<std::string>& vec, const std::string& delimiter);

private slots:
    void showContacts();

    void addContact();
   
    void editContact();

    void deleteContact();

    void searchContact();

    void sortContact();
};

#endif