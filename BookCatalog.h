#ifndef BOOKCATALOG_H
#define BOOKCATALOG_H

#include <vector>
#include "Book.h"
#include "Utils.h"
#include "Exceptions.h"
using namespace std;

// ============================================================
//                    BOOK CATALOG CLASS
//==============================================================
class BookCatalog
{
private:
    vector<Book *> BK;

public:
    // CONSTRUCTOR
    BookCatalog() {}
    // DESTRUCTOR (deletes all dynamically allocated Book objects to prevent memory leaks)
    ~BookCatalog()
    {
        for (int i = 0; i < (int)BK.size(); i++)
            delete BK[i];
        BK.clear();
    }

    void addbook(Book *b)
    {
        BK.push_back(b);
        if (!RuntimeLogger::isSilent())
            cout << "BOOK ADDED SUCCESSFULLY!" << endl;
    }

    void removeBook(string t)
    {
        if (BK.empty())
        {
            cout << "NO BOOKS IN CATALOG!" << endl;
            return;
        }
        for (int i = 0; i < (int)BK.size(); i++)
        {
            if (BK[i]->get_title() == t)
            {
                delete BK[i];
                BK.erase(BK.begin() + i);
                cout << "BOOK REMOVED!" << endl;
                return;
            }
        }
        cout << "BOOK NOT FOUND!" << endl;
    }

    // ================= SEARCH FUNCTIONS =================
    Book *SearchbyID(int id)
    {
        for (int i = 0; i < BK.size(); i++)
        {
            Book *b = BK[i];
            if (b->get_bookid() == id)
                return b;
        }
        return nullptr;
    }

    Book *SearchbyTitle(string t)
    {
        for (int i = 0; i < BK.size(); i++)
        {
            Book *b = BK[i];
            if (b->get_title() == t)
                return b;
        }
        return nullptr;
    }

    vector<Book *> SearchbyAuthor(string a)
    {
        vector<Book *> results;
        for (int i = 0; i < (int)BK.size(); i++)
            if (BK[i]->get_author() == a)
                results.push_back(BK[i]);
        return results;
    }

    vector<Book *> SearchbyCategory(string c)
    {
        vector<Book *> results;
        for (int i = 0; i < (int)BK.size(); i++)
            if (BK[i]->get_category() == c)
                results.push_back(BK[i]);
        return results;
    }

    vector<Book *> SearchbyPublisher(string p)
    {
        vector<Book *> results;
        for (int i = 0; i < (int)BK.size(); i++)
            if (BK[i]->get_publisher() == p)
                results.push_back(BK[i]);
        return results;
    }

    vector<Book *> SearchbyYOP(int yop)
    {
        vector<Book *> results;
        for (int i = 0; i < (int)BK.size(); i++)
            if (BK[i]->get_YOP() == yop)
                results.push_back(BK[i]);
        return results;
    }

    // ================= DISPLAY =================
    void ViewBookCatalog() const
    {
        if (BK.empty())
        {
            cout << "NO BOOKS IN CATALOG!" << endl;
            return;
        }
        cout << "==== BOOK CATALOG ====" << endl;
        for (Book *b : BK)
            cout << *b;
    }

    // ================= GETTERS =================
    int get_bookcount() { return (int)BK.size(); }

    Book *getBookAtIndex(int index)
    {
        if (index >= 0 && index < (int)BK.size())
            return BK[index];
        return nullptr;
    }
};

#endif // BOOKCATALOG_H
