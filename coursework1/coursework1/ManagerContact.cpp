#include "ManagerContact.h"

bool ManagerContact::CheckName(const wstring& name)
{
    wregex regexName(L"^[A-Za-zА-Яа-яЁё][A-Za-zА-Яа-яЁё0-9\\s-]*[A-Za-zА-Яа-яЁё0-9]$");
    return regex_match(name, regexName);
}

bool ManagerContact::CheckName(const string& name)
{
    regex regexName("^[A-Za-zА-Яа-яЁё][A-Za-zА-Яа-яЁё0-9\\s-]*[A-Za-zА-Яа-яЁё0-9]$");
    return regex_match(name, regexName);
}

bool ManagerContact::CheckEmail(const string& email)
{
    regex regexEmail(R"((\w+)(\.{0,1}\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, regexEmail);
}

bool ManagerContact::CheckPhone(const string& phone)
{
    regex regexPhone(R"(\+?\d{1,3}[- ]?\(?\d{1,4}?\)?[- ]?\d{1,4}[- ]?\d{1,4})");
    return regex_match(phone, regexPhone);
}

bool ManagerContact::CheckDate(const string& date)
{
    int year, month, day;
    char dash1, dash2;

    istringstream ss(date);
    if (!(ss >> year >> dash1 >> month >> dash2 >> day) ||
        dash1 != '-' || dash2 != '-') {
        return false; //некорректный формат даты
    }

    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return false; //некорректные значения месяца или дня
    }

    time_t t = time(nullptr);
    tm now;

#ifdef _MSC_VER
    localtime_s(&now, &t);
#else
    now = *localtime(&t);
#endif

    if (year > now.tm_year + 1900 ||
        (year == now.tm_year + 1900 && month > now.tm_mon + 1) ||
        (year == now.tm_year + 1900 && month == now.tm_mon + 1 && day > now.tm_mday)) {
        return false; //дата из будущего
    }

    //проверка високосного года
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) {
            return false; 
        }
    }

    //проверка месяцев с 30 днями
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return false; 
        }
    }

    return true;
}


void ManagerContact::LoadContacts()
{
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        Contact contact;
        stringstream ss(line);
        string field;

        auto readField = [&](string& target) {
            if (getline(ss, field, ',')) {
                target = field;
            }
            else {
                target = "";
            }
        };

        readField(contact.FirstName);
        readField(contact.LastName);
        readField(contact.MiddleName);
        readField(contact.BirthDate);
        readField(contact.Address);
        readField(contact.Email);

        string phones;
        if (getline(ss, phones)) {
            stringstream phoneStream(phones);
            string phone;
            while (getline(phoneStream, phone, ' ')) {
                if (!phone.empty())
                    contact.PhoneNumbers.push_back(phone);
            }
        }
        contacts.push_back(contact);
    }
    file.close();
}

void ManagerContact::SavingContacts()
{
    ofstream file(filename);
    for (const auto& contract : contacts) {
        file << contract.FirstName << "," << contract.LastName << ","
             << contract.MiddleName << "," << contract.BirthDate << "," 
             << contract.Address << "," << contract.Email << ",";
        for (const auto& phone : contract.PhoneNumbers) {
            file << phone << " ";
        }
        file << endl;
    }
    file.close();
}

void ManagerContact::AddContact()
{
    Contact contact;
    string input;

    while (true) {
        cout << "\nВведите Имя: ";
        getline(cin, input);
        if (CheckName(wstring(input.begin(), input.end()))) {
            contact.FirstName = input;
            break;
        }
        else {
            cout << "Введено некорректное имя!" << endl;
        }
    }

    while (true) {
        cout << "\nВведите Фамилию: ";
        getline(cin, input);
        if (CheckName(wstring(input.begin(), input.end()))) {
            contact.LastName = input;
            break;
        }
        else {
            cout << "Введена некорректная фамилия!" << endl;
        }
    }

    while (true) {
        cout << "\nВведите Отчество: ";
        getline(cin, input);
        if (CheckName(wstring(input.begin(), input.end()))) {
            contact.MiddleName = input;
            break;
        }
        else {
            cout << "Введено некорректное отчество!" << endl;
        }
    }

    while (true) {
        cout << "\nВведите Дату рождения (YYYY-MM-DD): ";
        getline(cin, input);
        if (CheckDate(input)) {
            contact.BirthDate = input;
            break;
        }
        else {
            cout << "Введена некорректная дата рождения!" << endl;
        }
    }

    cout << "\nВведите Адрес: ";
    getline(cin, contact.Address);

    while (true) {
        cout << "\nВведите Email: ";
        getline(cin, input);
        if (CheckEmail(input)) {
            contact.Email = input;
            break;
        }
        else {
            cout << "Введен некорректный email!" << endl;
        }
    }

    string phone;
    cout << "\nВведите телефонные номера (введите 'end' для завершения): " << endl;
    while (true) {
        getline(cin, phone);
        if (phone == "end") break;
        if (CheckPhone(phone)) {
            contact.PhoneNumbers.push_back(phone);
        }
        else {
            cout << "Введен некорректный номер телефона!" << endl;
        }
    }

    contacts.push_back(contact);
    SavingContacts();
    cout << "\nКонтакт был успешно добавлен!\n" << endl;
}

void ManagerContact::ShowContacts()
{
    for (const auto& contract : contacts) {
        contract.Show();
    }
}

void ManagerContact::DeleteContact()
{
    string lastName;
    cout << "\nВведите фамилию для удаления: ";
    getline(cin, lastName);

    vector<Contact>::iterator it = contacts.begin();
    while (it != contacts.end()) {
        if (it->LastName == lastName) {
            it = contacts.erase(it); 
        }
        else {
            ++it; 
        }
    }

    SavingContacts();
    cout << "\nКонтакт(ы) успешно удален(ы)!" << endl;
}

void ManagerContact::ChangeContact()
{
    string lastName;
    cout << "\nВведите фамилию для редактирования: ";
    getline(cin, lastName);

    for (auto& contact : contacts) {
        if (contact.LastName == lastName) {
            cout << "\nРедактирование записи:" << endl;
            contact.Show();

            string input;

            cout << "\nВведите новое Имя (или оставьте пустым для пропуска): ";
            getline(cin, input);
            if (!input.empty()) {
                if (CheckName(wstring(input.begin(), input.end())))
                    contact.FirstName = input;
                else
                    cout << "\nВведено некорректное имя! Имя не будет изменено" << endl;
            }

            cout << "\nВведите новую Фамилию (или оставьте пустым для пропуска): ";
            getline(cin, input);
            if (!input.empty()) {
                if (CheckName(wstring(input.begin(), input.end())))
                    contact.LastName = input;
                else
                    cout << "\nВведена некорректная фамилия! Фамилия не будет изменена" << endl;
            }

            cout << "\nВведите новое Отчество (или оставьте пустым для пропуска): ";
            getline(cin, input);
            if (!input.empty()) {
                if (CheckName(wstring(input.begin(), input.end())))
                    contact.MiddleName = input;
                else
                    cout << "\nВведено некорректное отчество! Отчество не будет изменено" << endl;
            }

            cout << "\nВведите новый Адрес (или оставьте пустым для пропуска): ";
            getline(cin, input);
            if (!input.empty()) contact.Address = input;

            cout << "\nВведите новую Дату рождения (YYYY-MM-DD, или оставьте пустым для пропуска): ";
            getline(cin, input);
            if (!input.empty()) {
                if (CheckDate(input))
                    contact.BirthDate = input;
                else
                    cout << "\nВведена некорректная дата рождения! Дата рождения не будет изменена" << endl;
            }

            cout << "\nВведите новый Email (или оставьте пустым для пропуска): ";
            getline(cin, input);
            if (!input.empty()) {
                if (CheckEmail(input))
                    contact.Email = input;
                else
                    cout << "\nВведен некорректный email! Email не будет изменен" << endl;
            }

            cout << "\nВведите новые телефонные номера (введите 'end' для завершения): " << endl;
            contact.PhoneNumbers.clear();
            string phone;
            while (true) {
                getline(cin, phone);
                if (phone == "end") break;
                if (CheckPhone(phone)) {
                    contact.PhoneNumbers.push_back(phone);
                }
                else {
                    cout << "\nНекорректный номер телефона!" << endl;
                }
            }

            SavingContacts();
            cout << "\nКонтакт успешно отредактирован!" << endl;
            return;
        }
    }
    cout << "\nЗапись с такой фамилией не найдена!" << endl;
}

void ManagerContact::FindContacts()
{
    string query;
    cout << "\nВведите строку для поиска: ";
    getline(cin, query);

    for (const auto& contact : contacts) {
        bool found = false;
        if (contact.FirstName.find(query) != string::npos) {
            found = true;
        }
        if (!found && contact.LastName.find(query) != string::npos) {
            found = true;
        }
        if (!found && contact.MiddleName.find(query) != string::npos) {
            found = true;
        }
        if (!found && contact.Address.find(query) != string::npos) {
            found = true;
        }
        if (!found && contact.Email.find(query) != string::npos) {
            found = true;
        }

        if (found) {
            contact.Show();
        }
    }
}

void ManagerContact::SortContacts()
{
    int choice;
    cout << "\nВыберите поле для сортировки:\n";
    cout << "1. Имя\n";
    cout << "2. Фамилия\n";
    cout << "3. Отчество\n";
    cout << "4. Дата рождения\n";
    cout << "5. Email\n";
    cout << "6. Адрес\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    cin.ignore();

    function<bool(const Contact&, const Contact&)> compareFunc;

    switch (choice) {
    case 1:
        compareFunc = [](const Contact& a, const Contact& b) {
            return a.FirstName < b.FirstName;
            };
        break;
    case 2:
        compareFunc = [](const Contact& a, const Contact& b) {
            return a.LastName < b.LastName;
            };
        break;
    case 3:
        compareFunc = [](const Contact& a, const Contact& b) {
            return a.MiddleName < b.MiddleName;
            };
        break;
    case 4:
        compareFunc = [](const Contact& a, const Contact& b) {
            return a.BirthDate < b.BirthDate;
            };
        break;
    case 5:
        compareFunc = [](const Contact& a, const Contact& b) {
            return a.Email < b.Email;
            };
        break;
    case 6:
        compareFunc = [](const Contact& a, const Contact& b) {
            return a.Address < b.Address;
            };
        break;
    default:
        cout << "\nНеверный выбор. Сортировка не выполнена.\n" << endl;
        return;
    }

    if (compareFunc)
        sort(contacts.begin(), contacts.end(), compareFunc);

    cout << "\nКонтакты отсортированы!\n" << endl;

    ShowContacts();

    cout << endl;
}