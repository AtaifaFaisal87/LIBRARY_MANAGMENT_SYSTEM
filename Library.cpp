#include "Library.h"
#include "FileIO.h"
#include "Librarian.h"

// ---------------- USER REGISTRATION ----------------
void library::registerUserInLibrary()
{
    User *newUser = User::registerUser();
    if (newUser)
    {
        for (int i = 0; i < (int)users.size(); i++)
        {
            if (users[i]->getIsDeleted())
                continue;
            if (users[i]->get_cnic() == newUser->get_cnic())
            {
                cout << "REGISTRATION FAILED! A USER WITH THIS CNIC ALREADY EXISTS." << endl;
                delete newUser;
                return;
            }
            if (users[i]->get_email() == newUser->get_email())
            {
                cout << "REGISTRATION FAILED! A USER WITH THIS EMAIL ALREADY EXISTS." << endl;
                delete newUser;
                return;
            }
            if (users[i]->get_phone() == newUser->get_phone())
            {
                cout << "REGISTRATION FAILED! A USER WITH THIS PHONE NUMBER ALREADY EXISTS." << endl;
                delete newUser;
                return;
            }
        }

        bool duplicate = true;
        while (duplicate)
        {
            duplicate = false;
            for (int i = 0; i < (int)users.size(); i++)
            {
                if (users[i]->getIsDeleted()) continue;
                if (users[i]->getUsername() == newUser->getUsername())
                {
                    duplicate = true;
                    cout << "USERNAME ALREADY EXISTS! PLEASE CHOOSE A DIFFERENT ONE." << endl;
                    string newUsername = getValidString("ENTER A NEW USERNAME: ");
                    newUser->setUsername(newUsername);
                    break;
                }
            }
        }

        try
        {
            users.push_back(newUser);
            saveUserToFile(newUser);
            cout << "USER REGISTERED SUCCESSFULLY!\n";
        }
        catch (FileException &e)
        {
            cout << e.what() << endl;
            cout << "USER ADDED TO MEMORY BUT NOT SAVED TO FILE.\n";
        }
    }
    else
        cout << "REGISTRATION FAILED!\n";
}

// ---------------- BOOK INTERACTIONS ----------------
bool library::readOnSite(User *u, string title)
{
    Book *book = catalog.SearchbyTitle(title);
    if (!book)
        throw BookException("BOOK NOT FOUND: " + title);
    book->incrementReadOnSiteCount();
    cout << "READING BOOK ONSITE!" << endl;
    return true;
}

bool library::downloadBook(User *u, string title)
{
    Book *book = catalog.SearchbyTitle(title);
    if (!book)
        throw BookException("BOOK NOT FOUND: " + title);
    book->incrementDownloadCount();
    cout << "BOOK DOWNLOADED!" << endl;
    return true;
}

bool library::hasOverdueBooks(User *u)
{
    time_t now = getCurrentTime();
    for (int i = 0; i < (int)transactions.size(); i++)
    {
        if (transactions[i].getuserid() == u->get_userid() &&
            !transactions[i].isReturned() &&
            now > transactions[i].getDueTime())
            return true;
    }
    return false;
}

bool library::borrowBook(User *u, string title, int duration_minutes)
{
    if (hasOverdueBooks(u))
        throw TransactionException("YOUR BOOK RETURN DATE HAS PASSED! PLEASE RETURN THE OVERDUE BOOKS FIRST.");

    if (!u->canBorrowBasedOnBalance())
        throw UserException("YOUR BALANCE IS TOO LOW OR UNPAID FINES EXCEED THE LIMIT! PLEASE CLEAR YOUR BALANCE FIRST.");

    if (!u->canBorrowToday())
        throw UserException("DAILY BORROW LIMIT REACHED! YOU CANNOT BORROW MORE BOOKS TODAY.");

    Book *book = catalog.SearchbyTitle(title);
    if (!book)
        throw BookException("BOOK NOT FOUND: " + title);

    if (book->getAvailableCopies() == 0)
        throw BookException("NO COPIES AVAILABLE FOR: " + title);

    if (!u->borrowbook(book->get_bookid()))
        throw UserException("BORROW LIMIT REACHED! YOU CANNOT BORROW MORE BOOKS.");

    book->setAvailableCopies(book->getAvailableCopies() - 1);
    book->incrementBorrowCount();
    u->recordDailyBorrow();
    transactions.emplace_back(u->get_userid(), book->get_bookid(), duration_minutes);
    saveTransactionToFile(transactions.back());
    rewriteUsersFile(users);

    time_t now = getCurrentTime();
    cout << "BOOK BORROWED SUCCESSFULLY!" << endl;
    cout << "BORROW TIME : " << timeToString(now) << endl;
    cout << "DUE TIME    : " << timeToString(addMinutes(now, duration_minutes)) << endl;
    cout << "FINE RATE   : Rs." << u->getFineRate() << " PER MINUTE IF LATE!" << endl;
    return true;
}

bool library::returnBook(User *u, string title)
{
    Book *book = catalog.SearchbyTitle(title);
    if (!book)
        throw BookException("BOOK NOT FOUND: " + title);

    if (!u->returnbook(book->get_bookid()))
        throw TransactionException("YOU HAVE NOT BORROWED THIS BOOK: " + title);

    book->setAvailableCopies(book->getAvailableCopies() + 1);

    for (int i = 0; i < (int)transactions.size(); i++)
    {
        if (transactions[i].getbookid() == book->get_bookid() &&
            transactions[i].getuserid() == u->get_userid() &&
            !transactions[i].isReturned())
        {
            int overdue_mins = transactions[i].markreturned();
            rewriteTransactionsFile(transactions);
            cout << "BOOK RETURNED SUCCESSFULLY!" << endl;
            cout << "RETURN TIME: " << timeToString(getCurrentTime()) << endl;

            if (overdue_mins > 0)
            {
                fines.emplace_back(u->get_userid(), book->get_bookid(), overdue_mins, u);
                saveFineToFile(fines.back());
                cout << "FINE GENERATED!" << endl;
                cout << "OVERDUE    : " << overdue_mins << " MINUTES" << endl;

                double originalFine = u->getFineRate() * overdue_mins;
                double finalFine = originalFine;

                if (u->hasMembership())
                {
                    finalFine = u->applyMembershipDiscount(originalFine);
                    cout << "ORIGINAL FINE: Rs." << originalFine << endl;
                    cout << "AFTER 15% MEMBERSHIP DISCOUNT: Rs." << finalFine << endl;
                }
                else
                {
                    cout << "FINE AMOUNT: Rs." << originalFine << endl;
                }

                double balanceBefore = u->getBalance();
                bool deducted = u->deductBalance(finalFine);
                if (deducted)
                {
                    cout << "FINE AUTO-DEDUCTED FROM BALANCE!" << endl;
                    cout << "FINE AMOUNT      : Rs." << finalFine << endl;
                    cout << "REMAINING BALANCE: Rs." << u->getBalance() << endl;
                }
                else
                {
                    cout << "WARNING: INSUFFICIENT BALANCE! FINE COULD NOT BE FULLY DEDUCTED." << endl;
                    cout << "FINE AMOUNT      : Rs." << finalFine << endl;
                    cout << "BALANCE BEFORE   : Rs." << balanceBefore << endl;
                    cout << "CURRENT BALANCE  : Rs." << u->getBalance() << endl;
                    cout << "OUTSTANDING DEBT : Rs." << (-u->getBalance()) << endl;
                    cout << "PLEASE TOP UP YOUR BALANCE TO CLEAR THE DEBT." << endl;
                }
                rewriteUsersFile(users);
            }
            else
                cout << "RETURNED ON TIME! NO FINE." << endl;

            break;
        }
    }
    return true;
}

int library::calculateLiveFine(Transaction &t, User *u)
{
    if (t.isReturned())
        return 0;
    time_t now = getCurrentTime();
    if (now <= t.getDueTime())
        return 0;
    int overdue = minutesDiff(t.getDueTime(), now);
    return overdue * u->getFineRate();
}

//--------------------views--------------------
void library::viewTransactions()
{
    if (transactions.empty())
    {
        cout << "NO TRANSACTIONS IN THE SYSTEM!" << endl;
        return;
    }
    cout << "==== ALL TRANSACTIONS ====" << endl;
    for (int i = 0; i < (int)transactions.size(); i++)
    {
        Transaction &t = transactions[i];
        cout << "=============== TRANSACTION DETAILS ===============" << endl
             << "| TRANSACTION ID : " << t.getTransactionID() << endl;

        User *u = nullptr;
        for (int j = 0; j < (int)users.size(); j++)
            if (users[j]->get_userid() == t.getuserid())
            {
                u = users[j];
                break;
            }

        if (u && u->getIsDeleted())
            cout << "| USER ID        : " << t.getuserid() << " [DELETED: " << u->get_name() << "]" << endl;
        else
            cout << "| USER ID        : " << t.getuserid() << endl;

        cout << "| BOOK ID        : " << t.getbookid() << endl
             << "| BORROWED ON    : " << timeToString(t.getBorrowTime()) << endl
             << "| DUE DATE       : " << timeToString(t.getDueTime()) << endl
             << "| RETURN DATE    : " << t.getReturnDate() << endl
             << "| RETURNED       : " << (t.isReturned() ? "YES" : "NO") << endl
             << "===================================================" << endl;
    }
}

void library::viewFines()
{
    if (fines.empty())
    {
        cout << "NO FINES IN THE SYSTEM!" << endl;
        return;
    }
    cout << "==== ALL FINES ====" << endl;
    for (int i = 0; i < (int)fines.size(); i++)
    {
        Fines &f = fines[i];
        cout << "================= FINE DETAILS =================" << endl;

        User *u = nullptr;
        for (int j = 0; j < (int)users.size(); j++)
            if (users[j]->get_userid() == f.getUserID())
            {
                u = users[j];
                break;
            }

        if (u && u->getIsDeleted())
            cout << "| USER ID        : " << f.getUserID() << " [DELETED: " << u->get_name() << "]" << endl;
        else
            cout << "| USER ID        : " << f.getUserID() << endl;

        cout << "| BOOK ID        : " << f.getBookID() << endl
             << "| MINUTES LATE   : " << f.getMinutesLate() << endl
             << "| RATE           : RS. " << f.getRate() << " /MIN" << endl
             << "| ORIGINAL AMT   : RS. " << f.getOriginal() << endl;
        if (f.wasDiscounted())
            cout << "| DISCOUNT       : 15% (MEMBERSHIP)" << endl
                 << "| FINAL AMT      : RS. " << f.getAmount() << endl;
        else
            cout << "| FINAL AMT      : RS. " << f.getAmount() << endl;

        cout << "=================================================" << endl;
    }
}

void library::viewUserTransactions(int userID)
{
    cout << "==== TRANSACTIONS FOR USER ID: " << userID << " ====" << endl;
    if (transactions.empty())
    {
        cout << "NO TRANSACTIONS IN THE SYSTEM!" << endl;
        return;
    }
    for (int i = 0; i < (int)transactions.size(); i++)
        if (transactions[i].getuserid() == userID)
            cout << transactions[i];
}

void library::viewUserFines(int userID)
{
    cout << "==== FINES FOR USER ID: " << userID << " ====" << endl;
    if (fines.empty() && transactions.empty())
    {
        cout << "NO FINES IN THE SYSTEM!" << endl;
        return;
    }
    bool found = false;
    for (int i = 0; i < (int)fines.size(); i++)
    {
        if (fines[i].getUserID() == userID)
        {
            cout << fines[i];
            found = true;
        }
    }
    for (int i = 0; i < (int)transactions.size(); i++)
    {
        if (transactions[i].getuserid() == userID && !transactions[i].isReturned())
        {
            User *u = nullptr;
            for (int j = 0; j < (int)users.size(); j++)
                if (users[j]->get_userid() == userID)
                {
                    u = users[j];
                    break;
                }
            if (!u)
                continue;
            int fine = calculateLiveFine(transactions[i], u);
            if (fine > 0)
            {
                if (u->hasMembership())
                {
                    double discountedFine = u->applyMembershipDiscount(fine);
                    cout << "|USER ID: " << userID
                         << " |BOOK ID: " << transactions[i].getbookid()
                         << " |STATUS: UNPAID (LIVE)"
                         << " |ORIGINAL FINE: Rs." << fine
                         << " |AFTER 15% DISCOUNT: Rs." << discountedFine
                         << " |" << endl;
                }
                else
                {
                    cout << "|USER ID: " << userID
                         << " |BOOK ID: " << transactions[i].getbookid()
                         << " |STATUS: UNPAID (LIVE)"
                         << " |LIVE FINE: Rs." << fine
                         << " |" << endl;
                }
                found = true;
            }
        }
    }
    if (!found)
        cout << "NO FINES FOUND!" << endl;
}

void library::viewUserReservations(int uid)
{
    autoExpireReservations();
    cout << "==== USER RESERVATIONS ====" << endl;
    bool found = false;
    for (int i = 0; i < (int)reservations.size(); i++)
        if (reservations[i].get_userid() == uid && reservations[i].isActive())
        {
            cout << reservations[i];
            found = true;
        }
    if (!found)
        cout << "NO ACTIVE RESERVATIONS." << endl;
}

void library::viewAllReservations()
{
    autoExpireReservations();
    cout << "\n===== ACTIVE RESERVATIONS =====" << endl;
    bool found = false;
    for (int i = 0; i < (int)reservations.size(); i++)
        if (reservations[i].isActive())
        {
            cout << reservations[i];
            found = true;
        }
    if (!found)
        cout << "NO ACTIVE RESERVATIONS." << endl;

    cout << "\n===== EXPIRED / CANCELLED RESERVATIONS =====" << endl;
    bool foundExpired = false;
    for (int i = 0; i < (int)reservations.size(); i++)
        if (!reservations[i].isActive())
        {
            reservations[i].printInactive(cout);
            foundExpired = true;
        }
    if (!foundExpired)
        cout << "NO EXPIRED OR CANCELLED RESERVATIONS." << endl;
}

void library::viewTables()
{
    cout << "==== TABLES (" << tables.size() << "/" << MAX_TABLES << ") ====" << endl;
    if (tables.empty())
    {
        cout << "NO TABLES AVAILABLE." << endl;
        return;
    }
    for (int i = 0; i < (int)tables.size(); i++)
    {
        cout << tables[i];
    }
}

// ---------------- DELETE USER ----------------
bool library::deleteUser(int uid, vector<Transaction> &txns, vector<Fines> &fins, vector<Reservation> &res)
{
    int idx = -1;
    for (int i = 0; i < (int)users.size(); i++)
    {
        if (users[i]->get_userid() == uid)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        cout << "USER WITH ID " << uid << " NOT FOUND!" << endl;
        return false;
    }
    User *u = users[idx];
    if (u->getIsDeleted())
    {
        cout << "USER IS ALREADY MARKED AS DELETED!" << endl;
        return false;
    }
    if (u->getBorrowedCount() > 0)
    {
        cout << "CANNOT DELETE USER! USER HAS " << u->getBorrowedCount()
             << " UNRETURNED BOOK(S). ENSURE ALL BOOKS ARE RETURNED FIRST." << endl;
        return false;
    }
    int txnCount = 0, fineCount = 0;
    for (int i = 0; i < (int)txns.size(); i++)
        if (txns[i].getuserid() == uid)
            txnCount++;
    for (int i = 0; i < (int)fins.size(); i++)
        if (fins[i].getUserID() == uid)
            fineCount++;

    cout << "\n======== USER TO BE DELETED ========" << endl;
    cout << "| ID           : " << u->get_userid() << endl;
    cout << "| NAME         : " << u->get_name() << endl;
    cout << "| EMAIL        : " << u->get_email() << endl;
    cout << "| PHONE        : " << u->get_phone() << endl;
    cout << "| CNIC         : " << u->get_cnic() << endl;
    cout << "| TYPE         : " << u->get_usertype() << endl;
    cout << "| USERNAME     : " << u->getUsername() << endl;
    cout << "| BALANCE      : Rs." << u->getBalance() << endl;
    cout << "| TRANSACTIONS : " << txnCount << endl;
    cout << "| FINES        : " << fineCount << endl;
    cout << "=====================================" << endl;

    cout << "ARE YOU SURE YOU WANT TO DELETE THIS USER? (1=YES / 0=NO): ";
    int confirm;
    cin >> confirm;
    cin.ignore(1000, '\n');

    if (confirm != 1)
    {
        cout << "DELETION CANCELLED." << endl;
        return false;
    }

    u->setIsDeleted(true);
    for (int i = 0; i < (int)res.size(); i++)
        if (res[i].get_userid() == uid && res[i].isActive())
            res[i].cancel();

    rewriteUsersFile(users);

    cout << "\n======== DELETION SUMMARY ========" << endl;
    cout << "| USER \"" << u->get_name() << "\" (ID: " << uid << ") HAS BEEN MARKED AS DELETED." << endl;
    cout << "| DATA IS PRESERVED IN users.dat" << endl;
    cout << "| TRANSACTIONS LINKED : " << txnCount << endl;
    cout << "| FINES LINKED        : " << fineCount << endl;
    cout << "==================================" << endl;

    return true;
}

void library::viewUsers()
{
    cout << "================= ALL USERS =================" << endl;
    if (users.empty())
    {
        cout << "NO USERS IN THE SYSTEM!" << endl;
        return;
    }
    for (int i = 0; i < (int)users.size(); i++)
    {
        User *u = users[i];
        cout << "| USER ID      : " << u->get_userid() << endl
             << "| NAME         : " << u->get_name() << endl
             << "| EMAIL        : " << u->get_email() << endl
             << "| PHONE        : " << u->get_phone() << endl
             << "| USER TYPE    : " << u->get_usertype() << endl
             << "| STATUS       : " << (u->getIsDeleted() ? "*** DELETED ***" : "ACTIVE") << endl
             << "| MEMBERSHIP   : " << (u->hasMembership() ? "ACTIVE" : "NONE") << endl
             << "| BORROW LIMIT : " << u->getEffectiveBorrowLimit() << endl
             << "| BALANCE      : Rs." << u->getBalance() << endl;
        if (u->getTotalFinesPaid() > 0)
            cout << "| TOTAL FINES  : Rs." << u->getTotalFinesPaid() << " (paid)" << endl;
        cout << "--------------------------------------------" << endl;
    }
}

void library::viewUsersByType(string type)
{
    cout << "=========== USERS OF TYPE: " << type << " ===========" << endl;
    if (users.empty())
    {
        cout << "NO USERS IN THE SYSTEM!" << endl;
        return;
    }
    for (int i = 0; i < (int)users.size(); i++)
    {
        User *u = users[i];
        if (u->get_usertype() == type)
        {
            cout << "| USER ID      : " << u->get_userid() << endl
                 << "| NAME         : " << u->get_name() << endl
                 << "| EMAIL        : " << u->get_email() << endl
                 << "| PHONE        : " << u->get_phone() << endl
                 << "| USER TYPE    : " << u->get_usertype() << endl
                 << "| MEMBERSHIP   : " << (u->hasMembership() ? "ACTIVE" : "NONE") << endl
                 << "| BALANCE      : Rs." << u->getBalance() << endl;
            if (u->getTotalFinesPaid() > 0)
                cout << "| TOTAL FINES  : Rs." << u->getTotalFinesPaid() << " (paid)" << endl
                     << "--------------------------------------------" << endl;
        }
    }
}

void library::viewAllMemberships()
{
    cout << "=============== ALL MEMBERSHIPS ===============" << endl;
    bool found = false;
    for (int i = 0; i < (int)users.size(); i++)
    {
        User *u = users[i];
        if (u->getMembership())
        {
            cout << *u->getMembership();
            found = true;
        }
    }
    if (!found)
        cout << "NO MEMBERSHIPS FOUND." << endl;
}

// ---------------- RESERVATION ----------------
void library::reserveBook(int uid, int bkid, string date, string start, string end)
{
    Book *book = catalog.SearchbyID(bkid);
    if (!book)
        throw BookException("BOOK NOT FOUND WITH ID: " + to_string(bkid));

    if (book->getAvailableCopies() <= 0)
        throw BookException("NO COPIES AVAILABLE TO RESERVE FOR BOOK ID: " + to_string(bkid));

    reservations.emplace_back(uid, bkid, 0, date, start, end);
    saveReservationToFile(reservations.back());
    book->decreaseCopy();
    cout << "BOOK RESERVED SUCCESSFULLY!\n";
}

void library::reserveTableManual(int uid, int tid, string date, string start, string end)
{
    bool tableExists = false;
    for (int i = 0; i < (int)tables.size(); i++)
        if (tables[i].get_id() == tid)
        {
            tableExists = true;
            break;
        }

    if (!tableExists)
        throw ReservationException("TABLE ID " + to_string(tid) + " DOES NOT EXIST!");

    for (int i = 0; i < (int)reservations.size(); i++)
    {
        Reservation &r = reservations[i];
        if (r.get_tableid() == tid && r.get_date() == date && r.isActive())
        {
            if (Reservation::overlap(r.get_start(), r.get_end(), start, end))
                throw ReservationException("TABLE " + to_string(tid) + " IS ALREADY BOOKED FOR THAT TIME SLOT!");
        }
    }

    reservations.emplace_back(uid, 0, tid, date, start, end);
    saveReservationToFile(reservations.back());
    cout << "TABLE RESERVED MANUALLY!\n";
}

void library::reserveTableAuto(int uid, string date, string start, string end)
{
    if (tables.empty())
        throw ReservationException("NO TABLES EXIST IN THE LIBRARY!");

    for (int i = 0; i < (int)tables.size(); i++)
    {
        int tid = tables[i].get_id();
        bool conflict = false;

        for (int j = 0; j < (int)reservations.size(); j++)
        {
            Reservation &r = reservations[j];
            if (r.get_tableid() == tid && r.get_date() == date && r.isActive())
            {
                if (Reservation::overlap(r.get_start(), r.get_end(), start, end))
                {
                    conflict = true;
                    break;
                }
            }
        }

        if (!conflict)
        {
            reservations.emplace_back(uid, 0, tid, date, start, end);
            saveReservationToFile(reservations.back());
            cout << "AUTO ASSIGNED TABLE ID: " << tid << endl;
            return;
        }
    }
    throw ReservationException("NO TABLES AVAILABLE FOR THE SELECTED TIME SLOT!");
}

void library::cancelReservation(int rid, int uid)
{
    for (int i = 0; i < (int)reservations.size(); i++)
    {
        if (reservations[i].get_reservationid() == rid)
        {
            if (reservations[i].get_userid() != uid)
                throw ReservationException("YOU CAN ONLY CANCEL YOUR OWN RESERVATIONS!");
            reservations[i].cancel();
            cout << "RESERVATION CANCELLED!" << endl;
            return;
        }
    }
    throw ReservationException("RESERVATION ID NOT FOUND!");
}

void library::autoExpireReservations()
{
    bool anyExpired = false;
    for (int i = 0; i < (int)reservations.size(); i++)
    {
        if (reservations[i].isActive() && reservations[i].isExpired())
        {
            reservations[i].cancel();
            anyExpired = true;
        }
    }
    if (anyExpired)
    {
        try
        {
            rewriteReservationsFile(reservations);
        }
        catch (FileException &e)
        {
            cout << e.what() << endl;
        }
    }
}

//----------------TABLE INTERATIONS----------------
void library::addTable(string loc, int cap)
{
    if ((int)tables.size() >= MAX_TABLES)
        throw ReservationException("TABLE LIMIT REACHED! MAXIMUM " + to_string(MAX_TABLES) + " TABLES ALLOWED.");

    tables.push_back(Table(loc, cap));
    saveTableToFile(tables.back());
    cout << "TABLE ADDED! (" << tables.size() << "/" << MAX_TABLES << " TABLES)" << endl;
}

void library::removeTable(int tid)
{
    for (int i = 0; i < (int)tables.size(); i++)
    {
        if (tables[i].get_id() == tid)
        {
            tables.erase(tables.begin() + i);
            try
            {
                rewriteTablesFile(tables);
            }
            catch (FileException &e)
            {
                cout << "FILE ERROR: " << e.what() << endl;
                cout << "TABLE REMOVED FROM MEMORY BUT FILE MAY NOT BE UPDATED." << endl;
            }
            cout << "TABLE " << tid << " REMOVED!" << endl;
            return;
        }
    }
    throw ReservationException("TABLE ID NOT FOUND!");
}

void library::viewAvailableTables(string date, string start, string end)
{
    cout << "==== AVAILABLE TABLES (" << tables.size() << "/" << MAX_TABLES << " TOTAL) ====" << endl;
    if (tables.empty())
    {
        cout << "NO TABLES IN LIBRARY. CONTACT LIBRARIAN." << endl;
        return;
    }
    bool anyAvailable = false;
    for (int i = 0; i < (int)tables.size(); i++)
    {
        int tid = tables[i].get_id();
        bool conflict = false;
        for (int j = 0; j < (int)reservations.size(); j++)
        {
            Reservation &r = reservations[j];
            if (r.get_tableid() == tid && r.get_date() == date && r.isActive())
            {
                if (Reservation::overlap(r.get_start(), r.get_end(), start, end))
                {
                    conflict = true;
                    break;
                }
            }
        }
        if (!conflict)
        {
            cout << tables[i];
            anyAvailable = true;
        }
    }
    if (!anyAvailable)
        cout << "NO TABLES AVAILABLE FOR THE SELECTED TIME SLOT." << endl;
}

int library::getTableCount()
{
    return (int)tables.size();
}

bool library::hasAvailableTable(string date, string start, string end)
{
    for (int i = 0; i < (int)tables.size(); i++)
    {
        int tid = tables[i].get_id();
        bool conflict = false;
        for (int j = 0; j < (int)reservations.size(); j++)
        {
            Reservation &r = reservations[j];
            if (r.get_tableid() == tid && r.get_date() == date && r.isActive())
            {
                if (Reservation::overlap(r.get_start(), r.get_end(), start, end))
                {
                    conflict = true;
                    break;
                }
            }
        }
        if (!conflict)
            return true;
    }
    return false;
}

Table library::getTable(int i)
{
    if (i < 0 || i >= (int)tables.size())
        throw ReservationException("TABLE INDEX OUT OF BOUNDS: " + to_string(i));
    return tables[i];
}
