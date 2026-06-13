#ifndef CONSTANTS_H
#define CONSTANTS_H

// ============================================================
//                    GLOBAL CONSTANTS
// ============================================================

// Maximum allowed unpaid fine / negative balance before borrowing is blocked
const double MAX_UNPAID_FINE_LIMIT = 500.0;

// Maximum tables in the library
const int MAX_TABLES = 10;

// Membership discount percentage
const double MEMBERSHIP_FINE_DISCOUNT = 0.15;  // 15%

// Default membership duration
const int DEFAULT_MEMBERSHIP_DAYS = 365;

// Maximum books a user can track in file
const int MAX_BORROWED_BOOKS = 10;

#endif // CONSTANTS_H
