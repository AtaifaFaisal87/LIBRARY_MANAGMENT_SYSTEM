#include "Authentication.h"
#include "Library.h"
#include "User.h"
#include "Librarian.h"

User *Authentication::loginUser(library *lib)
{
    cout << "===== USER LOGIN =====" << endl;
    string username = getValidString("ENTER USERNAME: ");
    string password = getValidString("ENTER PASSWORD: ");

    for (int i = 0; i < lib->getUserCount(); i++)
    {
        User *u = lib->getUserByIndex(i);
        if (u->getUsername() == username && u->getPassword() == password)
        {
            if (u->getIsDeleted())
                throw AuthException("THIS ACCOUNT HAS BEEN DELETED. PLEASE CONTACT THE LIBRARIAN.");
            cout << "LOGIN SUCCESSFUL! WELCOME " << u->get_name() << endl;
            return u;
        }
    }
    throw AuthException("INVALID USERNAME OR PASSWORD!");
}

librarian *Authentication::loginLibrarian()
{
    if (!lib_admin)
        throw AuthException("NO LIBRARIAN REGISTERED IN THE SYSTEM!");

    cout << "\n===== LIBRARIAN LOGIN =====" << endl;
    string uname = getValidString("ENTER USERNAME: ");
    string pass = getValidString("ENTER PASSWORD: ");

    if (lib_admin->getUsername() == uname && lib_admin->getPassword() == pass)
    {
        cout << "LIBRARIAN LOGIN SUCCESSFUL!" << endl;
        return lib_admin;
    }
    throw AuthException("INVALID LIBRARIAN CREDENTIALS!");
}
