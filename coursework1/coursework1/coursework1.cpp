#include <iostream>
#include "Contact.h"
#include "ManagerContact.h"

int main() {
    setlocale(LC_ALL, "Russian");
    ManagerContact MyManager;
    MyManager.LoadContacts();

    while (true) {

        cout << "1. Создать контакт\n"
             << "2. Вывести все контакты\n"
             << "3. Удалить контакт\n"
             << "4. Редактировать контакт\n"
             << "5. Поиск контактов\n"
             << "6. Сортировать контакты\n"
             << "7. Выход\n\n"
             << "Выберите действие: ";

        int choice;
        cin >> choice;
        cout << endl;
        cin.ignore(); //игнорируем символ новой строки после ввода числа

        switch (choice) {
        case 1:
            MyManager.AddContact();
            break;
        case 2:
            MyManager.ShowContacts();
            break;
        case 3:
            MyManager.DeleteContact();
            break;
        case 4:
            MyManager.ChangeContact();
            break;
        case 5:
            MyManager.FindContacts();
            break;
        case 6:
            MyManager.SortContacts();
            break;
        case 7:
            return 0;
        default:
            cout << "Некорректный выбор действия!" << endl;
        }
    }
    return 0;
}