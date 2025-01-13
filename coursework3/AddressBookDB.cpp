#include "AddressBookDB.h"


string AddressBook::RemoveSpaces(const string& str) {
    regex re(R"(^\s+|\s+$)");
    return regex_replace(str, re, "");
}

string AddressBook::CombiningStrings(const vector<string>& elements, const string& delimiter) {
    ostringstream oss;
    bool first = true;
    for (const auto& element : elements) {
        if (!first) {
            oss << delimiter;
        }
        oss << element;
        first = false;
    }
    return oss.str();
}


void AddressBook::showContacts() {
    QString contactsInfo;

    for (const auto& contact : managerContact.GetContacts()) {
        QString contactInfo = QString("Name: %1\nSurname: %2\nMiddle name: %3\nBirthday: %4\nAddress: %5\nEmail: %6\nPhone numbers: %7\n")
            .arg(QString::fromStdString(contact.FirstName))
            .arg(QString::fromStdString(contact.LastName))
            .arg(QString::fromStdString(contact.MiddleName))
            .arg(QString::fromStdString(contact.BirthDate))
            .arg(QString::fromStdString(contact.Address))
            .arg(QString::fromStdString(contact.Email))
            .arg(QString::fromStdString(CombiningStrings(contact.PhoneNumbers, ", ")));
        contactsInfo += contactInfo + "\n";
    }

    if (contactsInfo.isEmpty()) {
        QMessageBox::information(this, tr("Contacts"), tr("No contacts available."));
    }
    else {

        QTextEdit* contactsTextEdit = new QTextEdit();
        contactsTextEdit->setText(contactsInfo);
        contactsTextEdit->setReadOnly(true);

        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidget(contactsTextEdit);
        scrollArea->setWidgetResizable(true);

        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(scrollArea);

        QWidget* contactsWindow = new QWidget();
        contactsWindow->setLayout(layout);
        contactsWindow->setWindowTitle("Contacts");
        contactsWindow->show();
    }
}

void AddressBook::showContactsDB() {
    sqlite3* db;
    int rc = sqlite3_open("contacts.db", &db);

    if (rc) {
        showCustomMessage("Error", "Can't open database: " + QString::fromStdString(sqlite3_errmsg(db)));
        return;
    }

    const char* selectQuery = "SELECT firstName, lastName, middleName, address, birthDate, email, phoneNumbers FROM contacts;";
    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(db, selectQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        showCustomMessage("Error", "Failed to fetch " + QString::fromStdString(sqlite3_errmsg(db)));
        sqlite3_close(db);
        return;
    }


    QDialog* contactsDialog = new QDialog(this);
    contactsDialog->setWindowTitle(tr("Contacts from DB"));
    QVBoxLayout* mainLayout = new QVBoxLayout();

    QTableWidget* contactsTable = new QTableWidget();
    contactsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    contactsTable->setColumnCount(7);
    contactsTable->setHorizontalHeaderLabels({
        tr("Name"), tr("Surname"), tr("Middle Name"),
        tr("Birthday"), tr("Address"), tr("Email"),
        tr("Phone Numbers")
        });
    int row = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        contactsTable->insertRow(row);

        contactsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)))));
        contactsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))));
        contactsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)))));
        contactsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))));
        contactsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)))));
        contactsTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)))));
        contactsTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)))));
        row++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (contactsTable->rowCount() == 0) {
        showCustomMessage(tr("Contacts"), tr("No contacts available."));
        return;
    }


    contactsTable->resizeColumnsToContents();
    contactsTable->resizeRowsToContents();
    mainLayout->addWidget(contactsTable);
    contactsDialog->setLayout(mainLayout);
    contactsDialog->adjustSize();
    contactsDialog->show();
}

void AddressBook::addContact() {
    Contact newContact;

    auto validateAndSet = [&](QString line, string& target, auto checkFunction, const QString& errorMsg) {
        string value = RemoveSpaces(line.toStdString());
        if (!checkFunction(value)) {
            QMessageBox::warning(this, tr("Error"), errorMsg.arg(line));
            return false;
        }
        target = value;
        return true;
        };

    if (!validateAndSet(FirstNameLine->text(), newContact.FirstName, [&](const string& name) {return managerContact.CheckName(name); }, tr("Uncorrect name"))) return;
    if (!validateAndSet(LastNameLine->text(), newContact.LastName, [&](const string& name) {return managerContact.CheckName(name); }, tr("Uncorrect lastname"))) return;
    if (!validateAndSet(MiddleNameLine->text(), newContact.MiddleName, [&](const string& name) {return managerContact.CheckName(name); }, tr("Uncorrect middlename"))) return;
    if (!validateAndSet(BirthDateLine->text(), newContact.BirthDate, [&](const string& date) {return managerContact.CheckDate(date); }, tr("Uncorrect birthday"))) return;
    if (!validateAndSet(AddressLine->text(), newContact.Address, [&](const string& address) {return true; }, tr("Uncorrect address"))) return;
    if (!validateAndSet(EmailLine->text(), newContact.Email, [&](const string& email) {return managerContact.CheckEmail(email); }, tr("Uncorrect email"))) return;

    QString phoneInput = NumbersText->toPlainText();
    QStringList phoneList = phoneInput.split("\n", QString::SkipEmptyParts);
    for (const auto& phone : phoneList) {
        string phoneStr = RemoveSpaces(phone.toStdString());
        if (managerContact.CheckPhone(phoneStr)) {
            newContact.PhoneNumbers.push_back(phoneStr);
        }
        else {
            showCustomMessage(tr("Error"), tr("Invalid phone number: %1").arg(phone));
            return;
        }
    }

    managerContact.AddContact(newContact);
    managerContact.SaveToFile("contacts.txt");

    FirstNameLine->clear();
    LastNameLine->clear();
    MiddleNameLine->clear();
    BirthDateLine->clear();
    AddressLine->clear();
    EmailLine->clear();
    NumbersText->clear();
    showCustomMessage(tr("Add Contacts"), tr("Contact is completed!"));
}

void AddressBook::addContactDB() {
    Contact newContact;

    auto validateAndSet = [&](QLineEdit* lineEdit, std::string& target, auto checkFunction, const QString& errorMsg) {
        std::string value = RemoveSpaces(lineEdit->text().toStdString());
        if (!checkFunction(value)) {
            showCustomMessage(tr("Error"), errorMsg.arg(lineEdit->text()));
            return false; 
        }
        target = value;
        return true; 
        };

    if (!validateAndSet(FirstNameLine, newContact.FirstName, [&](const std::string& name) {return managerContact.CheckName(name); }, tr("Uncorrect name"))) return;
    if (!validateAndSet(LastNameLine, newContact.LastName, [&](const std::string& name) {return managerContact.CheckName(name); }, tr("Uncorrect lastname"))) return;
    if (!validateAndSet(MiddleNameLine, newContact.MiddleName, [&](const std::string& name) {return managerContact.CheckName(name); }, tr("Uncorrect middlename"))) return;
    if (!validateAndSet(BirthDateLine, newContact.BirthDate, [&](const std::string& date) {return managerContact.CheckDate(date); }, tr("Uncorrect birthday"))) return;
    if (!validateAndSet(AddressLine, newContact.Address, [&](const std::string& address) {return true; }, tr("Uncorrect address"))) return;
    if (!validateAndSet(EmailLine, newContact.Email, [&](const std::string& email) {return managerContact.CheckEmail(email); }, tr("Uncorrect email"))) return;

    QString phoneInput = NumbersText->toPlainText();
    QStringList phoneList = phoneInput.split("\n", QString::SkipEmptyParts);
    for (const auto& phone : phoneList) {
        std::string phoneStr = RemoveSpaces(phone.toStdString());
        if (managerContact.CheckPhone(phoneStr)) {
            newContact.PhoneNumbers.push_back(phoneStr);
        }
        else {
            showCustomMessage(tr("Error"), tr("Invalid phone number: %1").arg(phone));
            return;
        }
    }

    sqlite3* db;
    managerContact.CreatingDatabase("contacts.db");
    int rc = sqlite3_open("contacts.db", &db);
    if (rc) {
        showCustomMessage(tr("Error"), tr("Can't open database: %1").arg(QString::fromStdString(sqlite3_errmsg(db))));
        return;
    }

    string insertQuery = "INSERT INTO contacts (firstName, lastName, middleName, birthDate, address, email, phoneNumbers) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, insertQuery.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        showCustomMessage(tr("Error"), tr("Failed to prepare statement: %1").arg(QString::fromStdString(sqlite3_errmsg(db))));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, newContact.FirstName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, newContact.LastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, newContact.MiddleName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, newContact.BirthDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, newContact.Address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, newContact.Email.c_str(), -1, SQLITE_STATIC);
    std::string phones = CombiningStrings(newContact.PhoneNumbers, ", ");
    sqlite3_bind_text(stmt, 7, phones.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        showCustomMessage(tr("Error"), tr("Failed to insert %1").arg(QString::fromStdString(sqlite3_errmsg(db))));
    }
    else {
        showCustomMessage(tr("Success"), tr("Contact added successfully!"));
    }


    sqlite3_finalize(stmt);
    sqlite3_close(db);
    FirstNameLine->clear();
    LastNameLine->clear();
    MiddleNameLine->clear();
    BirthDateLine->clear();
    AddressLine->clear();
    EmailLine->clear();
    NumbersText->clear();
}

void AddressBook::editContact() {
    QString lastNameToEdit = QInputDialog::getText(this, tr("Edit Contact"), tr("Enter surname to edit:"));
    if (lastNameToEdit.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No last name provided."));
        return;
    }

    auto& contacts = managerContact.GetContacts();
    auto it = find_if(contacts.begin(), contacts.end(),
        [&lastNameToEdit](const Contact& c) {
            return QString::fromStdString(c.LastName).compare(lastNameToEdit, Qt::CaseInsensitive) == 0;
        });

    if (it == contacts.end()) {
        QMessageBox::warning(this, tr("Error"), tr("Contact not found."));
        return;
    }

    Contact& contact = *it;
    QDialog editDialog(this);
    editDialog.setWindowTitle(tr("Edit Contact"));

    QGridLayout* layout = new QGridLayout;
    int row = 0;

    //используем function для универсальной валидации
    auto createLineEditAndConnect = [&](QString initialValue, QString labelText, function<bool(const std::string&)> validator, QString errorMessage) -> QLineEdit* {
        QLineEdit* lineEdit = new QLineEdit(initialValue);
        QObject::connect(lineEdit, &QLineEdit::editingFinished, [&, labelText, validator, errorMessage, lineEdit]() {
            if (!validator(lineEdit->text().toStdString())) {
                QMessageBox::warning(this, tr("Error"), errorMessage.arg(lineEdit->text()));
            }
            });
        layout->addWidget(new QLabel(labelText), row, 0);
        layout->addWidget(lineEdit, row, 1);
        row++;
        return lineEdit;
        };

    QString firstName = QString::fromStdString(contact.FirstName);
    QLineEdit* firstNameEdit = createLineEditAndConnect(firstName, tr("First Name:"), [&](const string& name) { return managerContact.CheckName(name); }, tr("Incorrect first name"));

    QString lastName = QString::fromStdString(contact.LastName);
    QLineEdit* lastNameEdit = createLineEditAndConnect(lastName, tr("Surname:"), [&](const string& name) { return managerContact.CheckName(name); }, tr("Incorrect last name"));

    QString middleName = QString::fromStdString(contact.MiddleName);
    QLineEdit* middleNameEdit = createLineEditAndConnect(middleName, tr("Middle Name:"), [&](const string& name) { return managerContact.CheckName(name); }, tr("Incorrect middle name"));

    QString birthDate = QString::fromStdString(contact.BirthDate);
    QLineEdit* birthDateEdit = createLineEditAndConnect(birthDate, tr("Birth Date (YYYY-MM-DD):"), [&](const string& date) { return managerContact.CheckDate(date); }, tr("Incorrect birthday"));

    QString address = QString::fromStdString(contact.Address);
    QLineEdit* addressEdit = createLineEditAndConnect(address, tr("Address:"), [](const string& addr) { return true; }, tr("Incorrect address"));

    QString email = QString::fromStdString(contact.Email);
    QLineEdit* emailEdit = createLineEditAndConnect(email, tr("Email:"), [&](const string& email) { return managerContact.CheckEmail(email); }, tr("Incorrect email"));

    QTextEdit* phoneNumbersEdit = new QTextEdit(QString::fromStdString(CombiningStrings(contact.PhoneNumbers, "\n")));
    layout->addWidget(new QLabel(tr("Phone Numbers (one per line):")), row, 0);
    layout->addWidget(phoneNumbersEdit, row, 1);
    row++;

    QPushButton* saveButton = new QPushButton(tr("Save"));
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));

    layout->addWidget(saveButton, row, 0);
    layout->addWidget(cancelButton, row, 1);

    editDialog.setLayout(layout);
    QObject::connect(cancelButton, &QPushButton::clicked, &editDialog, &QDialog::reject);
    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        QString newFirstName = firstNameEdit->text();
        QString newLastName = lastNameEdit->text();
        QString newMiddleName = middleNameEdit->text();
        QString newBirthDate = birthDateEdit->text();
        QString newAddress = addressEdit->text();
        QString newEmail = emailEdit->text();
        QStringList updatedPhones = phoneNumbersEdit->toPlainText().split("\n", QString::SkipEmptyParts);
        bool isValid = true;
        QString errorMessage;
        if (newFirstName.isEmpty() || !managerContact.CheckName(newFirstName.toStdString())) {
            errorMessage += tr("Invalid or empty first name\n");
            isValid = false;
        }
        if (newLastName.isEmpty() || !managerContact.CheckName(newLastName.toStdString())) {
            errorMessage += tr("Invalid or empty surname\n");
            isValid = false;
        }
        if (!newMiddleName.isEmpty() && !managerContact.CheckName(newMiddleName.toStdString())) {
            errorMessage += tr("Invalid middle name\n");
            isValid = false;
        }
        if (!newBirthDate.isEmpty() && !managerContact.CheckDate(newBirthDate.toStdString())) {
            errorMessage += tr("Invalid birth date. Use YYYY-MM-DD format\n");
            isValid = false;
        }
        if (newAddress.isEmpty()) {
            errorMessage += tr("Address cannot be empty\n");
            isValid = false;
        }
        if (!newEmail.isEmpty() && !managerContact.CheckEmail(newEmail.toStdString())) {
            errorMessage += tr("Invalid email address\n");
            isValid = false;
        }
        vector<string> validPhones;
        for (const auto& phone : updatedPhones) {
            string phoneStr = RemoveSpaces(phone.toStdString());
            if (managerContact.CheckPhone(phoneStr)) {
                validPhones.push_back(phoneStr);
            }
            else {
                errorMessage += tr("Invalid phone number: %1\n").arg(phone);
                isValid = false;
            }
        }
        if (!isValid) {
            QMessageBox::warning(&editDialog, tr("Error"), errorMessage);
            return;
        }
        contact.FirstName = newFirstName.toStdString();
        contact.LastName = newLastName.toStdString();
        contact.MiddleName = newMiddleName.toStdString();
        contact.BirthDate = newBirthDate.toStdString();
        contact.Address = newAddress.toStdString();
        contact.Email = newEmail.toStdString();
        contact.PhoneNumbers = validPhones;
        managerContact.SaveToFile("contacts.txt");
        QMessageBox::information(&editDialog, tr("Success"), tr("Contact updated successfully!"));
        editDialog.accept();
        });
    editDialog.exec();
}

void AddressBook::deleteContact() {
    QString nameToDelete = QInputDialog::getText(this, tr("Delete Contact"), tr("Enter the first name of the contact to delete:"));

    if (nameToDelete.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No name provided."));
        return;
    }

    auto& contacts = managerContact.GetContacts();
    auto it = remove_if(contacts.begin(), contacts.end(),
        [&](const Contact& contact) {
            return QString::fromStdString(contact.FirstName).compare(nameToDelete, Qt::CaseInsensitive) == 0;
        });

    if (it == contacts.end()) {
        QMessageBox::warning(this, tr("Error"), tr("Contact not found"));
    }
    else {
        contacts.erase(it, contacts.end());
        QMessageBox::information(this, tr("Success"), tr("Contact deleted successfully"));
    }
}

void AddressBook::deleteContactDB() {
    QString lastNameToDelete = QInputDialog::getText(this, tr("Delete Contact"), tr("Enter the last name of the contact to delete:"));

    if (lastNameToDelete.isEmpty()) {
        showCustomMessage(tr("Error"), tr("No last name provided."));
        return;
    }

    int contactId = managerContact.FindContactsDB(lastNameToDelete.toStdString());
    if (contactId == -1) {
        showCustomMessage(tr("Error"), tr("Contact with last name '%1' not found.").arg(lastNameToDelete));
        return;
    }

    try {
        deleteContactFromDB(contactId);
        showCustomMessage(tr("Success"), tr("Contact deleted successfully."));
    }
    catch (const runtime_error& error) {
        showCustomMessage(tr("Error"), tr("Failed to delete contact from DB: %1").arg(error.what()));
    }
}

void AddressBook::deleteContactFromDB(int contactId) {
    sqlite3* db;
    int rc = sqlite3_open("contacts.db", &db);

    if (rc) {
        throw runtime_error("Cannot open database: " + string(sqlite3_errmsg(db)));
    }

    string sql = "DELETE FROM Contacts WHERE id = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw runtime_error("Failed to prepare statement: " + string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_int(stmt, 1, contactId);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        throw runtime_error("Failed to execute delete statement: " + string(sqlite3_errmsg(db)));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void AddressBook::searchContact() {
    QString query = QInputDialog::getText(this, tr("Search Contact"), tr("Enter query string:"));

    if (query.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No query provided"));
        return;
    }

    QString results;
    auto& contacts = managerContact.GetContacts();

    for (const auto& contact : contacts) {

        vector<QString> fields = {
              QString::fromStdString(contact.FirstName),
              QString::fromStdString(contact.LastName),
              QString::fromStdString(contact.MiddleName),
              QString::fromStdString(contact.Address),
              QString::fromStdString(contact.Email),
              QString::fromStdString(CombiningStrings(contact.PhoneNumbers, ", "))
        };

        if (any_of(fields.begin(), fields.end(),
            [&](const QString& field) {
                return field.contains(query, Qt::CaseInsensitive);
            }))
        {
            QString contactInfo = QString("Name: %1\nSurname: %2\nMiddle name: %3\nBirthday: %4\nAddress: %5\nEmail: %6\nPhone Numbers: %7\n")
                .arg(QString::fromStdString(contact.FirstName))
                .arg(QString::fromStdString(contact.LastName))
                .arg(QString::fromStdString(contact.MiddleName))
                .arg(QString::fromStdString(contact.BirthDate))
                .arg(QString::fromStdString(contact.Address))
                .arg(QString::fromStdString(contact.Email))
                .arg(QString::fromStdString(CombiningStrings(contact.PhoneNumbers, ", ")));
            results += contactInfo + "\n";
        }
    }

    if (results.isEmpty()) {
        QMessageBox::information(this, tr("Search Results"), tr("No contacts found."));
    }
    else {
        QMessageBox::information(this, tr("Search Results"), results);
    }
}

void AddressBook::sortContact() {
    QStringList sortOptions = { tr("Name"), tr("Surname"), tr("Middle name"), tr("Birth date"), tr("Address"), tr("Email") };

    bool ok;
    QString choice = QInputDialog::getItem(this, tr("Sort Contacts"),
        tr("Choose a field to sort by:"), sortOptions, 0, false, &ok);
    if (!ok || choice.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No option selected. Sorting cancelled"));
        return;
    }

    enum class SortField {
        Name,
        Surname,
        MiddleName,
        BirthDate,
        Address,
        Email,
        Invalid
    };

    SortField sortField;
    if (choice == tr("Name")) {
        sortField = SortField::Name;
    }
    else if (choice == tr("Surname")) {
        sortField = SortField::Surname;
    }
    else if (choice == tr("Middle name")) {
        sortField = SortField::MiddleName;
    }
    else if (choice == tr("Birth date")) {
        sortField = SortField::BirthDate;
    }
    else if (choice == tr("Address")) {
        sortField = SortField::Address;
    }
    else if (choice == tr("Email")) {
        sortField = SortField::Email;
    }
    else {
        sortField = SortField::Invalid;
    }

    auto& contacts = managerContact.GetContacts();

    switch (sortField) {
    case SortField::Name:
        std::sort(contacts.begin(), contacts.end(),
            [](const Contact& a, const Contact& b) { return a.FirstName < b.FirstName; });
        break;
    case SortField::Surname:
        std::sort(contacts.begin(), contacts.end(),
            [](const Contact& a, const Contact& b) { return a.LastName < b.LastName; });
        break;
    case SortField::MiddleName:
        std::sort(contacts.begin(), contacts.end(),
            [](const Contact& a, const Contact& b) { return a.MiddleName < b.MiddleName; });
        break;
    case SortField::BirthDate:
        std::sort(contacts.begin(), contacts.end(),
            [](const Contact& a, const Contact& b) { return a.BirthDate < b.BirthDate; });
        break;
    case SortField::Address:
        std::sort(contacts.begin(), contacts.end(),
            [](const Contact& a, const Contact& b) { return a.Address < b.Address; });
        break;
    case SortField::Email:
        std::sort(contacts.begin(), contacts.end(),
            [](const Contact& a, const Contact& b) { return a.Email < b.Email; });
        break;
    case SortField::Invalid:
        QMessageBox::warning(this, tr("Error"), tr("Invalid option. Sorting cancelled."));
        return;
    }

    QMessageBox::information(this, tr("Success"), tr("Contacts sorted successfully!"));
    managerContact.SaveToFile("contacts.txt");
}

void AddressBook::clearContact()
{
    FirstNameLine->clear();
    LastNameLine->clear();
    MiddleNameLine->clear();
    AddressLine->clear();
    BirthDateLine->clear();
    EmailLine->clear();
    NumbersText->clear();
}


AddressBook::AddressBook(QWidget* parent) : QWidget(parent), managerContact("contacts.txt") {
    //настройка палитры
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(240, 240, 240));
    palette.setColor(QPalette::WindowText, Qt::black);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    //поля ввода
    QLabel* firstnameLabel = new QLabel(tr("Name:"));
    FirstNameLine = new QLineEdit;

    QLabel* lastnameLabel = new QLabel(tr("Surname:"));
    LastNameLine = new QLineEdit;

    QLabel* middlenameLabel = new QLabel(tr("Middle name:"));
    MiddleNameLine = new QLineEdit;

    QLabel* birthdateLabel = new QLabel(tr("Birthday(YYYY-MM-DD):"));
    BirthDateLine = new QLineEdit;

    QLabel* addressLabel = new QLabel(tr("Adress:"));
    AddressLine = new QLineEdit;

    QLabel* emailLabel = new QLabel(tr("Email:"));
    EmailLine = new QLineEdit;

    QLabel* numbersLabel = new QLabel(tr("Phone numbers:"));
    NumbersText = new QTextEdit;

    //создание группы полей ввода
    QGroupBox* inputGroup = new QGroupBox(tr("Contact Details"));
    QVBoxLayout* inputLayout = new QVBoxLayout;
    inputLayout->addWidget(firstnameLabel);
    inputLayout->addWidget(FirstNameLine);
    inputLayout->addWidget(lastnameLabel);
    inputLayout->addWidget(LastNameLine);
    inputLayout->addWidget(middlenameLabel);
    inputLayout->addWidget(MiddleNameLine);
    inputLayout->addWidget(birthdateLabel);
    inputLayout->addWidget(BirthDateLine);
    inputLayout->addWidget(addressLabel);
    inputLayout->addWidget(AddressLine);
    inputLayout->addWidget(emailLabel);
    inputLayout->addWidget(EmailLine);
    inputLayout->addWidget(numbersLabel);
    inputLayout->addWidget(NumbersText);
    inputGroup->setLayout(inputLayout);

    //кнопки действий
    addButton = new QPushButton(tr("Add(File)"));
    addButtonDB = new QPushButton(tr("Add(DB)"));
    displayButton = new QPushButton(tr("Show(File)"));
    displayButtonDB = new QPushButton(tr("Show(DB)"));
    deleteButton = new QPushButton(tr("Delete(File)"));
    editButton = new QPushButton(tr("Edit"));
    deleteButtonDB = new QPushButton(tr("Delete(DB)"));
    searchButton = new QPushButton(tr("Search"));
    sortButton = new QPushButton(tr("Sort"));
    clearButton = new QPushButton(tr("Clear"));

    //стиль для кнопок
    QString buttonStyle = "QPushButton {background-color: #e0e0e0; border: 1px solid #c0c0c0; border-radius: 5px; padding: 5px;}"
        "QPushButton:hover {background-color: #d0d0d0;} "
        "QPushButton:pressed {background-color: #c0c0c0;}";

    addButton->setStyleSheet(buttonStyle);
    addButtonDB->setStyleSheet(buttonStyle);
    displayButton->setStyleSheet(buttonStyle);
    displayButtonDB->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);
    editButton->setStyleSheet(buttonStyle);
    deleteButtonDB->setStyleSheet(buttonStyle);
    searchButton->setStyleSheet(buttonStyle);
    sortButton->setStyleSheet(buttonStyle);
    clearButton->setStyleSheet(buttonStyle);


    //cоздание layouts для кнопок
    QHBoxLayout* addButtonLayout = new QHBoxLayout;
    addButtonLayout->addWidget(addButton);
    addButtonLayout->addWidget(addButtonDB);

    QHBoxLayout* displayButtonLayout = new QHBoxLayout;
    displayButtonLayout->addWidget(displayButton);
    displayButtonLayout->addWidget(displayButtonDB);

    QHBoxLayout* editButtonLayout = new QHBoxLayout;
    editButtonLayout->addWidget(deleteButton);
    editButtonLayout->addWidget(editButton);
    editButtonLayout->addWidget(deleteButtonDB);


    QHBoxLayout* searchButtonLayout = new QHBoxLayout;
    searchButtonLayout->addWidget(searchButton);
    searchButtonLayout->addWidget(sortButton);
    searchButtonLayout->addWidget(clearButton);

    //создание группы кнопок действий
    QGroupBox* buttonGroup = new QGroupBox(tr("Actions"));
    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addLayout(addButtonLayout);
    buttonLayout->addLayout(displayButtonLayout);
    buttonLayout->addLayout(editButtonLayout);
    buttonLayout->addLayout(searchButtonLayout);
    buttonGroup->setLayout(buttonLayout);



    //создание основной компоновки
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(buttonGroup);

    //создание виджета вкладок
    QTabWidget* tabWidget = new QTabWidget;
    QWidget* contactsTab = new QWidget;
    contactsTab->setLayout(mainLayout);
    tabWidget->addTab(contactsTab, tr("Contacts"));

    //создание главного окна
    QVBoxLayout* mainWinLayout = new QVBoxLayout(this);
    mainWinLayout->addWidget(tabWidget);

    setLayout(mainWinLayout);
    setWindowTitle(tr("Address Book"));

    //соединения сигналов и слотов
    connect(addButton, &QPushButton::clicked, this, &AddressBook::addContact);
    connect(addButtonDB, &QPushButton::clicked, this, &AddressBook::addContactDB);
    connect(displayButton, &QPushButton::clicked, this, &AddressBook::showContacts);
    connect(displayButtonDB, &QPushButton::clicked, this, &AddressBook::showContactsDB);
    connect(deleteButton, &QPushButton::clicked, this, &AddressBook::deleteContact);
    connect(editButton, &QPushButton::clicked, this, &AddressBook::editContact);
    connect(deleteButtonDB, &QPushButton::clicked, this, &AddressBook::deleteContactDB);
    connect(searchButton, &QPushButton::clicked, this, &AddressBook::searchContact);
    connect(sortButton, &QPushButton::clicked, this, &AddressBook::sortContact);
    connect(clearButton, &QPushButton::clicked, this, &AddressBook::clearContact);
}

void AddressBook::showCustomMessage(const QString& title, const QString& message) {
    QDialog* msgBox = new QDialog(this);
    msgBox->setWindowTitle(title);

    QPalette palette = msgBox->palette();
    palette.setColor(QPalette::Window, QColor(240, 240, 240));
    palette.setColor(QPalette::WindowText, Qt::black);
    msgBox->setPalette(palette);
    msgBox->setAutoFillBackground(true);

    QLabel* messageLabel = new QLabel(message);
    QPushButton* okButton = new QPushButton(tr("OK"));
    okButton->setStyleSheet("QPushButton {background-color: #e0e0e0; border: 1px solid #c0c0c0; border-radius: 5px; padding: 5px;}"
        "QPushButton:hover {background-color: #d0d0d0;} "
        "QPushButton:pressed {background-color: #c0c0c0;}");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch(1);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(messageLabel);
    mainLayout->addLayout(buttonLayout);
    msgBox->setLayout(mainLayout);
    QObject::connect(okButton, &QPushButton::clicked, msgBox, &QDialog::accept);

    msgBox->exec();
    delete msgBox;
}
