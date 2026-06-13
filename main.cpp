//g++ -o library_system main.cpp Utils.cpp Library.cpp User.cpp Librarian.cpp Authentication.cpp FileIO.cpp Fines.cpp Statics.cpp
// ./library_system.exe

#include <iostream>
#include "Library.h"
#include "Authentication.h"
#include "Librarian.h"
#include "FileIO.h"
#include "Utils.h"
#include "Exceptions.h"
using namespace std;

int main()
{
    library lib;

    // Using RuntimeLogger to suppress output during file loading
    RuntimeLogger::enableSilentMode();
    try
    {
        loadUsersFromFile(lib.getUsersVector());
    }
    catch (FileException &e)
    {
        cout << e.what() << endl;
    }

    int maxID = 0;
    for (int i = 0; i < lib.getUserCount(); i++)
    {
        if (lib.getUserByIndex(i)->get_userid() > maxID)
            maxID = lib.getUserByIndex(i)->get_userid();
    }
    User::setNextID(lib.getUserCount() + 1);

    try
    {
        loadBooksFromFile(lib.getCatalog());
    }
    catch (FileException &e)
    {
        cout << e.what() << endl;
    }

    try
    {
        loadTransactionsFromFile(lib.getTransactionsVector(), lib.getUsersVector());
    }
    catch (FileException &e)
    {
        cout << e.what() << endl;
    }

    try
    {
        loadFinesFromFile(lib.getFinesVector());
    }
    catch (FileException &e)
    {
        cout << "FINES FILE ERROR: " << e.what() << endl;
    }

    try
    {
        loadTablesFromFile(lib.getTablesVector());
    }
    catch (FileException &e)
    {
        cout << "TABLES FILE ERROR: " << e.what() << endl;
    }

    try
    {
        loadReservationsFromFile(lib.getReservationsVector());
    }
    catch (FileException &e)
    {
        cout << "RESERVATIONS FILE ERROR: " << e.what() << endl;
    }

    lib.autoExpireReservations();
    RuntimeLogger::disableSilentMode();

    // ===== SETUP LIBRARIAN =====
    librarian admin(1, "Admin", "admin@lib.com", "03000000000");
    admin.setCredentials("admin", "123");

    Authentication auth(&admin);
    int choice = 0;
    do
    {
        cout << "\n================ LIBRARY SYSTEM ================" << endl;
        cout << "1. REGISTER USER" << endl;
        cout << "2. LOGIN AS USER" << endl;
        cout << "3. LOGIN AS LIBRARIAN" << endl;
        cout << "4. EXIT" << endl;

        choice = getValidIntInRange("\nENTER CHOICE: ", 1, 4);

        switch (choice)
        {
        case 1:
            try
            {
                lib.registerUserInLibrary();
            }
            catch (UserException &e)
            {
                cout << e.what() << endl;
            }
            break;
        case 2:
        {
            try
            {
                User *u = auth.loginUser(&lib);
                if (u)
                {
                    u->userMenu(&lib);
                    auth.logout();
                }
            }
            catch (AuthException &e)
            {
                cout << e.what() << endl;
            }
            break;
        }
        case 3:
        {
            try
            {
                librarian *l = auth.loginLibrarian();
                if (l)
                {
                    l->librarianMainMenu(&lib);
                    auth.logout();
                }
            }
            catch (AuthException &e)
            {
                cout << e.what() << endl;
            }
            break;
        }
        case 4:
            cout << "EXITING SYSTEM..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 4);

    return 0;
}
