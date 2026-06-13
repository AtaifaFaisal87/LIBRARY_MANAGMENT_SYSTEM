#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include <iostream>
#include "Utils.h"
#include "Constants.h"
using namespace std;

// ============================================================
//                    MEMBERSHIP CLASS
// ============================================================
// membership gives users a fine discount and a higher borrow limit
class Membership
{
private:
    int membershipID;
    static int next_id;

    int userID;
    string userType;
    string userName;

    time_t activationTime;
    time_t expiryTime;
    bool active;

public:
    // DEFAULT CONSTRUCTOR
    Membership()
        : membershipID(next_id++), userID(0),
          userType("NONE"), userName("NONE"),
          activationTime(0), expiryTime(0), active(false) {}

    // PARAMETERIZED CONSTRUCTOR
    Membership(int uid, string utype, string uname, int durationDays = DEFAULT_MEMBERSHIP_DAYS)
        : membershipID(next_id++), userID(uid),
          userType(utype), userName(uname), active(true)
    {
        activationTime = getCurrentTime();
        expiryTime = addMinutes(activationTime, durationDays * 24 * 60);
    }

    // RESTORE CONSTRUCTOR (for loading from file — does not reset times)
    Membership(int uid, string utype, string uname, time_t activation, time_t expiry)
        : membershipID(next_id++), userID(uid),
          userType(utype), userName(uname), active(true),
          activationTime(activation), expiryTime(expiry) {}

    // DESTRUCTOR
    ~Membership() {}

    // ---- GETTERS ----
    int getMembershipID() const { return membershipID; }
    int getUserID() const { return userID; }
    string getUserType() const { return userType; }
    string getUserName() const { return userName; }
    bool isActive() const { return active; }
    time_t getActivationTime() { return activationTime; }
    time_t getExpiryTime() { return expiryTime; }

    // ---- SETTERS ----
    void setActivationTime(time_t t) { activationTime = t; }
    void setExpiryTime(time_t t) { expiryTime = t; }

    // Check if membership is still valid
    bool isValid() const
    {
        return active && (getCurrentTime() <= expiryTime);
    }

    // ---- FINE DISCOUNT ----
    double applyDiscount(double originalFine) const
    {
        if (!isValid())
            return originalFine;
        return originalFine * (1.0 - MEMBERSHIP_FINE_DISCOUNT);
    }

    // ---- BORROW LIMIT BOOST ----
    int getBorrowBoost() const
    {
        if (!isValid())
            return 0;
        if (userType == "STUDENT")
            return 2; // 3 -> 5
        if (userType == "FACULTY")
            return 2; // 5 -> 7
        if (userType == "GUEST")
            return 1; // 1 -> 2
        return 0;
    }

    // ---- CANCEL ----
    void cancel()
    {
        active = false;
        cout << "MEMBERSHIP CANCELLED FOR " << userName << "." << endl;
    }

    // ---- DISPLAY ----
    friend ostream &operator<<(ostream &os, const Membership &m)
    {
        os << "=========== MEMBERSHIP DETAILS ===========" << endl
           << "| MEMBERSHIP ID  : " << m.membershipID << endl
           << "| MEMBER NAME    : " << m.userName << endl
           << "| USER TYPE      : " << m.userType << endl
           << "| STATUS         : " << (m.isValid() ? "ACTIVE" : "EXPIRED/INACTIVE") << endl
           << "| FINE DISCOUNT  : 15%" << endl
           << "| BORROW BOOST   : +" << m.getBorrowBoost() << " BOOKS" << endl
           << "| ACTIVATED ON   : " << timeToString(m.activationTime)
           << "| EXPIRES ON     : " << timeToString(m.expiryTime)
           << "==========================================" << endl;
        return os;
    }
};

#endif // MEMBERSHIP_H
