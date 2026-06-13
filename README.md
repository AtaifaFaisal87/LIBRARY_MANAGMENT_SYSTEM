# Library Management System - Header File Division

## Quick Start

### Using Make (Recommended)
```bash
cd library_headers
make
./library_system
```

### Manual Compilation
```bash
cd library_headers
g++ -std=c++11 -Wall -Wextra -o library_system main.cpp Utils.cpp Library.cpp User.cpp Librarian.cpp Authentication.cpp FileIO.cpp Fines.cpp Statics.cpp
./library_system
```

### Clean Build Files
```bash
make clean
```

---

## File Structure

### Header Files (.h) - Declarations
| File | Description |
|------|-------------|
| **Constants.h** | Global constants (MAX_TABLES, fine limits, membership defaults) |
| **Exceptions.h** | Custom exception classes (FileException, BookException, UserException, AuthException, ReservationException, TransactionException) |
| **Structs.h** | Binary file I/O structs (UserRecord, BookRecord, TransactionRecord, FineRecord, TableRecord, ReservationRecord) |
| **Utils.h** | Utility functions, input validation, RuntimeLogger, time helpers |
| **Membership.h** | Membership class with 15% fine discount and borrow limit boosts |
| **Book.h** | Book class with counters (download, read-onsite, borrow) and availability |
| **BookCatalog.h** | Book catalog container with search by ID, title, author, category, publisher, year |
| **Transaction.h** | Transaction class tracking borrow time, due time, return time, overdue minutes |
| **Fines.h** | Fines class for overdue fine calculation with membership discount |
| **Reservation.h** | Reservation class for table/book reservations with time overlap detection |
| **Table.h** | Table class for library seating management |
| **User.h** | Abstract User base class + Student, Faculty, Guest derived classes |
| **Librarian.h** | Librarian class with admin credentials and menu forward declarations |
| **Authentication.h** | Login/logout authentication system |
| **Library.h** | Main Library class tying all components together |
| **FileIO.h** | All binary file save/load function declarations |

### Implementation Files (.cpp) - Definitions
| File | Description |
|------|-------------|
| **main.cpp** | Entry point, main menu loop, file loading, librarian setup |
| **Utils.cpp** | Input validation functions, time conversion utilities |
| **Library.cpp** | Library class methods (borrow, return, reservations, tables, user management, views) |
| **User.cpp** | User menu implementations (dashboard, search, borrow/return, reservations, membership, profile update) |
| **Librarian.cpp** | Librarian menus (view system, manage books/tables/users) |
| **Authentication.cpp** | User login and librarian login implementations |
| **FileIO.cpp** | All binary file read/write for users, books, transactions, fines, reservations, tables |
| **Fines.cpp** | Fines constructor with membership-aware discount |
| **Statics.cpp** | Static member initializations (next_id for Book, Transaction, Reservation, Table, Membership) |

---

## Default Credentials
- **Librarian**: username `admin`, password `123`

## Data Files (auto-created on first run)
| File | Content |
|------|---------|
| `users.dat` | User records |
| `books.dat` | Book catalog |
| `transactions.dat` | Borrow/return transactions |
| `fines.dat` | Fine records |
| `tables.dat` | Table records |
| `reservations.dat` | Reservation records |

## Design Notes

- **Forward declarations** used to break circular dependencies between User, Library, Librarian, and Authentication
- **Binary file I/O** with struct serialization for persistence
- **Polymorphism** via abstract User base class
- **Exception handling** with custom exception hierarchy
- **Membership system** with automatic fine discounts and borrow limit boosts
- **Soft delete** for users (data preserved, marked as deleted)
