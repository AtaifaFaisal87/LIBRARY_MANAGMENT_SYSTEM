#ifndef RESERVATION_H
#define RESERVATION_H

#include <iostream>
#include <string>
#include "Utils.h"
using namespace std;

// ============================================================
//                    RESERVATION CLASS
//==============================================================
class Reservation
{
private:
    int reservationid;
    static int next_id;

    int userid;
    int bookid;
    int tableid;

    string date;
    string startTime;
    string endTime;

    bool active;

public:
    // CONSTRUCTOR
    Reservation()
        : reservationid(next_id++), userid(0), bookid(0), tableid(0),
          date("UNKNOWN"), startTime("0"), endTime("0"), active(true) {}
    // PARAMETERIZED CONSTRUCTOR
    Reservation(int uid, int bid, int tid,
                string d, string start, string end)
    {
        reservationid = next_id++;
        userid = uid;
        bookid = bid;
        tableid = tid;
        date = d;
        startTime = start;
        endTime = end;
        active = true;
    }

    // DESTRUCTOR
    ~Reservation() {}

    // GETTERS
    int get_userid() { return userid; }
    int get_tableid() { return tableid; }
    int get_bookid() { return bookid; }
    int get_reservationid() { return reservationid; }
    string get_date() { return date; }
    string get_start() { return startTime; }
    string get_end() { return endTime; }

    void setReservationID(int id) { reservationid = id; }

    bool isActive() { return active; }
    bool isExpired()
    {
        time_t endT = reservationToTimeT(date, endTime);
        return getCurrentTime() > endT;
    }

    void cancel() { active = false; }

    // string comparison works here because times are in HH:MM (lexicographic order = chronological)
    static bool overlap(string s1, string e1, string s2, string e2)
    {
        return !(e1 <= s2 || e2 <= s1);
    }

    // DISPLAY via friend ostream
    friend ostream &operator<<(ostream &os, const Reservation &r)
    {
        if (!r.active)
            return os;

        os << "=============== RESERVATION DETAILS ===============" << endl
           << "| RESERVATION ID : " << r.reservationid << endl
           << "| USER ID        : " << r.userid << endl;

        if (r.bookid)
            os << "| BOOK ID        : " << r.bookid << endl;
        else
            os << "| TABLE ID       : " << r.tableid << endl;

        os << "| DATE           : " << r.date << endl
           << "| START TIME     : " << to12Hour(r.startTime) << endl
           << "| END TIME       : " << to12Hour(r.endTime) << endl
           << "===================================================" << endl;

        return os;
    }

    // Separate function to print inactive reservations
    void printInactive(ostream &os) const
    {
        os << "=============== RESERVATION DETAILS ===============" << endl
           << "| RESERVATION ID : " << reservationid << endl
           << "| USER ID        : " << userid << endl;

        if (bookid)
            os << "| BOOK ID        : " << bookid << endl;
        else
            os << "| TABLE ID       : " << tableid << endl;

        os << "| DATE           : " << date << endl
           << "| START TIME     : " << to12Hour(startTime) << endl
           << "| END TIME       : " << to12Hour(endTime) << endl
           << "| STATUS         : " << (active ? "ACTIVE" : "EXPIRED/CANCELLED") << endl
           << "===================================================" << endl;
    }
};

#endif // RESERVATION_H
