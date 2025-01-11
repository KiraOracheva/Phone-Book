#pragma once

#include "Header.h"

struct Contact {
    string FirstName;
    string LastName;
    string MiddleName;
    string BirthDate;
    string Address;
    string Email;
    vector<std::string> PhoneNumbers;

    void Show() const;
};