#include "ContactDB.h"

void Contact::Show() const
{
    cout << "���: " << FirstName << ", �������: " << LastName << ", ��������: " << MiddleName
        << ", ���� ��������: " << BirthDate << ", �����: " << Address << ", Email: " << Email << ", ��������: ";
    for (const auto& phone : PhoneNumbers) {
        cout << phone << " ";
    }
    cout << endl;
}
