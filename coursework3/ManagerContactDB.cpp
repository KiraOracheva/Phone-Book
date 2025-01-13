#include "ManagerContactDB.h"

vector<Contact>& ManagerContact::GetContacts() {
    return contacts;
}

ManagerContact::ManagerContact(const QString& filename) {

    try {
        LoadFromFile(filename);

    }
    catch (const exception& e) {
        cerr << "Error loading contacts: " << e.what() << endl;
    }
}

ManagerContact::ManagerContact(const QString& dbFilename, bool isDatabase) {

    try {
        LoadFromDatabase(dbFilename);

    }
    catch (const exception& e) {
        cerr << "Error loading contacts: " << e.what() << endl;
    }
}


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
    string emailTrimmed = email;
    emailTrimmed.erase(remove(emailTrimmed.begin(), emailTrimmed.end(), ' '), emailTrimmed.end());
    regex regexEmail(R"(^[a-zA-Z0-9]+(\.[a-zA-Z0-9]+)*@[a-zA-Z0-9]+(\.[a-zA-Z0-9]+)+$)");
    return regex_match(emailTrimmed, regexEmail);
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


void ManagerContact::AddContact(const Contact& contact) {
    contacts.push_back(contact);
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

void ManagerContact::SaveToFile(const QString& filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw std::runtime_error("Unable to open file for writing");
    }

    QTextStream out(&file);
    for (const auto& contact : contacts) {
        QStringList phoneNumbersList;
        for (const auto& phoneNumber : contact.PhoneNumbers) {
            phoneNumbersList.append(QString::fromStdString(phoneNumber));
        }
        QString phoneNumbers = phoneNumbersList.join(',');

        out << QString::fromStdString(contact.FirstName) << ";"
            << QString::fromStdString(contact.LastName) << ";"
            << QString::fromStdString(contact.MiddleName) << ";"
            << QString::fromStdString(contact.Address) << ";"
            << QString::fromStdString(contact.BirthDate) << ";"
            << QString::fromStdString(contact.Email) << ";"
            << phoneNumbers << "\n";
    }

    file.close();
}

void ManagerContact::LoadFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Unable to open file for reading");
    }

    QTextStream in(&file);
    contacts.clear();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(';');

        if (parts.size() < 6) continue;

        // Extract all parts (with bounds checking for optional fields)
        QString firstName = parts[0];
        QString lastName = parts[1];
        QString middleName = parts[2];
        QString address = parts[3];
        QString birthDate = parts[4];
        QString email = parts[5];
        QString phoneNumbersStr = (parts.size() > 6) ? parts[6] : "";


        // Split phone numbers and process them
        std::vector<std::string> phoneNumbers;
        if (!phoneNumbersStr.isEmpty())
        {
            QStringList phoneNumbersList = phoneNumbersStr.split(',');
            for (const QString& phone : phoneNumbersList) {
                if (!phone.isEmpty())
                    phoneNumbers.push_back(phone.toStdString());
            }
        }

        // Create Contact object and append to vector
        contacts.emplace_back(Contact{
            firstName.toStdString(),
            lastName.toStdString(),
            middleName.toStdString(),
            address.toStdString(),
            birthDate.toStdString(),
            email.toStdString(),
            phoneNumbers
            });
    }

    file.close();
}

void ManagerContact::CreatingDatabase(const QString& dbFilename) {
    sqlite3* db;
    int rc = sqlite3_open(dbFilename.toStdString().c_str(), &db);

    if (rc) {
        throw runtime_error("Cannot open database: " + string(sqlite3_errmsg(db)));
    }


    string sql = R"(
        CREATE TABLE IF NOT EXISTS Contacts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            firstName TEXT,
            lastName TEXT,
            middleName TEXT,
            birthDate TEXT,
            address TEXT,
            email TEXT,
            phoneNumbers TEXT
        );
    )";


    char* errMsg = nullptr;
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        string error = "SQL error: " + string(errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        throw runtime_error(error);
    }
    sqlite3_close(db);
}

void ManagerContact::SaveToDatabase(const QString& dbFilename) const {
    sqlite3* db;
    int rc = sqlite3_open(dbFilename.toStdString().c_str(), &db);

    if (rc) {
        throw std::runtime_error("Cannot open database: " + string(sqlite3_errmsg(db)));
    }

    string sql = R"(
        INSERT INTO Contacts (firstName, lastName, middleName, birthDate, address, email, phoneNumbers)
        VALUES (?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    for (const auto& contact : contacts) {
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
        }

        sqlite3_bind_text(stmt, 1, contact.FirstName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, contact.LastName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, contact.MiddleName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, contact.BirthDate.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, contact.Address.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, contact.Email.c_str(), -1, SQLITE_STATIC);


        std::ostringstream phoneStream;
        for (size_t i = 0; i < contact.PhoneNumbers.size(); ++i) {
            phoneStream << contact.PhoneNumbers[i];
            if (i < contact.PhoneNumbers.size() - 1) {
                phoneStream << ",";
            }
        }
        sqlite3_bind_text(stmt, 7, phoneStream.str().c_str(), -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            throw runtime_error("Failed to execute statement: " + string(sqlite3_errmsg(db)));
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
}

void ManagerContact::LoadFromDatabase(const QString& dbFilename) {
    sqlite3* db;
    int rc = sqlite3_open(dbFilename.toStdString().c_str(), &db);

    if (rc) {
        throw runtime_error("Cannot open database: " + string(sqlite3_errmsg(db)));
    }

    string sql = "SELECT firstName, lastName, middleName, birthDate, address, email, phoneNumbers FROM Contacts;";
    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
    }

    contacts.clear();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Contact contact;
        contact.FirstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        contact.LastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        contact.MiddleName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        contact.BirthDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        contact.Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        contact.Email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        const char* phoneNumbersText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        string phoneNumbers = phoneNumbersText ? phoneNumbersText : "";
        istringstream phoneStream(phoneNumbers);
        string phone;
        while (getline(phoneStream, phone, ',')) {
            if (!phone.empty()) {
                contact.PhoneNumbers.push_back(phone);
            }
        }

        contacts.push_back(contact);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
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

int ManagerContact::FindContactsDB(const string& lastName) const {
    sqlite3* db;
    int rc = sqlite3_open("contacts.db", &db);

    if (rc) {
        throw runtime_error("Cannot open database: " + string(sqlite3_errmsg(db)));
    }

    string sql = "SELECT id FROM Contacts WHERE lastName LIKE ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
    }
    sqlite3_bind_text(stmt, 1, lastName.c_str(), -1, SQLITE_STATIC);


    int contactId = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        contactId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return contactId;
}