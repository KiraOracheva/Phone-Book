#pragma once
#include "Header.h"
#include "Contact.h"

class ManagerContact {
public:
    const string filename = "contacts.txt";

    ManagerContact(const QString& filename);

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

    void FindContacts();

};
