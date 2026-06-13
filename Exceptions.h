#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
using namespace std;

// ============================================================
//                  CUSTOM EXCEPTION CLASSES
// ============================================================

// covers: File could not be opened for reading or writing
class FileException : public exception
{
    string message;
public:
    FileException(string msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

// covers: Book not found in catalog, or no copies available
class BookException : public exception
{
    string message;
public:
    BookException(string msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

// covers: user not found, duplicate username, borrow limit hit
class UserException : public exception
{
    string message;
public:
    UserException(string msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

// covers: Wrong username or password
class AuthException : public exception
{
    string message;
public:
    AuthException(string msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

// covers: table not found, full, or time conflict
class ReservationException : public exception
{
    string message;
public:
    ReservationException(string msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

// covers: Overdue books blocking a borrow, or fine calculation issue
class TransactionException : public exception
{
    string message;
public:
    TransactionException(string msg) : message(msg) {}
    const char *what() const noexcept override { return message.c_str(); }
};

#endif // EXCEPTIONS_H
