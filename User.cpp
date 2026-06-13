#include "User.h"
#include "Library.h"
#include "FileIO.h"

// Static member initialization
int User::next_id = 1;

// ================= REGISTER FUNCTION =================
User *User::registerUser()
{
    int choice = getValidIntInRange("SELECT USER TYPE:\n1. STUDENT\n2. FACULTY\n3. GUEST\nCHOICE: ", 1, 3);

    string name = getValidAlphaString("ENTER NAME: ");
    string CNIC = getValidCNIC("ENTER CNIC: ");
    string email = getValidEmail("ENTER EMAIL: ");
    string phone = getValidPhone("ENTER PHONE: ");
    string username = getValidString("ENTER USERNAME: ");
    string password = getValidPassword("ENTER PASSWORD: ");
    double balance = getValidBalance("ENTER INITIAL BALANCE (Rs.): ");

    if (choice == 1)
    {
        string seat = getValidString("ENTER SEAT NUMBER: ");
        string major = getValidAlphaString("ENTER MAJOR: ");
        User *u = new Student(name, CNIC, email, phone, seat, major);
        u->setUsername(username);
        u->setPassword(password);
        u->setBalance(balance);
        return u;
    }
    else if (choice == 2)
    {
        string dept = getValidAlphaString("ENTER DEPARTMENT: ");
        string des = getValidAlphaString("ENTER DESIGNATION: ");
        User *u = new Faculty(name, CNIC, email, phone, dept, des);
        u->setUsername(username);
        u->setPassword(password);
        u->setBalance(balance);
        return u;
    }
    else
    {
        string purpose = getValidString("ENTER PURPOSE: ");
        User *u = new Guest(name, CNIC, email, phone, purpose);
        u->setUsername(username);
        u->setPassword(password);
        u->setBalance(balance);
        return u;
    }
}

// Stub for calculateTotalUnpaidFines
double User::calculateTotalUnpaidFines(library *lib)
{
    double total = 0.0;
    vector<Transaction> &txns = lib->getTransactionsVector();
    for (int i = 0; i < (int)txns.size(); i++)
    {
        if (txns[i].getuserid() == userid && !txns[i].isReturned())
        {
            int fine = lib->calculateLiveFine(txns[i], this);
            if (fine > 0)
                total += fine;
        }
    }
    return total;
}

//=============================================
//     UPDATE USER INFO MENU
void User::updateUser(library *lib)
{
    int choice = 0;
    do
    {
        cout << "================ UPDATE PROFILE ================" << endl;
        cout << "1. UPDATE USERNAME" << endl;
        cout << "2. UPDATE PASSWORD" << endl;
        cout << "3. UPDATE PHONE NUMBER" << endl;

        if (get_usertype() == "STUDENT")
        {
            cout << "4. UPDATE SEAT NUMBER" << endl;
            cout << "5. UPDATE MAJOR" << endl;
        }
        else if (get_usertype() == "FACULTY")
        {
            cout << "4. UPDATE DESIGNATION" << endl;
            cout << "5. UPDATE DEPARTMENT" << endl;
        }
        else if (get_usertype() == "GUEST")
        {
            cout << "4. UPDATE PURPOSE" << endl;
        }

        cout << "6. UPDATE BALANCE (ADD FUNDS)" << endl;
        cout << "7. EXIT" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 7);

        switch (choice)
        {
        case 1:
        {
            string newUsername = getValidString("ENTER NEW USERNAME: ");
            bool duplicate = false;
            for (int i = 0; i < lib->getUserCount(); i++)
            {
                User *other = lib->getUserByIndex(i);
                if (other != this && other->getUsername() == newUsername)
                {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate)
                cout << "USERNAME ALREADY EXISTS! PLEASE CHOOSE A DIFFERENT ONE." << endl;
            else
            {
                setUsername(newUsername);
                cout << "USERNAME UPDATED SUCCESSFULLY!" << endl;
                rewriteUsersFile(lib->getUsersVector());
            }
            break;
        }
        case 2:
        {
            string newPassword = getValidPassword("ENTER NEW PASSWORD: ");
            setPassword(newPassword);
            cout << "PASSWORD UPDATED SUCCESSFULLY!" << endl;
            rewriteUsersFile(lib->getUsersVector());
            break;
        }
        case 3:
        {
            string newPhone = getValidPhone("ENTER NEW PHONE NUMBER: ");
            bool duplicate = false;
            for (int i = 0; i < lib->getUserCount(); i++)
            {
                User *other = lib->getUserByIndex(i);
                if (other != this && other->get_phone() == newPhone)
                {
                    duplicate = true;
                    break;
                }
            }
            if (duplicate)
                cout << "PHONE NUMBER ALREADY EXISTS! PLEASE USE A DIFFERENT ONE." << endl;
            else
            {
                setphone_no(newPhone);
                cout << "PHONE NUMBER UPDATED SUCCESSFULLY!" << endl;
                rewriteUsersFile(lib->getUsersVector());
            }
            break;
        }
        case 4:
        {
            if (get_usertype() == "STUDENT")
            {
                Student *s = dynamic_cast<Student *>(this);
                string newSeat = getValidString("ENTER NEW SEAT NUMBER: ");
                s->setSeatNo(newSeat);
                cout << "SEAT NUMBER UPDATED SUCCESSFULLY!" << endl;
                rewriteUsersFile(lib->getUsersVector());
            }
            else if (get_usertype() == "FACULTY")
            {
                Faculty *f = dynamic_cast<Faculty *>(this);
                string newDesig = getValidAlphaString("ENTER NEW DESIGNATION: ");
                f->setDesignation(newDesig);
                cout << "DESIGNATION UPDATED SUCCESSFULLY!" << endl;
                rewriteUsersFile(lib->getUsersVector());
            }
            else if (get_usertype() == "GUEST")
            {
                Guest *g = dynamic_cast<Guest *>(this);
                string newPurpose = getValidString("ENTER NEW PURPOSE: ");
                g->setPurpose(newPurpose);
                cout << "PURPOSE UPDATED SUCCESSFULLY!" << endl;
                rewriteUsersFile(lib->getUsersVector());
            }
            break;
        }
        case 5:
        {
            if (get_usertype() == "STUDENT")
            {
                Student *s = dynamic_cast<Student *>(this);
                string newMajor = getValidAlphaString("ENTER NEW MAJOR: ");
                s->setMajor(newMajor);
                cout << "MAJOR UPDATED SUCCESSFULLY!" << endl;
                rewriteUsersFile(lib->getUsersVector());
            }
            else if (get_usertype() == "FACULTY")
            {
                Faculty *f = dynamic_cast<Faculty *>(this);
                string newDept = getValidAlphaString("ENTER NEW DEPARTMENT: ");
                f->setDepartment(newDept);
                cout << "DEPARTMENT UPDATED SUCCESSFULLY!" << endl;
                rewriteUsersFile(lib->getUsersVector());
            }
            else if (get_usertype() == "GUEST")
            {
                cout << "INVALID CHOICE FOR GUEST USER!" << endl;
            }
            break;
        }
        case 6:
        {
            double amount = 0;
            while (true)
            {
                cout << "ENTER AMOUNT TO ADD (MINIMUM Rs. 10): ";
                cin >> amount;
                if (cin.fail() || amount < 10)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "INVALID INPUT! PLEASE ENTER A VALID AMOUNT (MINIMUM Rs. 10)." << endl;
                }
                else
                {
                    cin.ignore(1000, '\n');
                    break;
                }
            }
            addBalance(amount);
            cout << "Rs." << amount << " ADDED TO BALANCE SUCCESSFULLY!" << endl;
            cout << "NEW BALANCE: Rs." << balance << endl;
            rewriteUsersFile(lib->getUsersVector());
            break;
        }
        case 7:
            cout << "EXITING UPDATE MENU..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 7);
}

//=============================================
//     SEARCH BOOKS MENU
void User::searchBooks(library *lib)
{
    int choice = 0;
    do
    {
        cout << "\n============= BOOK SEARCH MENU =============" << endl;
        cout << "1. SEARCH BY ID" << endl;
        cout << "2. SEARCH BY TITLE" << endl;
        cout << "3. SEARCH BY AUTHOR" << endl;
        cout << "4. SEARCH BY CATEGORY" << endl;
        cout << "5. SEARCH BY PUBLISHER" << endl;
        cout << "6. SEARCH BY YEAR" << endl;
        cout << "7. EXIT" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 7);
        Book *b = nullptr;

        switch (choice)
        {
        case 1:
        {
            int id = getValidInt("ENTER BOOK ID: ");
            b = lib->getCatalog()->SearchbyID(id);
            break;
        }
        case 2:
        {
            string t = getValidString("ENTER TITLE: ");
            b = lib->getCatalog()->SearchbyTitle(t);
            break;
        }
        case 3:
        {
            string a = getValidString("ENTER AUTHOR: ");
            vector<Book *> results = lib->getCatalog()->SearchbyAuthor(a);
            if (results.empty())
                cout << "NO BOOKS FOUND!" << endl;
            else
                for (int i = 0; i < (int)results.size(); i++)
                    cout << *results[i];
            break;
        }
        case 4:
        {
            string c = getValidString("ENTER CATEGORY: ");
            vector<Book *> results = lib->getCatalog()->SearchbyCategory(c);
            if (results.empty())
                cout << "NO BOOKS FOUND!" << endl;
            else
                for (int i = 0; i < (int)results.size(); i++)
                    cout << *results[i];
            break;
        }
        case 5:
        {
            string p = getValidString("ENTER PUBLISHER: ");
            vector<Book *> results = lib->getCatalog()->SearchbyPublisher(p);
            if (results.empty())
                cout << "NO BOOKS FOUND!" << endl;
            else
                for (int i = 0; i < (int)results.size(); i++)
                    cout << *results[i];
            break;
        }
        case 6:
        {
            int y = getValidIntInRange("ENTER YEAR: ", 1000, 2026);
            vector<Book *> results = lib->getCatalog()->SearchbyYOP(y);
            if (results.empty())
                cout << "NO BOOKS FOUND!" << endl;
            else
                for (int i = 0; i < (int)results.size(); i++)
                    cout << *results[i];
            break;
        }
        case 7:
            cout << "EXITING SEARCH MENU..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }

        if (b != nullptr)
        {
            cout << "\n--------------- BOOK FOUND ---------------" << endl;
            cout << *b;
        }
        else if (choice == 1 || choice == 2)
            cout << "BOOK NOT FOUND!" << endl;

    } while (choice != 7);
}

//=============================================
//     RESERVATION MENU
void User::reservationMenu(library *lib)
{
    int choice = 0;
    do
    {
        cout << "\n============= RESERVATION MENU =============" << endl;
        cout << "1. AUTO RESERVE TABLE" << endl;
        cout << "2. MANUAL RESERVE TABLE" << endl;
        cout << "3. RESERVE BOOK" << endl;
        cout << "4. VIEW MY RESERVATIONS" << endl;
        cout << "5. CANCEL A RESERVATION" << endl;
        cout << "6. EXIT" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 6);

        switch (choice)
        {
        case 1:
        {
            while (true)
            {
                string date = getValidDate("ENTER DATE (DD/MM/YYYY): ");
                string start = getValidTime("ENTER START TIME (HH:MM): ");
                string end = getValidTime("ENTER END TIME (HH:MM): ");
                if (!isValidReservationTime(date, start, end))
                    continue;
                try
                {
                    lib->reserveTableAuto(userid, date, start, end);
                }
                catch (ReservationException &e)
                {
                    cout << e.what() << endl;
                }
                break;
            }
            break;
        }
        case 2:
        {
            while (true)
            {
                string date = getValidDate("ENTER DATE (DD/MM/YYYY): ");
                string start = getValidTime("ENTER START TIME (HH:MM): ");
                string end = getValidTime("ENTER END TIME (HH:MM): ");
                if (!isValidReservationTime(date, start, end))
                    continue;
                cout << "\n--------------- AVAILABLE TABLES ---------------" << endl;
                lib->viewAvailableTables(date, start, end);
                cout << "------------------------------------------------" << endl;
                if (!lib->hasAvailableTable(date, start, end))
                    break;
                int tid = getValidInt("ENTER TABLE ID: ");
                try
                {
                    lib->reserveTableManual(userid, tid, date, start, end);
                }
                catch (ReservationException &e)
                {
                    cout << e.what() << endl;
                }
                break;
            }
            break;
        }
        case 3:
        {
            while (true)
            {
                int bid = getValidInt("ENTER BOOK ID: ");
                string date = getValidDate("ENTER DATE (DD/MM/YYYY): ");
                string start = getValidTime("ENTER START TIME (HH:MM): ");
                string end = getValidTime("ENTER END TIME (HH:MM): ");
                if (!isValidReservationTime(date, start, end))
                    continue;
                try
                {
                    lib->reserveBook(userid, bid, date, start, end);
                }
                catch (BookException &e)
                {
                    cout << e.what() << endl;
                }
                catch (ReservationException &e)
                {
                    cout << e.what() << endl;
                }
                break;
            }
            break;
        }
        case 4:
            cout << "\n----------- YOUR RESERVATIONS -----------" << endl;
            lib->viewUserReservations(userid);
            break;
        case 5:
        {
            cout << "\n----------- YOUR RESERVATIONS -----------" << endl;
            lib->viewUserReservations(userid);
            int rid = getValidInt("ENTER RESERVATION ID TO CANCEL: ");
            try
            {
                lib->cancelReservation(rid, userid);
                rewriteReservationsFile(lib->getReservationsVector());
            }
            catch (ReservationException &e)
            {
                cout << e.what() << endl;
            }
            break;
        }
        case 6:
            cout << "EXITING RESERVATION MENU..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 6);
}

//=============================================================
//     BOOK ACTIONS MENU
void User::bookActionMenu(library *lib)
{
    int choice = 0;
    string title;

    do
    {
        cout << "\n================ BOOK ACTIONS ================" << endl;
        cout << "1. BORROW BOOK" << endl;
        cout << "2. RETURN BOOK" << endl;
        cout << "3. READ ON SITE" << endl;
        cout << "4. DOWNLOAD BOOK" << endl;
        cout << "5. EXIT" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 5);

        switch (choice)
        {
        case 1:
        {
            lib->getCatalog()->ViewBookCatalog();
            int duration = 0;
            title = getValidString("ENTER BOOK TITLE: ");
            int unit = getValidIntInRange("\nSELECT TIME UNIT:\n1. MINUTES\n2. HOURS\n3. DAYS\nCHOICE: ", 1, 3);

            if (unit == 1)
            {
                int mins = getValidIntInRange("ENTER MINUTES: ", 1, 10080);
                duration = mins;
            }
            else if (unit == 2)
            {
                int hours = getValidIntInRange("ENTER HOURS: ", 1, 168);
                duration = hours * 60;
            }
            else if (unit == 3)
            {
                int days = getValidIntInRange("ENTER DAYS: ", 1, 7);
                duration = days * 24 * 60;
            }

            try
            {
                lib->borrowBook(this, title, duration);
                rewriteBooksFile(lib->getCatalog());
            }
            catch (TransactionException &e) { cout << e.what() << endl; }
            catch (BookException &e) { cout << e.what() << endl; }
            catch (UserException &e) { cout << e.what() << endl; }
            catch (FileException &e) { cout << e.what() << endl; }
            break;
        }
        case 2:
        {
            cout << "\n===== YOUR CURRENTLY BORROWED BOOKS =====" << endl;
            vector<int> activeBorrows;
            bool found = false;
            for (int i = 0; i < (int)lib->getTransactionsVector().size(); i++)
            {
                Transaction &t = lib->getTransactionsVector()[i];
                if (t.getuserid() == userid && !t.isReturned())
                {
                    Book *b = lib->getCatalog()->SearchbyID(t.getbookid());
                    if (b)
                    {
                        cout << (int)activeBorrows.size() + 1
                             << ". " << b->get_title()
                             << " (Book ID: " << b->get_bookid() << ")"
                             << " | Due: " << timeToString(t.getDueTime());
                        activeBorrows.push_back(t.getbookid());
                        found = true;
                    }
                }
            }
            if (!found)
            {
                cout << "YOU HAVE NO BOOKS TO RETURN!" << endl;
                break;
            }
            cout << "=========================================" << endl;
            int pick = getValidIntInRange("SELECT BOOK TO RETURN (number): ", 1, (int)activeBorrows.size());
            int selectedBookID = activeBorrows[pick - 1];
            Book *b = lib->getCatalog()->SearchbyID(selectedBookID);
            if (b)
            {
                try
                {
                    lib->returnBook(this, b->get_title());
                    rewriteBooksFile(lib->getCatalog());
                    rewriteTransactionsFile(lib->getTransactionsVector());
                    rewriteUsersFile(lib->getUsersVector());
                }
                catch (BookException &e) { cout << "BOOK ERROR: " << e.what() << endl; }
                catch (TransactionException &e) { cout << "TRANSACTION ERROR: " << e.what() << endl; }
                catch (FileException &e)
                {
                    cout << "FILE ERROR: " << e.what() << endl;
                    cout << "RETURN WAS PROCESSED BUT SOME FILES MAY NOT BE UPDATED." << endl;
                }
            }
            break;
        }
        case 3:
        {
            title = getValidString("ENTER BOOK TITLE: ");
            try { lib->readOnSite(this, title); }
            catch (BookException &e) { cout << e.what() << endl; }
            break;
        }
        case 4:
        {
            title = getValidString("ENTER BOOK TITLE: ");
            try { lib->downloadBook(this, title); }
            catch (BookException &e) { cout << e.what() << endl; }
            break;
        }
        case 5:
            cout << "EXITING BOOK ACTIONS..." << endl;
            return;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 5);
}

//=====================================================
//     MEMBERSHIP MENU
void User::membershipMenu(library *lib)
{
    int choice = 0;
    do
    {
        cout << "\n================ MEMBERSHIP MENU ================" << endl;
        cout << "1. VIEW MEMBERSHIP STATUS" << endl;
        cout << "2. PURCHASE / ACTIVATE MEMBERSHIP" << endl;
        cout << "3. CANCEL MEMBERSHIP" << endl;
        cout << "4. BACK" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 4);

        switch (choice)
        {
        case 1:
            viewMembership();
            break;
        case 2:
            if (hasMembership())
            {
                cout << "YOU ALREADY HAVE AN ACTIVE MEMBERSHIP!" << endl;
                viewMembership();
            }
            else
            {
                cout << "\n=========== MEMBERSHIP BENEFITS FOR YOU ("
                     << get_usertype() << ") ===========" << endl;
                cout << "* 15% DISCOUNT ON ALL FINES" << endl;
                if (get_usertype() == "STUDENT")
                    cout << "* BORROW LIMIT: 3 -> 5 BOOKS" << endl;
                else if (get_usertype() == "FACULTY")
                    cout << "* BORROW LIMIT: 5 -> 7 BOOKS" << endl;
                else if (get_usertype() == "GUEST")
                    cout << "* BORROW LIMIT: 1 -> 2 BOOKS" << endl;
                cout << "* DURATION: 1 YEAR" << endl;

                int confirm = getValidIntInRange("\nCONFIRM MEMBERSHIP PURCHASE? (1=YES / 0=NO): ", 0, 1);
                if (confirm == 1)
                {
                    activateMembership(DEFAULT_MEMBERSHIP_DAYS);
                    rewriteUsersFile(lib->getUsersVector());
                }
                else
                    cout << "MEMBERSHIP PURCHASE CANCELLED." << endl;
            }
            break;
        case 3:
            cancelMembership();
            rewriteUsersFile(lib->getUsersVector());
            break;
        case 4:
            cout << "RETURNING TO MAIN MENU..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 4);
}

//==============================================================================
//     USER MAIN MENU
void User::userMenu(library *lib)
{
    int choice = 0;
    do
    {
        cout << "\n================ USER MAIN MENU ================" << endl;
        cout << "1. VIEW DASHBOARD" << endl;
        cout << "2. BOOK SEARCH" << endl;
        cout << "3. BOOK ACTIONS (BORROW / RETURN / READ / DOWNLOAD)" << endl;
        cout << "4. RESERVATIONS" << endl;
        cout << "5. MEMBERSHIP" << endl;
        cout << "6. UPDATE PROFILE" << endl;
        cout << "7. EXIT" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 7);

        switch (choice)
        {
        case 1:
            viewFullDetails(lib);
            break;
        case 2:
            searchBooks(lib);
            break;
        case 3:
            bookActionMenu(lib);
            break;
        case 4:
            reservationMenu(lib);
            break;
        case 5:
            membershipMenu(lib);
            break;
        case 6:
            updateUser(lib);
            break;
        case 7:
            cout << "EXITING..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 7);
}

//=============================================
//     FULL DASHBOARD
void User::viewFullDetails(library *lib)
{
    int choice;
    do
    {
        cout << "\n================ USER DASHBOARD ================" << endl;
        cout << "1. VIEW YOUR DATA" << endl;
        cout << "2. VIEW BORROWED BOOKS" << endl;
        cout << "3. VIEW TRANSACTIONS" << endl;
        cout << "4. VIEW FINES" << endl;
        cout << "5. VIEW RESERVATIONS" << endl;
        cout << "6. EXIT" << endl;

        choice = getValidIntInRange("ENTER CHOICE: ", 1, 6);

        switch (choice)
        {
        case 1:
            cout << "\n--------------- USER DETAILS ---------------" << endl;
            cout << *this;
            cout << "BORROW LIMIT  : " << getEffectiveBorrowLimit()
                 << (hasMembership() ? " [MEMBERSHIP ACTIVE]" : " [NO MEMBERSHIP]") << endl;
            cout << "FINE DISCOUNT : " << (hasMembership() ? "15% (MEMBERSHIP ACTIVE)" : "NONE") << endl;
            cout << "CURRENT BALANCE  : Rs." << balance << endl;
            if (total_fines_paid > 0)
                cout << "TOTAL FINES PAID : Rs." << total_fines_paid << " (lifetime)" << endl;
            if (balance < 0)
                cout << "WARNING: NEGATIVE BALANCE! BORROWING RESTRICTED UNTIL CLEARED." << endl;
            break;
        case 2:
        {
            cout << "\n| BORROWED BOOKS : " << getBorrowedCount() << endl;
            if (borrowedbook.empty())
            {
                cout << "|   NO BOOKS BORROWED YET." << endl;
            }
            else
            {
                cout << "|" << endl;
                for (int i = 0; i < (int)borrowedbook.size(); i++)
                {
                    Book *b = lib->getCatalog()->SearchbyID(borrowedbook[i]);
                    if (b)
                    {
                        cout << "|   " << (i + 1) << ". " << b->get_title()
                             << " by " << b->get_author()
                             << " (ID: " << b->get_bookid() << ")" << endl;
                    }
                }
            }
            break;
        }
        case 3:
            cout << "\n------------ TRANSACTIONS ------------" << endl;
            lib->viewUserTransactions(userid);
            break;
        case 4:
            cout << "\n--------------- FINES ---------------" << endl;
            lib->viewUserFines(userid);
            break;
        case 5:
            cout << "\n----------- RESERVATIONS -----------" << endl;
            lib->viewUserReservations(userid);
            break;
        case 6:
            cout << "EXITING DASHBOARD..." << endl;
            break;
        default:
            cout << "INVALID CHOICE!" << endl;
        }
    } while (choice != 6);
}
