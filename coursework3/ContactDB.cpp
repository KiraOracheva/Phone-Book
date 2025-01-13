#include "ContactDB.h"

void Contact::Show() const
{
    cout << "Имя: " << FirstName << ", Фамилия: " << LastName << ", Отчество: " << MiddleName
        << ", Дата рождения: " << BirthDate << ", Адрес: " << Address << ", Email: " << Email << ", Телефоны: ";
    for (const auto& phone : PhoneNumbers) {
        cout << phone << " ";
    }
    cout << endl;
}
