#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include "BookCatalog.h"
#include "Table.h"
#include "Reservation.h"
#include "Transaction.h"
#include "Fines.h"
#include "User.h"
#include "Exceptions.h"
#include "Constants.h"
using namespace std;

// ============================================================
//                    LIBRARY CLASS
//============================================================
class library
{
private:
    BookCatalog catalog;
    vector<Table> tables;
    vector<Reservation> reservations;
    vector<Transaction> transactions;
    vector<Fines> fines;
    vector<User *> users;

public:
    // ---------------- USER ----------------
    User *getUserByIndex(int index)
    {
        if (index >= 0 && index < (int)users.size())
            return users[index];
        return nullptr;
    }

    int getUserCount() { return (int)users.size(); }
    int getActiveUserCount()
    {
        int count = 0;
        for (int i = 0; i < (int)users.size(); i++)
        {
            if (!users[i]->getIsDeleted())
                count++;
        }
        return count;
    }

    vector<User *> &getUsersVector() { return users; }
    vector<Table> &getTablesVector() { return tables; }
    vector<Transaction> &getTransactionsVector() { return transactions; }
    vector<Fines> &getFinesVector() { return fines; }
    vector<Reservation> &getReservationsVector() { return reservations; }

    void registerUserInLibrary();

    // ---------------- BOOK INTERACTIONS ----------------
    bool readOnSite(User *u, string title);
    bool downloadBook(User *u, string title);
    bool hasOverdueBooks(User *u);
    bool borrowBook(User *u, string title, int duration_minutes);
    bool returnBook(User *u, string title);
    int calculateLiveFine(Transaction &t, User *u);

    //--------------------views--------------------
    void viewTransactions();
    void viewFines();
    void viewUserTransactions(int userID);
    void viewUserFines(int userID);
    void viewUserReservations(int uid);
    void viewAllReservations();
    void viewTables();

    // ---------------- DELETE USER ----------------
    bool deleteUser(int uid, vector<Transaction> &txns, vector<Fines> &fins, vector<Reservation> &res);

    void viewUsers();
    void viewUsersByType(string type);
    void viewAllMemberships();

    // ---------------- RESERVATION ----------------
    void reserveBook(int uid, int bkid, string date, string start, string end);
    void reserveTableManual(int uid, int tid, string date, string start, string end);
    void reserveTableAuto(int uid, string date, string start, string end);
    void cancelReservation(int rid, int uid);
    void autoExpireReservations();

    //----------------TABLE INTERATIONS----------------
    void addTable(string loc, int cap);
    void removeTable(int tid);
    void viewAvailableTables(string date, string start, string end);
    int getTableCount();
    bool hasAvailableTable(string date, string start, string end);
    Table getTable(int i);

    // ---------------- CATALOG ACCESS ----------------
    BookCatalog *getCatalog()
    {
        return &catalog;
    }
};

#endif // LIBRARY_H
