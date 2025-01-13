#pragma once

#include "HeaderDB.h"
#include "ContactDB.h"

class ManagerContact {
public:
    const string filename = "contactss.txt";

    ManagerContact(const QString& filename);

    ManagerContact(const QString& dbFilename, bool isDatabase);

    vector<Contact>& GetContacts();

    vector<Contact> contacts;

    bool CheckName(const wstring& name);

    bool CheckName(const string& name);

    bool CheckEmail(const string& email);

    bool CheckPhone(const string& phone);

    bool CheckDate(const string& date);

public:
    void AddContact(const Contact& contact);

    void LoadContacts();

    void SaveToFile(const QString& filename) const;

    void LoadFromFile(const QString& filename);

    void CreatingDatabase(const QString& dbFilename);

    void SaveToDatabase(const QString& dbFilename) const;

    void LoadFromDatabase(const QString& dbFilename);

    void FindContacts();

    int FindContactsDB(const string& lastName) const;
};
