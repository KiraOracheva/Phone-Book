#include "ManagerContact.h"

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


bool ManagerContact::CheckName(const wstring& name)
{
    wregex regexName(L"^[A-Za-z�-��-���][A-Za-z�-��-���0-9\\s-]*[A-Za-z�-��-���0-9]$");
    return regex_match(name, regexName);
}

bool ManagerContact::CheckName(const string& name)
{
    regex regexName("^[A-Za-z�-��-���][A-Za-z�-��-���0-9\\s-]*[A-Za-z�-��-���0-9]$");
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
        return false; //������������ ������ ����
    }

    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return false; //������������ �������� ������ ��� ���
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
        return false; //���� �� ��������
    }

    //�������� ����������� ����
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) {
            return false;
        }
    }

    //�������� ������� � 30 �����
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

void ManagerContact::FindContacts()
{
    string query;
    cout << "\n������� ������ ��� ������: ";
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
