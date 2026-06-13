#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
using namespace std;

// ============================================================
//                    BOOK CLASS
//==============================================================
class Book
{
private:
    static int next_id;
    int bookid;
    string title;
    string author;
    string category;
    string ISBN;
    string publisher;
    int year_of_publication;
    int available_copies = 0;
    bool availability_status;
    int download_count = 0;
    int read_onsite_count = 0;
    int timesborrowed_count = 0;

public:
    // CONSTRUCTOR
    Book() : bookid(next_id++), title("UNKNOWN"), author("UNKNOWN"), category("UNKNOWN"),
             ISBN("UNKNOWN"), publisher("UNKNOWN"), year_of_publication(0),
             available_copies(0), availability_status(false),
             download_count(0), read_onsite_count(0), timesborrowed_count(0) {}
    // PARAMETERIZED CONSTRUCTOR
    Book(string t, string a, string c, string isbn, string p, int yop)
        : bookid(next_id++), title(t), author(a), category(c),
          ISBN(isbn), publisher(p), year_of_publication(yop),
          available_copies(0), availability_status(false),
          download_count(0), read_onsite_count(0), timesborrowed_count(0) {}

    // DESTRUCTOR
    ~Book() {}

    // GETTERS
    int get_bookid() { return bookid; }
    string get_title() { return title; }
    string get_author() { return author; }
    string get_category() { return category; }
    string get_isbn() { return ISBN; }
    string get_publisher() { return publisher; }
    int get_YOP() { return year_of_publication; }
    int getAvailableCopies() { return available_copies; }
    int get_downloadcount() { return download_count; }
    int get_read_onsitecount() { return read_onsite_count; }
    int get_timesborrowed_count() { return timesborrowed_count; }
    void decreaseCopy()
    {
        if (available_copies > 0)
            available_copies--;
    }

    // SETTERS
    void set_bookid(int id) { bookid = id; }
    void set_author(string a) { author = a; }
    void set_title(string t) { title = t; }
    void set_category(string c) { category = c; }
    void set_isbn(string isbn) { ISBN = isbn; }
    void set_publisher(string p) { publisher = p; }
    void set_YOP(int yop) { year_of_publication = yop; }

    // STATIC ID MANAGEMENT
    static void setNextID(int id) { next_id = id; }
    static int getNextID() { return next_id; }

    void setAvailableCopies(int ac)
    {
        available_copies = ac;
        if (available_copies > 0)
            availability_status = true;
        else
            availability_status = false;
    }

    // ================= COUNTER FUNCTIONS =================
    void incrementBorrowCount()
    {
        if (available_copies >= 0)
            timesborrowed_count++;
    }

    void incrementDownloadCount()
    {
        download_count++;
    }

    void incrementReadOnSiteCount()
    {
        read_onsite_count++;
    }

    //================= OPERATOR OVERLOADING ==================
    bool operator==(const Book &other) const { return bookid == other.bookid; }
    bool operator==(const string &t) const { return title == t; }
    bool operator==(const int &y) const { return year_of_publication == y; }

    // ================= DISPLAY =================
    friend ostream &operator<<(ostream &os, const Book &b)
    {
        os << "================= BOOK DETAILS =================" << endl
           << "| BOOK ID          : " << b.bookid << endl
           << "| TITLE            : " << b.title << endl
           << "| AUTHOR           : " << b.author << endl
           << "| CATEGORY         : " << b.category << endl
           << "| ISBN             : " << b.ISBN << endl
           << "| PUBLISHER        : " << b.publisher << endl
           << "| YEAR             : " << b.year_of_publication << endl
           << "| AVAILABLE COPIES : " << b.available_copies << endl
           << "| STATUS           : " << (b.availability_status ? "AVAILABLE" : "NOT AVAILABLE") << endl
           << "| DOWNLOADED       : " << b.download_count << " TIMES" << endl
           << "| READ ON SITE     : " << b.read_onsite_count << " TIMES" << endl
           << "| TIMES BORROWED   : " << b.timesborrowed_count << " TIMES" << endl
           << "================================================" << endl;
        return os;
    }
};

#endif // BOOK_H
