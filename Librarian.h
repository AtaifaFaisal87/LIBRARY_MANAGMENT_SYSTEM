#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include <iostream>
#include <string>
using namespace std;

// Forward declarations
class library;
class BookCatalog;
class Book;

// ============================================================
//                    LIBRARIAN CLASS
//==============================================================
class librarian
{
private:
    int librarian_id;
    string name;
    string email;
    string phone_no;
    string username;
    string password;

public:
    // CONSTRUCTOR
    librarian() : librarian_id(0), name("UNKNOWN"), email("UNKNOWN"), phone_no("UNKNOWN") {}
    librarian(int id, string n, string e, string p) : librarian_id(id), name(n), email(e), phone_no(p) {}

    // DESTRUCTOR
    ~librarian() {}

    // CREDENTIALS
    void setCredentials(string u, string p)
    {
        username = u;
        password = p;
    }

    string getUsername() { return username; }
    string getPassword() { return password; }

    // thin wrappers
    void addbook(Book *b, BookCatalog *catalog);
    void removeBook(string t, BookCatalog *catalog);

    // DISPLAY via friend ostream
    friend ostream &operator<<(ostream &os, const librarian &l)
    {
        os << "=============== LIBRARIAN DETAILS ===============" << endl
           << "| ID       : " << l.librarian_id << endl
           << "| NAME     : " << l.name << endl
           << "| EMAIL    : " << l.email << endl
           << "| PHONE #  : " << l.phone_no << endl
           << "=================================================" << endl;
        return os;
    }

    // Menu functions (defined after library class is complete)
    void viewFullDetails(library *lib);
    void manageBooks(library *lib);
    void manageTables(library *lib);
    void manageUsers(library *lib);
    void librarianMainMenu(library *lib);
};

#endif // LIBRARIAN_H
