#pragma once

#include "Header.h"
#include "Contact.h"

class ManagerContact {
private:
    vector<Contact> contacts;
    const string filename = "contacts.txt";

    //проверка имени на соответствие шаблону
    bool CheckName(const wstring& name);

    bool CheckName(const string& name);

    //проверка адреса почты на соответствие шаблону
    bool CheckEmail(const string& email);

    //проверка номера телефона на соответствие шаблону
    bool CheckPhone(const string& phone);

    //проверка даты рождения на соответствие шаблону
    bool CheckDate(const string& date);

public:

    //чтение контактов из файла
    void LoadContacts();

    //сохранение контактов
    void SavingContacts();

    //добавление нового контакта
    void AddContact();

    //вывод всех контактов
    void ShowContacts();

    //удаление контакта
    void DeleteContact();

    //изменение контакта
    void ChangeContact();

    //поиск контактов
    void FindContacts();

    //сортировка контактов по выбранному полю
    void SortContacts();
};