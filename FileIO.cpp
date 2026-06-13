#include "FileIO.h"

// ---- USER FILE I/O ----
void saveUserToFile(User *u)
{
    ofstream file("users.dat", ios::binary | ios::app);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN users.dat FOR WRITING!");

    UserRecord ur = {};
    ur.id = u->get_userid();
    strncpy(ur.name, u->get_name().c_str(), sizeof(ur.name) - 1);
    ur.name[sizeof(ur.name) - 1] = '\0';
    strncpy(ur.email, u->get_email().c_str(), sizeof(ur.email) - 1);
    ur.email[sizeof(ur.email) - 1] = '\0';
    strncpy(ur.phone, u->get_phone().c_str(), sizeof(ur.phone) - 1);
    ur.phone[sizeof(ur.phone) - 1] = '\0';
    strncpy(ur.cnic, u->get_cnic().c_str(), sizeof(ur.cnic) - 1);
    ur.cnic[sizeof(ur.cnic) - 1] = '\0';
    strncpy(ur.type, u->get_usertype().c_str(), sizeof(ur.type) - 1);
    ur.type[sizeof(ur.type) - 1] = '\0';
    strncpy(ur.username, u->getUsername().c_str(), sizeof(ur.username) - 1);
    ur.username[sizeof(ur.username) - 1] = '\0';
    strncpy(ur.password, u->getPassword().c_str(), sizeof(ur.password) - 1);
    ur.password[sizeof(ur.password) - 1] = '\0';

    if (u->get_usertype() == "STUDENT")
    {
        Student *s = dynamic_cast<Student *>(u);
        strncpy(ur.seatno, s->getSeatNo().c_str(), sizeof(ur.seatno) - 1);
        ur.seatno[sizeof(ur.seatno) - 1] = '\0';
        strncpy(ur.major, s->getMajor().c_str(), sizeof(ur.major) - 1);
        ur.major[sizeof(ur.major) - 1] = '\0';
    }
    else if (u->get_usertype() == "FACULTY")
    {
        Faculty *f = dynamic_cast<Faculty *>(u);
        strncpy(ur.department, f->getDepartment().c_str(), sizeof(ur.department) - 1);
        ur.department[sizeof(ur.department) - 1] = '\0';
        strncpy(ur.designation, f->getDesignation().c_str(), sizeof(ur.designation) - 1);
        ur.designation[sizeof(ur.designation) - 1] = '\0';
    }
    else if (u->get_usertype() == "GUEST")
    {
        Guest *g = dynamic_cast<Guest *>(u);
        strncpy(ur.purpose, g->getPurpose().c_str(), sizeof(ur.purpose) - 1);
        ur.purpose[sizeof(ur.purpose) - 1] = '\0';
    }
    ur.borrowed_count = u->getBorrowedCount();
    for (int i = 0; i < ur.borrowed_count && i < MAX_BORROWED_BOOKS; i++)
    {
        ur.borrowed_ids[i] = u->getBorrowedBooks()[i];
    }
    ur.daily_borrows = u->getDailyBorrows();
    strncpy(ur.last_borrow_date, u->getLastBorrowDate().c_str(), sizeof(ur.last_borrow_date) - 1);
    ur.last_borrow_date[sizeof(ur.last_borrow_date) - 1] = '\0';
    ur.balance = u->getBalance();
    ur.total_fines_paid = u->getTotalFinesPaid();
    ur.isDeleted = u->getIsDeleted();

    if (u->getMembership() && u->getMembership()->isValid())
    {
        ur.hasMembership = true;
        ur.membershipActivation = u->getMembership()->getActivationTime();
        ur.membershipExpiry = u->getMembership()->getExpiryTime();
        ur.membershipDurationDays = DEFAULT_MEMBERSHIP_DAYS;
    }
    else
    {
        ur.hasMembership = false;
        ur.membershipActivation = 0;
        ur.membershipExpiry = 0;
        ur.membershipDurationDays = 0;
    }
    file.write((char *)&ur, sizeof(ur));
    file.close();
}

void loadUsersFromFile(vector<User *> &users)
{
    ifstream file("users.dat", ios::binary);
    if (!file)
        return;

    UserRecord ur;
    while (file.read((char *)&ur, sizeof(ur)))
    {
        User *u = nullptr;
        string type = ur.type;

        if (type == "STUDENT")
            u = new Student(ur.name, ur.cnic, ur.email, ur.phone, ur.seatno, ur.major);
        else if (type == "FACULTY")
            u = new Faculty(ur.name, ur.cnic, ur.email, ur.phone, ur.department, ur.designation);
        else
            u = new Guest(ur.name, ur.cnic, ur.email, ur.phone, ur.purpose);

        u->setid(ur.id);
        u->setUsername(ur.username);
        u->setPassword(ur.password);
        u->setDailyBorrows(ur.daily_borrows);
        u->setLastBorrowDate(ur.last_borrow_date);
        u->setBalance(ur.balance);
        u->setTotalFinesPaid(ur.total_fines_paid);
        u->setIsDeleted(ur.isDeleted);

        if (ur.hasMembership)
        {
            u->restoreMembership(ur.membershipActivation, ur.membershipExpiry);
        }
        for (int i = 0; i < ur.borrowed_count; i++)
        {
            u->borrowbook(ur.borrowed_ids[i]);
        }
        users.push_back(u);
    }
    file.close();
}

void rewriteUsersFile(vector<User *> &users)
{
    ofstream file("users.dat", ios::binary);
    for (int i = 0; i < (int)users.size(); i++)
    {
        User *u = users[i];
        UserRecord ur = {};
        ur.id = u->get_userid();
        strncpy(ur.name, u->get_name().c_str(), sizeof(ur.name) - 1);
        ur.name[sizeof(ur.name) - 1] = '\0';
        strncpy(ur.email, u->get_email().c_str(), sizeof(ur.email) - 1);
        ur.email[sizeof(ur.email) - 1] = '\0';
        strncpy(ur.phone, u->get_phone().c_str(), sizeof(ur.phone) - 1);
        ur.phone[sizeof(ur.phone) - 1] = '\0';
        strncpy(ur.cnic, u->get_cnic().c_str(), sizeof(ur.cnic) - 1);
        ur.cnic[sizeof(ur.cnic) - 1] = '\0';
        strncpy(ur.type, u->get_usertype().c_str(), sizeof(ur.type) - 1);
        ur.type[sizeof(ur.type) - 1] = '\0';
        strncpy(ur.username, u->getUsername().c_str(), sizeof(ur.username) - 1);
        ur.username[sizeof(ur.username) - 1] = '\0';
        strncpy(ur.password, u->getPassword().c_str(), sizeof(ur.password) - 1);
        ur.password[sizeof(ur.password) - 1] = '\0';

        if (u->get_usertype() == "STUDENT")
        {
            Student *s = dynamic_cast<Student *>(u);
            strncpy(ur.seatno, s->getSeatNo().c_str(), sizeof(ur.seatno) - 1);
            ur.seatno[sizeof(ur.seatno) - 1] = '\0';
            strncpy(ur.major, s->getMajor().c_str(), sizeof(ur.major) - 1);
            ur.major[sizeof(ur.major) - 1] = '\0';
        }
        else if (u->get_usertype() == "FACULTY")
        {
            Faculty *f = dynamic_cast<Faculty *>(u);
            strncpy(ur.department, f->getDepartment().c_str(), sizeof(ur.department) - 1);
            ur.department[sizeof(ur.department) - 1] = '\0';
            strncpy(ur.designation, f->getDesignation().c_str(), sizeof(ur.designation) - 1);
            ur.designation[sizeof(ur.designation) - 1] = '\0';
        }
        else if (u->get_usertype() == "GUEST")
        {
            Guest *g = dynamic_cast<Guest *>(u);
            strncpy(ur.purpose, g->getPurpose().c_str(), sizeof(ur.purpose) - 1);
            ur.purpose[sizeof(ur.purpose) - 1] = '\0';
        }

        ur.borrowed_count = u->getBorrowedCount();
        for (int j = 0; j < ur.borrowed_count && j < MAX_BORROWED_BOOKS; j++)
            ur.borrowed_ids[j] = u->getBorrowedBooks()[j];

        ur.daily_borrows = u->getDailyBorrows();
        strncpy(ur.last_borrow_date, u->getLastBorrowDate().c_str(), sizeof(ur.last_borrow_date) - 1);
        ur.last_borrow_date[sizeof(ur.last_borrow_date) - 1] = '\0';
        ur.balance = u->getBalance();
        ur.total_fines_paid = u->getTotalFinesPaid();
        ur.isDeleted = u->getIsDeleted();

        if (u->getMembership() && u->getMembership()->isValid())
        {
            ur.hasMembership = true;
            ur.membershipActivation = u->getMembership()->getActivationTime();
            ur.membershipExpiry = u->getMembership()->getExpiryTime();
            ur.membershipDurationDays = DEFAULT_MEMBERSHIP_DAYS;
        }
        else
        {
            ur.hasMembership = false;
            ur.membershipActivation = 0;
            ur.membershipExpiry = 0;
            ur.membershipDurationDays = 0;
        }
        file.write((char *)&ur, sizeof(ur));
    }
    file.close();
}

// ---- BOOK FILE I/O ----
void saveBookToFile(Book *b)
{
    ofstream file("books.dat", ios::binary | ios::app);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN books.dat FOR WRITING!");

    BookRecord br;
    br.id = b->get_bookid();
    strncpy(br.title, b->get_title().c_str(), sizeof(br.title) - 1);
    br.title[sizeof(br.title) - 1] = '\0';
    strncpy(br.author, b->get_author().c_str(), sizeof(br.author) - 1);
    br.author[sizeof(br.author) - 1] = '\0';
    strncpy(br.category, b->get_category().c_str(), sizeof(br.category) - 1);
    br.category[sizeof(br.category) - 1] = '\0';
    strncpy(br.isbn, b->get_isbn().c_str(), sizeof(br.isbn) - 1);
    br.isbn[sizeof(br.isbn) - 1] = '\0';
    strncpy(br.publisher, b->get_publisher().c_str(), sizeof(br.publisher) - 1);
    br.publisher[sizeof(br.publisher) - 1] = '\0';
    br.year = b->get_YOP();
    br.copies = b->getAvailableCopies();
    br.download_count = b->get_downloadcount();
    br.read_onsite_count = b->get_read_onsitecount();
    br.times_borrowed = b->get_timesborrowed_count();

    file.write((char *)&br, sizeof(br));
    if (!file)
        throw FileException("ERROR: FAILED TO WRITE BOOK RECORD TO FILE!");
    file.close();
}

void loadBooksFromFile(BookCatalog *catalog)
{
    ifstream file("books.dat", ios::binary);
    if (!file)
        return;

    BookRecord br;
    while (file.read((char *)&br, sizeof(br)))
    {
        Book *b = new Book(br.title, br.author, br.category, br.isbn, br.publisher, br.year);
        if (br.id >= Book::getNextID())
            Book::setNextID(br.id + 1);
        b->set_bookid(br.id);
        b->setAvailableCopies(br.copies);

        for (int i = 0; i < br.download_count; i++)
            b->incrementDownloadCount();
        for (int i = 0; i < br.read_onsite_count; i++)
            b->incrementReadOnSiteCount();
        for (int i = 0; i < br.times_borrowed; i++)
            b->incrementBorrowCount();

        catalog->addbook(b);
    }
    file.close();
}

void rewriteBooksFile(BookCatalog *catalog)
{
    ofstream file("books.dat", ios::binary);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN books.dat FOR REWRITING!");

    for (int i = 0; i < catalog->get_bookcount(); i++)
    {
        Book *b = catalog->getBookAtIndex(i);
        BookRecord br;
        br.id = b->get_bookid();
        strncpy(br.title, b->get_title().c_str(), sizeof(br.title) - 1);
        br.title[sizeof(br.title) - 1] = '\0';
        strncpy(br.author, b->get_author().c_str(), sizeof(br.author) - 1);
        br.author[sizeof(br.author) - 1] = '\0';
        strncpy(br.category, b->get_category().c_str(), sizeof(br.category) - 1);
        br.category[sizeof(br.category) - 1] = '\0';
        strncpy(br.isbn, b->get_isbn().c_str(), sizeof(br.isbn) - 1);
        br.isbn[sizeof(br.isbn) - 1] = '\0';
        strncpy(br.publisher, b->get_publisher().c_str(), sizeof(br.publisher) - 1);
        br.publisher[sizeof(br.publisher) - 1] = '\0';
        br.year = b->get_YOP();
        br.copies = b->getAvailableCopies();
        br.download_count = b->get_downloadcount();
        br.read_onsite_count = b->get_read_onsitecount();
        br.times_borrowed = b->get_timesborrowed_count();

        file.write((char *)&br, sizeof(br));
        if (!file)
            throw FileException("ERROR: FAILED TO WRITE DURING BOOK FILE REWRITE!");
    }
    file.close();
}

// ---- TRANSACTION FILE I/O ----
void saveTransactionToFile(Transaction &t)
{
    ofstream file("transactions.dat", ios::binary | ios::app);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN transactions.dat FOR WRITING!");

    TransactionRecord tr = {};
    tr.tid = t.getTransactionID();
    tr.uid = t.getuserid();
    tr.bid = t.getbookid();
    tr.borrow_time = t.getBorrowTime();
    tr.due_time = t.getDueTime();
    tr.return_time = t.getReturnTime();
    tr.returned = t.isReturned();
    strncpy(tr.return_date, t.getReturnDate().c_str(), sizeof(tr.return_date) - 1);
    tr.return_date[sizeof(tr.return_date) - 1] = '\0';

    file.write((char *)&tr, sizeof(tr));
    file.close();
}

void loadTransactionsFromFile(vector<Transaction> &transactions, vector<User *> &users)
{
    ifstream file("transactions.dat", ios::binary);
    if (!file)
        return;

    TransactionRecord tr;
    while (file.read((char *)&tr, sizeof(tr)))
    {
        Transaction t;
        t.setTransactionID(tr.tid);
        t.setUserID(tr.uid);
        t.setBookID(tr.bid);
        t.setBorrowTime(tr.borrow_time);
        t.setDueTime(tr.due_time);
        t.setReturnTime(tr.return_time);
        t.setReturnDate(tr.return_date);
        t.setReturned(tr.returned);
        transactions.push_back(t);

        if (!tr.returned)
        {
            for (int i = 0; i < (int)users.size(); i++)
            {
                if (users[i]->get_userid() == tr.uid)
                {
                    users[i]->borrowbook(tr.bid);
                    break;
                }
            }
        }
    }
    file.close();
}

void rewriteTransactionsFile(vector<Transaction> &transactions)
{
    ofstream file("transactions.dat", ios::binary);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN transactions.dat FOR REWRITING!");

    for (int i = 0; i < (int)transactions.size(); i++)
    {
        TransactionRecord tr = {};
        tr.tid = transactions[i].getTransactionID();
        tr.uid = transactions[i].getuserid();
        tr.bid = transactions[i].getbookid();
        tr.borrow_time = transactions[i].getBorrowTime();
        tr.due_time = transactions[i].getDueTime();
        tr.return_time = transactions[i].getReturnTime();
        tr.returned = transactions[i].isReturned();
        strncpy(tr.return_date, transactions[i].getReturnDate().c_str(), sizeof(tr.return_date) - 1);
        tr.return_date[sizeof(tr.return_date) - 1] = '\0';
        file.write((char *)&tr, sizeof(tr));
    }
    file.close();
}

// ---- FINES FILE I/O ----
void saveFineToFile(Fines &f)
{
    ofstream file("fines.dat", ios::binary | ios::app);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN fines.dat FOR WRITING!");

    FineRecord fr = {};
    fr.user_id = f.getUserID();
    fr.book_id = f.getBookID();
    fr.minuteslate = f.getMinutesLate();
    fr.rate = f.getRate();
    fr.originalAmount = f.getOriginal();
    fr.finalAmount = f.getAmount();
    fr.discountApplied = f.wasDiscounted();

    file.write((char *)&fr, sizeof(fr));
    file.close();
}

void loadFinesFromFile(vector<Fines> &fines)
{
    ifstream file("fines.dat", ios::binary);
    if (!file)
        return;

    FineRecord fr;
    while (file.read((char *)&fr, sizeof(fr)))
    {
        Fines f;
        f.setUserID(fr.user_id);
        f.setBookID(fr.book_id);
        f.setMinutesLate(fr.minuteslate);
        f.setRate(fr.rate);
        f.setOriginalAmount(fr.originalAmount);
        f.setFinalAmount(fr.finalAmount);
        f.setDiscountApplied(fr.discountApplied);
        fines.push_back(f);
    }
    file.close();
}

void rewriteFinesFile(vector<Fines> &fines)
{
    ofstream file("fines.dat", ios::binary);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN fines.dat FOR REWRITING!");

    for (int i = 0; i < (int)fines.size(); i++)
    {
        FineRecord fr = {};
        fr.user_id = fines[i].getUserID();
        fr.book_id = fines[i].getBookID();
        fr.minuteslate = fines[i].getMinutesLate();
        fr.rate = fines[i].getRate();
        fr.originalAmount = fines[i].getOriginal();
        fr.finalAmount = fines[i].getAmount();
        fr.discountApplied = fines[i].wasDiscounted();
        file.write((char *)&fr, sizeof(fr));
    }
    file.close();
}

// ---- RESERVATION FILE I/O ----
void saveReservationToFile(Reservation &r)
{
    ofstream file("reservations.dat", ios::binary | ios::app);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN reservations.dat FOR WRITING!");

    ReservationRecord rr = {};
    rr.reservationid = r.get_reservationid();
    rr.userid = r.get_userid();
    rr.bookid = r.get_bookid();
    rr.tableid = r.get_tableid();
    rr.active = r.isActive();
    strncpy(rr.date, r.get_date().c_str(), sizeof(rr.date) - 1);
    strncpy(rr.startTime, r.get_start().c_str(), sizeof(rr.startTime) - 1);
    strncpy(rr.endTime, r.get_end().c_str(), sizeof(rr.endTime) - 1);

    file.write((char *)&rr, sizeof(rr));
    file.close();
}

void loadReservationsFromFile(vector<Reservation> &reservations)
{
    ifstream file("reservations.dat", ios::binary);
    if (!file)
        return;

    ReservationRecord rr;
    while (file.read((char *)&rr, sizeof(rr)))
    {
        Reservation r(rr.userid, rr.bookid, rr.tableid, rr.date, rr.startTime, rr.endTime);
        r.setReservationID(rr.reservationid);
        if (!rr.active)
            r.cancel();
        reservations.push_back(r);
    }
    file.close();
}

void rewriteReservationsFile(vector<Reservation> &reservations)
{
    ofstream file("reservations.dat", ios::binary);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN reservations.dat FOR REWRITING!");

    for (int i = 0; i < (int)reservations.size(); i++)
    {
        ReservationRecord rr = {};
        rr.reservationid = reservations[i].get_reservationid();
        rr.userid = reservations[i].get_userid();
        rr.bookid = reservations[i].get_bookid();
        rr.tableid = reservations[i].get_tableid();
        rr.active = reservations[i].isActive();
        strncpy(rr.date, reservations[i].get_date().c_str(), sizeof(rr.date) - 1);
        strncpy(rr.startTime, reservations[i].get_start().c_str(), sizeof(rr.startTime) - 1);
        strncpy(rr.endTime, reservations[i].get_end().c_str(), sizeof(rr.endTime) - 1);
        file.write((char *)&rr, sizeof(rr));
    }
    file.close();
}

// ---- TABLE FILE I/O ----
void saveTableToFile(Table &t)
{
    ofstream file("tables.dat", ios::binary | ios::app);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN tables.dat FOR WRITING!");

    TableRecord tr = {};
    tr.id = t.get_id();
    strncpy(tr.location, t.get_location().c_str(), sizeof(tr.location) - 1);
    tr.location[sizeof(tr.location) - 1] = '\0';
    tr.capacity = t.get_capacity();

    file.write((char *)&tr, sizeof(tr));
    file.close();
}

void loadTablesFromFile(vector<Table> &tables)
{
    ifstream file("tables.dat", ios::binary);
    if (!file)
        return;

    TableRecord tr;
    while (file.read((char *)&tr, sizeof(tr)))
    {
        Table t;
        t.setID(tr.id);
        t.setLocation(tr.location);
        t.setCapacity(tr.capacity);
        tables.push_back(t);
    }
    file.close();
}

void rewriteTablesFile(vector<Table> &tables)
{
    ofstream file("tables.dat", ios::binary);
    if (!file)
        throw FileException("ERROR: COULD NOT OPEN tables.dat FOR REWRITING!");

    for (int i = 0; i < (int)tables.size(); i++)
    {
        TableRecord tr = {};
        tr.id = tables[i].get_id();
        strncpy(tr.location, tables[i].get_location().c_str(), sizeof(tr.location) - 1);
        tr.location[sizeof(tr.location) - 1] = '\0';
        tr.capacity = tables[i].get_capacity();
        file.write((char *)&tr, sizeof(tr));
        if (!file)
            throw FileException("ERROR: FAILED TO WRITE TABLE RECORD DURING REWRITE!");
    }
    file.close();
}
