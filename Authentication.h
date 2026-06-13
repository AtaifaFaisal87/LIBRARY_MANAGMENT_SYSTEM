#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <iostream>
#include "Utils.h"
#include "Exceptions.h"
using namespace std;

// Forward declarations
class User;
class library;
class librarian;

// ============================================================
//                    AUTHENTICATION CLASS
//============================================================
class Authentication
{
private:
    librarian *lib_admin;

public:
    Authentication(librarian *l = nullptr)
    {
        lib_admin = l;
    }

    void setLibrarian(librarian *l)
    {
        lib_admin = l;
    }

    User *loginUser(library *lib);
    librarian *loginLibrarian();

    void logout()
    {
        cout << "LOGGED OUT SUCCESSFULLY!\n";
    }
};

#endif // AUTHENTICATION_H
