#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>
using namespace std;

// ============================================================
//                    TABLE CLASS
//============================================================
class Table
{
private:
    int tableid;
    static int next_id;

    string location;
    int capacity;

public:
    // DEFAULT CONSTRUCTOR
    Table()
    {
        tableid = next_id++;
        location = "UNKNOWN";
        capacity = 1;
    }

    // PARAMETERIZED CONSTRUCTOR
    Table(string loc, int cap)
    {
        tableid = next_id++;
        location = loc;
        capacity = cap;
    }

    int get_id() { return tableid; }
    string get_location() { return location; }
    int get_capacity() { return capacity; }
    void setID(int id) { tableid = id; }
    void setLocation(string loc) { location = loc; }
    void setCapacity(int cap) { capacity = cap; }

    // DISPLAY via friend ostream
    friend ostream &operator<<(ostream &os, const Table &t)
    {
        os << "================= TABLE DETAILS =================" << endl
           << "| TABLE ID   : " << t.tableid << endl
           << "| LOCATION   : " << t.location << endl
           << "| CAPACITY   : " << t.capacity << endl
           << "=================================================" << endl;
        return os;
    }
};

#endif // TABLE_H
