#include "Fines.h"
#include "User.h"

Fines::Fines(int u, int b, int mins, User *user)
    : user_id(u), book_id(b)
{
    minuteslate = (mins < 0) ? 0 : mins;
    rate = user->getFineRate();
    originalAmount = rate * minuteslate;

    if (user->hasMembership())
    {
        finalAmount = user->applyMembershipDiscount(originalAmount);
        discountApplied = true;
    }
    else
    {
        finalAmount = originalAmount;
        discountApplied = false;
    }
}
