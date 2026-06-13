#include "library_core.h"
#include <algorithm>

const double Database::FINE_DISCOUNT = 0.15;
const double Database::MAX_UNPAID    = 500.0;

Database& Database::instance() {
    static Database db;
    return db;
}

// ---- helpers ----
std::string Database::timeStr(time_t t) {
    if (!t) return "N/A";
    char buf[64];
    struct tm* tm = localtime(&t);
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", tm);
    return buf;
}
time_t Database::addMinutes(time_t t, int mins) { return t + (time_t)(mins * 60); }
int    Database::minutesDiff(time_t s, time_t e) {
    double d = difftime(e, s);
    return (d > 0) ? (int)(d / 60) : 0;
}

// ---- find helpers ----
UserRecord* Database::findUser(int id) {
    for (auto& u : users) if (u.id == id) return &u;
    return nullptr;
}
UserRecord* Database::findUserByUsername(const std::string& u) {
    for (auto& r : users) if (r.username == u) return &r;
    return nullptr;
}
BookRecord* Database::findBook(int id) {
    for (auto& b : books) if (b.id == id) return &b;
    return nullptr;
}
BookRecord* Database::findBookByTitle(const std::string& t) {
    for (auto& b : books) if (b.title == t) return &b;
    return nullptr;
}
TableRecord* Database::findTable(int id) {
    for (auto& t : tables) if (t.id == id) return &t;
    return nullptr;
}

// ============================================================
//  LOAD ALL
// ============================================================
void Database::loadAll() {
    // ---- users ----
    {
        std::ifstream f("users.dat", std::ios::binary);
        if (f) { UserRecord r; while (f.read((char*)&r, sizeof(r))) users.push_back(r); }
        for (auto& u : users) if (u.id >= nextUserID) nextUserID = u.id + 1;
    }
    // ---- books ----
    {
        std::ifstream f("books.dat", std::ios::binary);
        if (f) { BookRecord r; while (f.read((char*)&r, sizeof(r))) books.push_back(r); }
        for (auto& b : books) if (b.id >= nextBookID) nextBookID = b.id + 1;
    }
    // ---- transactions ----
    {
        std::ifstream f("transactions.dat", std::ios::binary);
        if (f) { TransactionRecord r; while (f.read((char*)&r, sizeof(r))) transactions.push_back(r); }
        for (auto& t : transactions) if (t.tid >= nextTxnID) nextTxnID = t.tid + 1;
    }
    // ---- fines ----
    {
        std::ifstream f("fines.dat", std::ios::binary);
        if (f) { FineRecord r; while (f.read((char*)&r, sizeof(r))) fines.push_back(r); }
    }
    // ---- tables ----
    {
        std::ifstream f("tables.dat", std::ios::binary);
        if (f) { TableRecord r; while (f.read((char*)&r, sizeof(r))) tables.push_back(r); }
        for (auto& t : tables) if (t.id >= nextTableID) nextTableID = t.id + 1;
    }
    // ---- reservations ----
    {
        std::ifstream f("reservations.dat", std::ios::binary);
        if (f) { ReservationRecord r; while (f.read((char*)&r, sizeof(r))) reservations.push_back(r); }
        for (auto& r : reservations) if (r.reservationid >= nextResID) nextResID = r.reservationid + 1;
    }
}

// ============================================================
//  REWRITES
// ============================================================
void Database::rewriteUsers() {
    std::ofstream f("users.dat", std::ios::binary);
    for (auto& r : users) f.write((char*)&r, sizeof(r));
}
void Database::rewriteBooks() {
    std::ofstream f("books.dat", std::ios::binary);
    for (auto& r : books) f.write((char*)&r, sizeof(r));
}
void Database::rewriteTransactions() {
    std::ofstream f("transactions.dat", std::ios::binary);
    for (auto& r : transactions) f.write((char*)&r, sizeof(r));
}
void Database::rewriteFines() {
    std::ofstream f("fines.dat", std::ios::binary);
    for (auto& r : fines) f.write((char*)&r, sizeof(r));
}
void Database::rewriteTables() {
    std::ofstream f("tables.dat", std::ios::binary);
    for (auto& r : tables) f.write((char*)&r, sizeof(r));
}
void Database::rewriteReservations() {
    std::ofstream f("reservations.dat", std::ios::binary);
    for (auto& r : reservations) f.write((char*)&r, sizeof(r));
}

// ============================================================
//  APPENDS
// ============================================================
void Database::appendUser(const UserRecord& r) {
    std::ofstream f("users.dat", std::ios::binary | std::ios::app);
    f.write((char*)&r, sizeof(r));
}
void Database::appendBook(const BookRecord& r) {
    std::ofstream f("books.dat", std::ios::binary | std::ios::app);
    f.write((char*)&r, sizeof(r));
}
void Database::appendTransaction(const TransactionRecord& r) {
    std::ofstream f("transactions.dat", std::ios::binary | std::ios::app);
    f.write((char*)&r, sizeof(r));
}
void Database::appendFine(const FineRecord& r) {
    std::ofstream f("fines.dat", std::ios::binary | std::ios::app);
    f.write((char*)&r, sizeof(r));
}
void Database::appendTable(const TableRecord& r) {
    std::ofstream f("tables.dat", std::ios::binary | std::ios::app);
    f.write((char*)&r, sizeof(r));
}
void Database::appendReservation(const ReservationRecord& r) {
    std::ofstream f("reservations.dat", std::ios::binary | std::ios::app);
    f.write((char*)&r, sizeof(r));
}
