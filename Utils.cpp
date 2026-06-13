#include "Utils.h"

// Initialize static member
bool RuntimeLogger::suppressMessages = false;

time_t getCurrentTime()
{
    return time(nullptr);
}

string timeToString(time_t t)
{
    if (t == 0)
        return "N/A";
    return ctime(&t);
}

string to12Hour(string timeStr)
{
    int hours = stoi(timeStr.substr(0, 2));
    int minutes = stoi(timeStr.substr(3, 2));
    string period = (hours >= 12) ? "PM" : "AM";
    if (hours == 0)
        hours = 12;
    else if (hours > 12)
        hours -= 12;
    char buf[9];
    sprintf(buf, "%02d:%02d %s", hours, minutes, period.c_str());
    return string(buf);
}

time_t reservationToTimeT(string date, string timeStr)
{
    struct tm t = {};
    t.tm_mday = stoi(date.substr(0, 2));
    t.tm_mon = stoi(date.substr(3, 2)) - 1;
    t.tm_year = stoi(date.substr(6, 4)) - 1900;
    t.tm_hour = stoi(timeStr.substr(0, 2));
    t.tm_min = stoi(timeStr.substr(3, 2));
    t.tm_sec = 0;
    t.tm_isdst = -1;
    return mktime(&t);
}

int minutesDiff(time_t start, time_t end)
{
    double diff = difftime(end, start);
    return (diff > 0) ? (int)(diff / 60) : 0;
}

time_t addMinutes(time_t t, int minutes)
{
    return t + (time_t)(minutes * 60);
}

string getValidDate(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);

        bool valid = true;
        if (value.size() != 10)
            valid = false;
        else if (value[2] != '/' || value[5] != '/')
            valid = false;
        else
        {
            for (int i = 0; i < 10; i++)
            {
                if (i == 2 || i == 5)
                    continue;
                if (!isdigit(value[i]))
                {
                    valid = false;
                    break;
                }
            }
            int day = stoi(value.substr(0, 2));
            int month = stoi(value.substr(3, 2));
            int year = stoi(value.substr(6, 4));
            if (day < 1 || day > 31)
                valid = false;
            if (month < 1 || month > 12)
                valid = false;
            if (year < 2000 || year > 2100)
                valid = false;
        }
        if (!valid)
            cout << "INVALID DATE! FORMAT MUST BE: DD/MM/YYYY" << endl;
        else
            return value;
    }
}

string getValidTime(string prompt)
{
    string value;
    string period;
    while (true)
    {
        cout << prompt;
        getline(cin, value);

        bool valid = true;
        if (value.size() != 5)
            valid = false;
        else if (value[2] != ':')
            valid = false;
        else
        {
            for (int i = 0; i < 5; i++)
            {
                if (i == 2)
                    continue;
                if (!isdigit(value[i]))
                {
                    valid = false;
                    break;
                }
            }
            int hours = stoi(value.substr(0, 2));
            int minutes = stoi(value.substr(3, 2));
            if (hours < 1 || hours > 12)
                valid = false;
            if (minutes < 0 || minutes > 59)
                valid = false;
        }
        if (!valid)
        {
            cout << "INVALID TIME! FORMAT MUST BE: HH:MM (1-12)" << endl;
            continue;
        }
        cout << "ENTER AM OR PM: ";
        getline(cin, period);
        if (period != "AM" && period != "PM" && period != "am" && period != "pm")
        {
            cout << "INVALID! ENTER AM OR PM" << endl;
            continue;
        }
        if (period == "am")
            period = "AM";
        if (period == "pm")
            period = "PM";

        int hours = stoi(value.substr(0, 2));
        int minutes = stoi(value.substr(3, 2));

        if (period == "AM" && hours == 12)
            hours = 0;
        if (period == "PM" && hours != 12)
            hours += 12;

        char buf[6];
        sprintf(buf, "%02d:%02d", hours, minutes);
        return string(buf);
    }
}

bool isValidReservationTime(string date, string start, string end)
{
    int startHour = stoi(start.substr(0, 2));
    int endHour = stoi(end.substr(0, 2));

    if (startHour >= 0 && startHour < 5)
    {
        cout << "LIBRARY IS CLOSED FROM 12 AM TO 5 AM! PLEASE ENTER A VALID TIME." << endl;
        return false;
    }
    if (endHour >= 0 && endHour < 5)
    {
        cout << "LIBRARY IS CLOSED FROM 12 AM TO 5 AM! PLEASE ENTER A VALID TIME." << endl;
        return false;
    }

    time_t startT = reservationToTimeT(date, start);
    time_t endT = reservationToTimeT(date, end);
    time_t now = getCurrentTime();

    if (startT <= now)
    {
        cout << "START TIME IS IN THE PAST! PLEASE ENTER A FUTURE TIME." << endl;
        return false;
    }
    if (endT <= startT)
    {
        cout << "END TIME MUST BE AFTER START TIME!" << endl;
        return false;
    }
    return true;
}

int getValidInt(string prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "INVALID INPUT! PLEASE ENTER A NUMBER." << endl;
        }
        else
        {
            cin.ignore(1000, '\n');
            return value;
        }
    }
}

int getValidIntInRange(string prompt, int min, int max)
{
    int value;
    while (true)
    {
        value = getValidInt(prompt);
        if (value < min || value > max)
            cout << "PLEASE ENTER A NUMBER BETWEEN " << min << " AND " << max << "." << endl;
        else
            return value;
    }
}

string getValidString(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);
        if (value.empty())
            cout << "INPUT CANNOT BE EMPTY!" << endl;
        else
            return value;
    }
}

string getValidPublisher(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);
        if (value.empty())
        {
            cout << "INPUT CANNOT BE EMPTY!" << endl;
            continue;
        }
        bool valid = true;
        for (int i = 0; i < (int)value.size(); i++)
        {
            if (!isalpha(value[i]) && value[i] != ' ' &&
                value[i] != '.' && value[i] != '&' && value[i] != '-')
            {
                valid = false;
                break;
            }
        }
        if (!valid)
            cout << "INVALID PUBLISHER! ONLY LETTERS, SPACES, '.', '&', '-' ALLOWED." << endl;
        else
            return value;
    }
}

string getValidISBN(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);
        if (value.empty())
        {
            cout << "INPUT CANNOT BE EMPTY!" << endl;
            continue;
        }
        string digitsOnly = "";
        bool valid = true;
        for (int i = 0; i < (int)value.size(); i++)
        {
            if (isdigit(value[i]))
                digitsOnly += value[i];
            else if (value[i] != '-')
            {
                valid = false;
                break;
            }
        }
        if (!valid || (digitsOnly.size() != 10 && digitsOnly.size() != 13))
            cout << "INVALID ISBN! MUST BE 10 OR 13 DIGITS (HYPHENS OPTIONAL)." << endl;
        else
            return value;
    }
}

string getValidAlphaString(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);
        if (value.empty())
        {
            cout << "INPUT CANNOT BE EMPTY!" << endl;
            continue;
        }
        bool valid = true;
        for (int i = 0; i < (int)value.size(); i++)
        {
            if (!isalpha(value[i]) && value[i] != ' ')
            {
                valid = false;
                break;
            }
        }
        if (!valid)
            cout << "ONLY LETTERS ALLOWED!" << endl;
        else
            return value;
    }
}

string getValidCNIC(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);
        bool valid = true;
        if (value.size() != 15)
            valid = false;
        else
        {
            for (int i = 0; i < 15; i++)
            {
                if (i == 5 || i == 13)
                {
                    if (value[i] != '-')
                    {
                        valid = false;
                        break;
                    }
                }
                else
                {
                    if (!isdigit(value[i]))
                    {
                        valid = false;
                        break;
                    }
                }
            }
        }
        if (!valid)
            cout << "INVALID CNIC! FORMAT MUST BE: xxxxx-xxxxxxx-x" << endl;
        else
            return value;
    }
}

string getValidEmail(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);
        bool hasAt = false;
        bool hasDot = false;
        for (int i = 0; i < (int)value.size(); i++)
        {
            if (value[i] == '@')
                hasAt = true;
            if (value[i] == '.' && hasAt)
                hasDot = true;
        }
        if (value.empty() || !hasAt || !hasDot)
            cout << "INVALID EMAIL! MUST CONTAIN @ AND ." << endl;
        else
            return value;
    }
}

string getValidPhone(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);
        bool valid = true;
        if (value.size() != 11)
            valid = false;
        else if (value[0] != '0' || value[1] != '3')
            valid = false;
        else
        {
            for (int i = 0; i < 11; i++)
            {
                if (!isdigit(value[i]))
                {
                    valid = false;
                    break;
                }
            }
        }
        if (!valid)
            cout << "INVALID PHONE! FORMAT MUST BE: 03xxxxxxxxx (11 digits)" << endl;
        else
            return value;
    }
}

string getValidPassword(string prompt)
{
    string value;
    while (true)
    {
        cout << prompt;
        getline(cin, value);
        if (value.size() < 6)
            cout << "PASSWORD TOO SHORT! MINIMUM 6 CHARACTERS." << endl;
        else
            return value;
    }
}

double getValidBalance(string prompt)
{
    double value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "INVALID INPUT! PLEASE ENTER A VALID NUMERIC AMOUNT." << endl;
        }
        else if (value < 0)
        {
            cin.ignore(1000, '\n');
            cout << "BALANCE CANNOT BE NEGATIVE! PLEASE ENTER A VALID AMOUNT (MINIMUM Rs. 0)." << endl;
        }
        else
        {
            cin.ignore(1000, '\n');
            return value;
        }
    }
}
