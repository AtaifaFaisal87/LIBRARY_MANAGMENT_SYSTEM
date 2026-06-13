#include "Librarian.h"
#include "Library.h"
#include "FileIO.h"

void librarian::addbook(Book *b, BookCatalog *catalog)
{
    catalog->addbook(b);
}

void librarian::removeBook(string t, BookCatalog *catalog)
{
    catalog->removeBook(t);
}

//=======================================
//----------LIBRARY MENUS---------------
void librarian::viewFullDetails(library *lib)
{
    int choice = 0;
    do
    {
        cout << "\n=============== LIBRARIAN DASHBOARD ===============" << endl;
        cout << "1. VIEW LIBRARIAN INFO" << endl;
        cout << "2. VIEW ALL USERS" << endl;
        cout << "3. VIEW USER BY TYPE" << endl;
        cout << "4. VIEW ALL TRANSACTIONS" << endl;
        cout << "5. VIEW ALL FINES" << endl;
        cout << "6. VIEW ALL RESERVATIONS" << endl;
        cout << "7. VIEW BOOK CATALOG" << endl;
        cout << "8. VIEW ALL MEMBERSHIPS" << endl;
        cout << "9. EXIT" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 9);

        switch (choice)
        {
        case 1:
            cout << "\n--------------- LIBRARIAN INFO ---------------" << endl;
            cout << *this;
            break;
        case 2:
            cout << "\n--------------- ALL USERS ---------------" << endl;
            lib->viewUsers();
            break;
        case 3:
        {
            cout << "\n--------------- USERS BY TYPE ---------------" << endl;
            int typeChoice = getValidIntInRange("SELECT USER TYPE:\n1. STUDENT\n2. FACULTY\n3. GUEST\nCHOICE: ", 1, 3);
            string type;
            if (typeChoice == 1)
                type = "STUDENT";
            else if (typeChoice == 2)
                type = "FACULTY";
            else
                type = "GUEST";
            lib->viewUsersByType(type);
            break;
        }
        case 4:
            cout << "\n------------ ALL TRANSACTIONS ------------" << endl;
            lib->viewTransactions();
            break;
        case 5:
            cout << "\n---------------- FINES ----------------" << endl;
            lib->viewFines();
            break;
        case 6:
            cout << "\n------------- RESERVATIONS -------------" << endl;
            lib->viewAllReservations();
            break;
        case 7:
            cout << "\n------------- BOOK CATALOG -------------" << endl;
            lib->getCatalog()->ViewBookCatalog();
            break;
        case 8:
            cout << "\n------------ ALL MEMBERSHIPS ------------" << endl;
            lib->viewAllMemberships();
            break;
        case 9:
            cout << "EXITING LIBRARIAN DASHBOARD..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 9);
}

//==================================================================
//   MANAGE BOOKS
void librarian::manageBooks(library *lib)
{
    int choice = 0;
    do
    {
        cout << "\n================ BOOK MANAGEMENT ================" << endl;
        cout << "1. ADD BOOK" << endl;
        cout << "2. REMOVE BOOK" << endl;
        cout << "3. BACK" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 3);

        switch (choice)
        {
        case 1:
        {
            string title = getValidString("ENTER TITLE: ");
            string author = getValidAlphaString("ENTER AUTHOR: ");
            string category = getValidAlphaString("ENTER CATEGORY: ");
            string isbn = getValidISBN("ENTER ISBN: ");
            string publisher = getValidPublisher("ENTER PUBLISHER: ");
            int yop = getValidIntInRange("ENTER YEAR: ", 1000, 2026);
            int copies = getValidIntInRange("ENTER AVAILABLE COPIES: ", 1, 1000);

            try
            {
                Book *existing = lib->getCatalog()->SearchbyTitle(title);
                if (existing != nullptr)
                {
                    cout << "A BOOK WITH THIS TITLE ALREADY EXISTS!" << endl;
                    break;
                }
                Book *b = new Book(title, author, category, isbn, publisher, yop);
                b->setAvailableCopies(copies);
                addbook(b, lib->getCatalog());
                saveBookToFile(b);
                cout << "ALLOTTED ID: " << b->get_bookid() << endl;
            }
            catch (FileException &e)
            {
                cout << e.what() << endl;
            }
            break;
        }
        case 2:
        {
            string title = getValidString("ENTER TITLE TO REMOVE: ");
            try
            {
                removeBook(title, lib->getCatalog());
                rewriteBooksFile(lib->getCatalog());
            }
            catch (FileException &e)
            {
                cout << e.what() << endl;
            }
            break;
        }
        case 3:
            cout << "RETURNING..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 3);
}

//==================================================================
//                 MANAGE TABLES
void librarian::manageTables(library *lib)
{
    int choice;
    do
    {
        cout << "\n================ TABLE MANAGEMENT ================" << endl;
        cout << "TOTAL TABLES: " << lib->getTableCount() << " / " << MAX_TABLES << endl;
        cout << "1. ADD TABLE" << endl;
        cout << "2. VIEW ALL TABLES" << endl;
        cout << "3. REMOVE TABLE" << endl;
        cout << "4. BACK" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 4);

        switch (choice)
        {
        case 1:
        {
            string loc = getValidString("ENTER LOCATION: ");
            int cap = getValidIntInRange("ENTER CAPACITY: ", 1, 100);
            try
            {
                lib->addTable(loc, cap);
            }
            catch (ReservationException &e)
            {
                cout << e.what() << endl;
            }
            break;
        }
        case 2:
            lib->viewTables();
            break;
        case 3:
        {
            lib->viewTables();
            int tid = getValidInt("ENTER TABLE ID TO REMOVE: ");
            lib->removeTable(tid);
            break;
        }
        case 4:
            cout << "RETURNING..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 4);
}

//==================================================================
//                 MANAGE USERS
void librarian::manageUsers(library *lib)
{
    int choice;
    do
    {
        cout << "\n================ USER MANAGEMENT ================" << endl;
        cout << "TOTAL USERS: " << lib->getActiveUserCount() << endl;
        cout << "1. VIEW ALL USERS" << endl;
        cout << "2. DELETE USER BY ID" << endl;
        cout << "3. BACK" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 3);
        cout << endl;

        switch (choice)
        {
        case 1:
            lib->viewUsers();
            break;
        case 2:
        {
            if (lib->getActiveUserCount() == 0)
            {
                cout << "---------------NO USERS TO DELETE!---------------" << endl;
                break;
            }
            lib->viewUsers();
            int uid = getValidInt("ENTER USER ID TO DELETE: ");
            lib->deleteUser(uid,
                            lib->getTransactionsVector(),
                            lib->getFinesVector(),
                            lib->getReservationsVector());
            break;
        }
        case 3:
            cout << "RETURNING..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 3);
}

//=============================================================
//     LIBRARIAN MAIN MENU
void librarian::librarianMainMenu(library *lib)
{
    int choice;
    do
    {
        cout << "\n================ LIBRARIAN MAIN MENU ================" << endl;
        cout << "1. VIEW SYSTEM DETAILS" << endl;
        cout << "2. MANAGE BOOKS" << endl;
        cout << "3. MANAGE TABLES" << endl;
        cout << "4. MANAGE USERS" << endl;
        cout << "5. EXIT" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 5);

        switch (choice)
        {
        case 1:
            viewFullDetails(lib);
            break;
        case 2:
            manageBooks(lib);
            break;
        case 3:
            manageTables(lib);
            break;
        case 4:
            manageUsers(lib);
            break;
        case 5:
            cout << "EXITING LIBRARIAN SYSTEM..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 5);
}
