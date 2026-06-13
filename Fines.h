#ifndef FINES_H
#define FINES_H

#include <iostream>
#include "Membership.h"
using namespace std;

// Forward declaration (User is declared in User.h, but we only need pointer here)
class User;

// ============================================================
//                    FINES CLASS
//==============================================================
class Fines
{
private:
    int user_id;
    int book_id;
    int minuteslate;
    double rate;
    double originalAmount;
    double finalAmount;
    bool discountApplied;

public:
    // CONSTRUCTOR
    Fines()
        : user_id(0), book_id(0), minuteslate(0),
          rate(0), originalAmount(0), finalAmount(0), discountApplied(false) {}

    Fines(int u, int b, int mins, User *user);

    // DESTRUCTOR
    ~Fines() {}

    // GETTERS
    double getAmount() { return finalAmount; }
    double getOriginal() { return originalAmount; }
    int getUserID() { return user_id; }
    int getBookID() { return book_id; }
    bool wasDiscounted() { return discountApplied; }
    int getMinutesLate() { return minuteslate; }
    double getRate() { return rate; }

    // SETTERS
    void setUserID(int id) { user_id = id; }
    void setBookID(int id) { book_id = id; }
    void setMinutesLate(int m) { minuteslate = m; }
    void setRate(double r) { rate = r; }
    void setOriginalAmount(double a) { originalAmount = a; }
    void setFinalAmount(double a) { finalAmount = a; }
    void setDiscountApplied(bool d) { discountApplied = d; }

    // DISPLAY via friend ostream
    friend ostream &operator<<(ostream &os, const Fines &f)
    {
        os << "================= FINE DETAILS =================" << endl
           << "| USER ID        : " << f.user_id << endl
           << "| BOOK ID        : " << f.book_id << endl
           << "| MINUTES LATE   : " << f.minuteslate << endl
           << "| RATE           : RS. " << f.rate << " /MIN" << endl
           << "| ORIGINAL AMT   : RS. " << f.originalAmount << endl;

        if (f.discountApplied)
        {
            os << "| DISCOUNT       : 15% (MEMBERSHIP)" << endl
               << "| FINAL AMT      : RS. " << f.finalAmount << endl;
        }
        else
        {
            os << "| FINAL AMT      : RS. " << f.finalAmount << endl;
        }

        os << "=================================================" << endl;
        return os;
    }
};

#endif // FINES_H
