#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <cstring>
#include <fstream>
#include <stdexcept>

// ============================================================
//  BINARY FILE STRUCTS  (must match the original .dat layout)
// ============================================================
struct UserRecord {
    int    id;
    char   name[50];
    char   email[50];
    char   phone[20];
    char   cnic[20];
    char   type[20];
    char   username[50];
    char   password[50];
    bool   isDeleted;
    char   seatno[30];
    char   major[50];
    char   department[50];
    char   designation[50];
    char   purpose[100];
    int    borrowed_count;
    int    borrowed_ids[10];
    bool   hasMembership;
    int    membershipDurationDays;
    time_t membershipActivation;
    time_t membershipExpiry;
    int    daily_borrows;
    char   last_borrow_date[15];
    double balance;
    double total_fines_paid;
};

struct BookRecord {
    int  id;
    char title[100];
    char author[100];
    char category[50];
    char isbn[30];
    char publisher[50];
    int  year;
    int  copies;
    int  download_count;
    int  read_onsite_count;
    int  times_borrowed;
};

struct TransactionRecord {
    int    tid;
    int    uid;
    int    bid;
    time_t borrow_time;
    time_t due_time;
    time_t return_time;
    char   return_date[30];
    bool   returned;
};

struct FineRecord {
    int    user_id;
    int    book_id;
    int    minuteslate;
    double rate;
    double originalAmount;
    double finalAmount;
    bool   discountApplied;
};

struct TableRecord {
    int  id;
    char location[100];
    int  capacity;
};

struct ReservationRecord {
    int  reservationid;
    int  userid;
    int  bookid;
    int  tableid;
    char date[15];
    char startTime[10];
    char endTime[10];
    bool active;
};

// ============================================================
//  IN-MEMORY MODELS
// ============================================================
struct UserModel {
    UserRecord rec;
    // helpers
    std::string name()     const { return rec.name; }
    std::string email()    const { return rec.email; }
    std::string type()     const { return rec.type; }
    std::string username() const { return rec.username; }
    bool isDeleted()       const { return rec.isDeleted; }
    int  id()              const { return rec.id; }
};

struct BookModel  { BookRecord  rec; };
struct TxnModel   { TransactionRecord rec; };
struct FineModel  { FineRecord  rec; };
struct TableModel { TableRecord rec; };
struct ResModel   { ReservationRecord rec; };

// ============================================================
//  DATABASE SINGLETON – wraps all binary file I/O
// ============================================================
class Database {
public:
    static Database& instance();

    // load everything on startup
    void loadAll();
    void saveAll();   // full rewrite of every file

    // individual save (append)
    void appendUser(const UserRecord& r);
    void appendBook(const BookRecord& r);
    void appendTransaction(const TransactionRecord& r);
    void appendFine(const FineRecord& r);
    void appendTable(const TableRecord& r);
    void appendReservation(const ReservationRecord& r);

    // full rewrites
    void rewriteUsers();
    void rewriteBooks();
    void rewriteTransactions();
    void rewriteFines();
    void rewriteTables();
    void rewriteReservations();

    // data vectors
    std::vector<UserRecord>        users;
    std::vector<BookRecord>        books;
    std::vector<TransactionRecord> transactions;
    std::vector<FineRecord>        fines;
    std::vector<TableRecord>       tables;
    std::vector<ReservationRecord> reservations;

    // ID counters
    int nextUserID = 1;
    int nextBookID = 1;
    int nextTxnID  = 1;
    int nextTableID= 1;
    int nextResID  = 1;

    // helpers
    UserRecord*  findUser(int id);
    UserRecord*  findUserByUsername(const std::string& u);
    BookRecord*  findBook(int id);
    BookRecord*  findBookByTitle(const std::string& t);
    TableRecord* findTable(int id);

    static std::string timeStr(time_t t);
    static time_t addMinutes(time_t t, int mins);
    static int    minutesDiff(time_t start, time_t end);

    static const double FINE_DISCOUNT;   // 15%
    static const double MAX_UNPAID;      // 500

private:
    Database() {}
    Database(const Database&) = delete;
};
