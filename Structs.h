#ifndef STRUCTS_H
#define STRUCTS_H

#include <ctime>
#include "Constants.h"

// ============================================================
// STRUCTS - used for file I/O to flatten complex objects into plain data
// ============================================================

// All user types use the same struct.
// Unused fields stay empty depending on the user type.
struct UserRecord
{
    int id;
    char name[50];
    char email[50];
    char phone[20];
    char cnic[20];
    char type[20];
    char username[50];
    char password[50];
    bool isDeleted;
    // Student fields
    char seatno[30];
    char major[50];
    // Faculty fields
    char department[50];
    char designation[50];
    // Guest fields
    char purpose[100];
    // Borrowed books
    int borrowed_count;
    int borrowed_ids[MAX_BORROWED_BOOKS];
    // Membership
    bool hasMembership;
    int membershipDurationDays;
    time_t membershipActivation;
    time_t membershipExpiry;
    // Daily borrow limit tracking
    int daily_borrows;
    char last_borrow_date[15]; // DD/MM/YYYY format
    // Balance for fine auto-deduction
    double balance;
    // Total fines paid throughout system lifetime
    double total_fines_paid;
};

struct BookRecord
{
    int id;
    char title[100];
    char author[100];
    char category[50];
    char isbn[30];
    char publisher[50];
    int year;
    int copies;
    int download_count;
    int read_onsite_count;
    int times_borrowed;
};

struct TransactionRecord
{
    int tid;
    int uid;
    int bid;
    time_t borrow_time;
    time_t due_time;
    time_t return_time;
    char return_date[30];
    bool returned;
};

struct FineRecord
{
    int user_id;
    int book_id;
    int minuteslate;
    double rate;
    double originalAmount;
    double finalAmount;
    bool discountApplied;
};

struct TableRecord
{
    int id;
    char location[100];
    int capacity;
};

struct ReservationRecord
{
    int reservationid;
    int userid;
    int bookid;
    int tableid;
    char date[15];
    char startTime[10];
    char endTime[10];
    bool active;
};

#endif // STRUCTS_H
