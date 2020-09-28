#include <stdlib.h>


/*
    The abs() function shall compute the absolute value of its integer operand, i.
    If the result cannot be represented, the behavior is undefined.

    The abs() function shall return the absolute value of its integer operand.
*/


int abs(int x)
{
    return (x < 0) ? -x : x;
}


long labs(long x)
{
    return (x < 0) ? -x : x;
}


long long llabs(long long x)
{
    return (x < 0) ? -x : x;
}

