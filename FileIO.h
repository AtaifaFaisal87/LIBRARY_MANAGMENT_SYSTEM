#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <vector>
#include "Structs.h"
#include "User.h"
#include "Book.h"
#include "BookCatalog.h"
#include "Transaction.h"
#include "Fines.h"
#include "Reservation.h"
#include "Table.h"
#include "Exceptions.h"
using namespace std;

// ============================================================
//                    FILE I/O FUNCTIONS
// ============================================================

// ---- USER FILE I/O ----
void saveUserToFile(User *u);
void loadUsersFromFile(vector<User *> &users);
void rewriteUsersFile(vector<User *> &users);

// ---- BOOK FILE I/O ----
void saveBookToFile(Book *b);
void loadBooksFromFile(BookCatalog *catalog);
void rewriteBooksFile(BookCatalog *catalog);

// ---- TRANSACTION FILE I/O ----
void saveTransactionToFile(Transaction &t);
void loadTransactionsFromFile(vector<Transaction> &transactions, vector<User *> &users);
void rewriteTransactionsFile(vector<Transaction> &transactions);

// ---- FINES FILE I/O ----
void saveFineToFile(Fines &f);
void loadFinesFromFile(vector<Fines> &fines);
void rewriteFinesFile(vector<Fines> &fines);

// ---- RESERVATION FILE I/O ----
void saveReservationToFile(Reservation &r);
void loadReservationsFromFile(vector<Reservation> &reservations);
void rewriteReservationsFile(vector<Reservation> &reservations);

// ---- TABLE FILE I/O ----
void saveTableToFile(Table &t);
void loadTablesFromFile(vector<Table> &tables);
void rewriteTablesFile(vector<Table> &tables);

#endif // FILEIO_H
