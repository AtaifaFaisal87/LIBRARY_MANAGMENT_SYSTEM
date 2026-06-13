#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>
#include "Utils.h"
using namespace std;

// ============================================================
//                    TRANSACTION CLASS
//==============================================================
class Transaction
{
private:
    static int next_id;
    int transaction_id;
    int user_id;
    int book_id;
    time_t borrow_time;
    time_t due_time;
    time_t return_time;
    string return_date;
    bool returned;

public:
    // CONSTRUCTOR
    Transaction()
        : transaction_id(next_id++), user_id(0), book_id(0),
          borrow_time(0), due_time(0), return_time(0),
          return_date("NOT RETURNED"), returned(false) {}
    // PARAMETERIZED CONSTRUCTOR
    Transaction(int uid, int bid, int duration_minutes)
        : transaction_id(next_id++), user_id(uid), book_id(bid),
          return_date("NOT RETURNED"), returned(false), return_time(0)
    {
        borrow_time = getCurrentTime();
        due_time = addMinutes(borrow_time, duration_minutes);
    }

    // DESTRUCTOR
    ~Transaction() {}

    // GETTERS
    int getuserid() { return user_id; }
    int getbookid() { return book_id; }
    int getTransactionID() { return transaction_id; }
    bool isReturned() { return returned; }
    time_t getDueTime() { return due_time; }
    time_t getBorrowTime() { return borrow_time; }
    time_t getReturnTime() { return return_time; }
    string getReturnDate() { return return_date; }

    // SETTERS
    void setTransactionID(int id) { transaction_id = id; }
    void setUserID(int id) { user_id = id; }
    void setBookID(int id) { book_id = id; }
    void setBorrowTime(time_t t) { borrow_time = t; }
    void setDueTime(time_t t) { due_time = t; }
    void setReturnTime(time_t t) { return_time = t; }
    void setReturnDate(string d) { return_date = d; }
    void setReturned(bool r) { returned = r; }

    // CALCULATE OVERDUE MINUTES
    int getOverdueMinutes()
    {
        if (!returned || return_time == 0 || return_time <= due_time)
            return 0;
        return minutesDiff(due_time, return_time);
    }

    // MARK AS RETURNED
    int markreturned()
    {
        returned = true;
        return_time = getCurrentTime();
        return_date = timeToString(return_time);
        return getOverdueMinutes();
    }

    // DISPLAY via friend ostream
    friend ostream &operator<<(ostream &os, Transaction &t)
    {
        os << "=============== TRANSACTION DETAILS ===============" << endl
           << "| TRANSACTION ID : " << t.transaction_id << endl
           << "| USER ID        : " << t.user_id << endl
           << "| BOOK ID        : " << t.book_id << endl
           << "| BORROWED ON    : " << timeToString(t.borrow_time) << endl
           << "| DUE DATE       : " << timeToString(t.due_time) << endl
           << "| RETURN DATE    : " << t.return_date << endl
           << "| OVERDUE        : " << t.getOverdueMinutes() << endl
           << "| RETURNED       : " << (t.returned ? "YES" : "NO") << endl
           << "===================================================" << endl;
        return os;
    }
};

#endif // TRANSACTION_H
