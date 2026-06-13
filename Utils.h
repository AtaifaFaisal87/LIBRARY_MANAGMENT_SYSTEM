#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <ctime>
#include <cctype>
#include <cstring>
using namespace std;

// ============================================================
//                       RUNTIME LOGGER
// ============================================================

// Controls which runtime messages are printed during sensitive operations (e.g. file loading).
class RuntimeLogger
{
private:
    static bool suppressMessages;
public:
    static void enableSilentMode() { suppressMessages = true; }
    static void disableSilentMode() { suppressMessages = false; }
    static bool isSilent() { return suppressMessages; }
};

// ============================================================
//            REAL-TIME UTILITY FUNCTIONS
// ============================================================

// Gets current system time as time_t
time_t getCurrentTime();

// Converts time_t to human-readable string, or "N/A" if time is 0
string timeToString(time_t t);

// Converts 24-hour HH:MM to 12-hour format with AM/PM
string to12Hour(string timeStr);

// Converts date (DD/MM/YYYY) and time (HH:MM in 24-hour) to time_t
time_t reservationToTimeT(string date, string timeStr);

// Returns difference in minutes between two time_t values (end - start)
int minutesDiff(time_t start, time_t end);

// Adds given number of minutes to a time_t value and returns the new time_t
time_t addMinutes(time_t t, int minutes);

// ============================================================
// INPUT VALIDATION FUNCTIONS
// ============================================================

string getValidDate(string prompt);
string getValidTime(string prompt);
bool isValidReservationTime(string date, string start, string end);
int getValidInt(string prompt);
int getValidIntInRange(string prompt, int min, int max);
string getValidString(string prompt);
string getValidPublisher(string prompt);
string getValidISBN(string prompt);
string getValidAlphaString(string prompt);
string getValidCNIC(string prompt);
string getValidEmail(string prompt);
string getValidPhone(string prompt);
string getValidPassword(string prompt);
double getValidBalance(string prompt);

#endif // UTILS_H
