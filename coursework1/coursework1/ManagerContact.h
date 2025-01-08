#pragma once

#include "Header.h"
#include "Contact.h"

class ManagerContact {
private:
    vector<Contact> contacts;
    const string filename = "contacts.txt";

    //�������� ����� �� ������������ �������
    bool CheckName(const wstring& name);

    bool CheckName(const string& name);

    //�������� ������ ����� �� ������������ �������
    bool CheckEmail(const string& email);

    //�������� ������ �������� �� ������������ �������
    bool CheckPhone(const string& phone);

    //�������� ���� �������� �� ������������ �������
    bool CheckDate(const string& date);

public:

    //������ ��������� �� �����
    void LoadContacts();

    //���������� ���������
    void SavingContacts();

    //���������� ������ ��������
    void AddContact();

    //����� ���� ���������
    void ShowContacts();

    //�������� ��������
    void DeleteContact();

    //��������� ��������
    void ChangeContact();

    //����� ���������
    void FindContacts();

    //���������� ��������� �� ���������� ����
    void SortContacts();
};