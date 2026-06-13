#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <string>
#include "Membership.h"
#include "Utils.h"
#include "Constants.h"
using namespace std;

// Forward declarations
class library;
class BookCatalog;

// ============================================================
//                    USER CLASS (ABSTRACT)
//==============================================================
// base class for Student, Faculty, Guest
class User
{
protected:
    int userid;
    static int next_id;
    string name;
    string CNIC;
    string email;
    string username;
    string password;
    string phonenumber;
    bool isDeleted;
    vector<int> borrowedbook;

    Membership *membership;

    // Daily borrow limit tracking
    int daily_borrows;
    string last_borrow_date; // stored as "DD/MM/YYYY"

    // Balance for fine auto-deduction
    double balance;
    // Total fines paid throughout system lifetime
    double total_fines_paid;

public:
    // DEFAULT CONSTRUCTOR
    User(string n = "NONE", string c = "xxxxx-xxxxxxx-x",
         string e = "abc@gmail.com", string p = "03xxxxxxxxx")
    {
        userid = next_id++;
        name = n;
        CNIC = c;
        email = e;
        phonenumber = p;
        membership = nullptr;
        daily_borrows = 0;
        last_borrow_date = "";
        balance = 0.0;
        total_fines_paid = 0.0;
        isDeleted = false;
    }

    // polymorphic destructor
    virtual ~User()
    {
        delete membership;
    }

    // ================= SETTERS =================
    void setid(int id) { userid = id; }
    void setname(string n) { name = n; }
    void setemail(string e) { email = e; }
    void setphone_no(string p) { phonenumber = p; }
    void setCNIC(string c) { CNIC = c; }
    void setUsername(string u) { username = u; }
    void setPassword(string p) { password = p; }
    static void setNextID(int id) { next_id = id; }
    void setDailyBorrows(int d) { daily_borrows = d; }
    void setLastBorrowDate(string d) { last_borrow_date = d; }
    void setBalance(double b) { balance = b; }

    // ================= GETTERS =================
    int get_userid() { return userid; }
    string get_name() { return name; }
    string get_email() { return email; }
    string get_phone() { return phonenumber; }
    string get_cnic() { return CNIC; }
    string getUsername() { return username; }
    string getPassword() { return password; }
    int getDailyBorrows() { return daily_borrows; }
    string getLastBorrowDate() { return last_borrow_date; }
    double getBalance() { return balance; }
    double getTotalFinesPaid() { return total_fines_paid; }
    void setTotalFinesPaid(double f) { total_fines_paid = f; }
    bool getIsDeleted() { return isDeleted; }
    void setIsDeleted(bool d) { isDeleted = d; }

    // USED BY LIBRARY CLASS
    const vector<int> &getBorrowedBooks() { return borrowedbook; }
    int getBorrowedCount() { return (int)borrowedbook.size(); }

    // ================= POLYMORPHISM =================
    virtual int getborrowlimit() = 0;
    virtual string get_usertype() = 0;
    virtual void printTo(ostream &os) const = 0;
    virtual int getFineRate() = 0;

    friend ostream &operator<<(ostream &os, const User &u)
    {
        u.printTo(os);
        return os;
    }

    // ================= BORROW =================
    bool borrowbook(int id)
    {
        if ((int)borrowedbook.size() < getEffectiveBorrowLimit())
        {
            borrowedbook.push_back(id);
            return true;
        }
        if (!RuntimeLogger::isSilent())
            cout << "BORROW LIMIT REACHED!" << endl;
        return false;
    }

    // ================= RETURN =================
    bool returnbook(int id)
    {
        for (int i = 0; i < (int)borrowedbook.size(); i++)
        {
            if (borrowedbook[i] == id)
            {
                borrowedbook.erase(borrowedbook.begin() + i);
                return true;
            }
        }
        cout << "BOOK NOT BORROWED BY THIS USER!" << endl;
        return false;
    }

    //===========MEMBERSHIP INTERATIONS================
    Membership *getMembership() { return membership; }
    bool hasMembership() { return (membership != nullptr && membership->isValid()); }

    void activateMembership(int durationDays = DEFAULT_MEMBERSHIP_DAYS)
    {
        if (hasMembership())
        {
            cout << "USER ALREADY HAS AN ACTIVE MEMBERSHIP!" << endl;
            return;
        }
        delete membership;
        membership = new Membership(userid, get_usertype(), name, durationDays);
        cout << "\nMEMBERSHIP ACTIVATED SUCCESSFULLY!" << endl;
        cout << *membership;
    }

    void cancelMembership()
    {
        if (!membership)
        {
            cout << "NO MEMBERSHIP TO CANCEL!" << endl;
            return;
        }
        membership->cancel();
    }

    void viewMembership()
    {
        if (!membership)
        {
            cout << "YOU DO NOT HAVE A MEMBERSHIP.PURCHASE ONE FROM THE MENU!" << endl;
            return;
        }
        cout << *membership;
    }

    void restoreMembership(time_t activation, time_t expiry)
    {
        delete membership;
        membership = new Membership(userid, get_usertype(), name, activation, expiry);
    }

    // ---- BORROW LIMIT (membership-aware) ----
    int getEffectiveBorrowLimit()
    {
        return getborrowlimit() + (hasMembership() ? membership->getBorrowBoost() : 0);
    }

    // Returns today's date as "DD/MM/YYYY"
    static string getTodayDateString()
    {
        time_t now = getCurrentTime();
        struct tm *t = localtime(&now);
        char buf[20];
        sprintf(buf, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
        return string(buf);
    }

    bool canBorrowToday()
    {
        string today = getTodayDateString();
        if (last_borrow_date != today)
        {
            daily_borrows = 0;
            last_borrow_date = today;
        }
        return daily_borrows < getEffectiveBorrowLimit();
    }

    void recordDailyBorrow()
    {
        string today = getTodayDateString();
        if (last_borrow_date != today)
        {
            daily_borrows = 0;
            last_borrow_date = today;
        }
        daily_borrows++;
    }

    // ---- FINE DISCOUNT (membership-aware) ----
    double applyMembershipDiscount(double fine)
    {
        if (hasMembership())
            return membership->applyDiscount(fine);
        return fine;
    }

    // ---- BALANCE MANAGEMENT ----
    bool deductBalance(double amount)
    {
        total_fines_paid += amount;
        if (balance >= amount)
        {
            balance -= amount;
            return true;
        }
        balance -= amount;
        return false;
    }

    void addBalance(double amount)
    {
        balance += amount;
    }

    bool canBorrowBasedOnBalance()
    {
        return balance > -MAX_UNPAID_FINE_LIMIT;
    }

    // These will be defined in User.cpp or after library class is fully declared
    double calculateTotalUnpaidFines(library *lib);
    void updateUser(library *lib);
    void viewFullDetails(library *lib);
    void searchBooks(library *lib);
    void reservationMenu(library *lib);
    void bookActionMenu(library *lib);
    void membershipMenu(library *lib);
    void userMenu(library *lib);
    static User *registerUser();
};

// ================= STUDENT =================
class Student : public User
{
private:
    string seatno;
    string major;

public:
    Student(string n, string c, string e, string p, string s, string m)
        : User(n, c, e, p), seatno(s), major(m) {}

    int getborrowlimit() override { return 3; }
    string get_usertype() override { return "STUDENT"; }
    string getSeatNo() { return seatno; }
    string getMajor() { return major; }
    void setSeatNo(string s) { seatno = s; }
    void setMajor(string m) { major = m; }
    void printTo(ostream &os) const override
    {
        os << "============= STUDENT DETAILS =============" << endl
           << "| ID             : " << userid << endl
           << "| NAME           : " << name << endl
           << "| SEAT NO        : " << seatno << endl
           << "| MAJOR          : " << major << endl
           << "| USER TYPE      : STUDENT" << endl
           << "===========================================" << endl;
    }
    int getFineRate() override { return 10; }
};

// ================= FACULTY =================
class Faculty : public User
{
private:
    string department;
    string designation;

public:
    Faculty(string n, string c, string e, string p, string dept, string des)
        : User(n, c, e, p), department(dept), designation(des) {}

    int getborrowlimit() override { return 5; }
    string get_usertype() override { return "FACULTY"; }
    string getDepartment() { return department; }
    string getDesignation() { return designation; }
    void setDepartment(string d) { department = d; }
    void setDesignation(string d) { designation = d; }
    void printTo(ostream &os) const override
    {
        os << "============= FACULTY DETAILS =============" << endl
           << "| ID             : " << userid << endl
           << "| NAME           : " << name << endl
           << "| DEPARTMENT     : " << department << endl
           << "| DESIGNATION    : " << designation << endl
           << "| USER TYPE      : FACULTY" << endl
           << "===========================================" << endl;
    }
    int getFineRate() override { return 15; }
};

// ================= GUEST =================
class Guest : public User
{
private:
    string purpose;

public:
    Guest(string n, string c, string e, string p, string ps)
        : User(n, c, e, p), purpose(ps) {}

    int getborrowlimit() override { return 1; }
    string get_usertype() override { return "GUEST"; }
    string getPurpose() { return purpose; }
    void setPurpose(string p) { purpose = p; }

    void printTo(ostream &os) const override
    {
        os << "============== GUEST DETAILS ==============" << endl
           << "| ID             : " << userid << endl
           << "| NAME           : " << name << endl
           << "| PURPOSE        : " << purpose << endl
           << "| USER TYPE      : GUEST" << endl
           << "===========================================" << endl;
    }
    int getFineRate() override { return 20; }
};

#endif // USER_H
