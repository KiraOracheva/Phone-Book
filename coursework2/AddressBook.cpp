#include "AddressBook.h"


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

void AddressBook::addContact() {
    Contact newContact;

    auto validateAndSet = [&](QString line, std::string& target, auto checkFunction, const QString& errorMsg) {
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
            QMessageBox::warning(this, tr("Error"), tr("Invalid phone number: %1").arg(phone));
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
    QMessageBox::information(this, tr("Add Contacts"), tr("Contact is completed!"));
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

    // Используем function для универсальной валидации
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


AddressBook::AddressBook(QWidget* parent) : QWidget(parent), managerContact("contacts.txt") {
    // Настройка палитры
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
    addButton = new QPushButton(tr("Add"));
    displayButton = new QPushButton(tr("Show"));
    deleteButton = new QPushButton(tr("Delete"));
    editButton = new QPushButton(tr("Edit"));
    searchButton = new QPushButton(tr("Search"));
    sortButton = new QPushButton(tr("Sort"));


    //стиль для кнопок
    QString buttonStyle = "QPushButton {background-color: #e0e0e0; border: 1px solid #c0c0c0; border-radius: 5px; padding: 5px;}"
        "QPushButton:hover {background-color: #d0d0d0;} "
        "QPushButton:pressed {background-color: #c0c0c0;}";

    addButton->setStyleSheet(buttonStyle);
    displayButton->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);
    editButton->setStyleSheet(buttonStyle);
    searchButton->setStyleSheet(buttonStyle);
    sortButton->setStyleSheet(buttonStyle);

    //создание группы кнопок действий
    QGroupBox* buttonGroup = new QGroupBox(tr("Actions"));
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(displayButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(sortButton);
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
    connect(displayButton, &QPushButton::clicked, this, &AddressBook::showContacts);
    connect(deleteButton, &QPushButton::clicked, this, &AddressBook::deleteContact);
    connect(editButton, &QPushButton::clicked, this, &AddressBook::editContact);
    connect(searchButton, &QPushButton::clicked, this, &AddressBook::searchContact);
    connect(sortButton, &QPushButton::clicked, this, &AddressBook::sortContact);
}
