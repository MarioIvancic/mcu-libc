#include <stdlib.h>

// from newlib

/*
    The rand() function shall compute a sequence of pseudo-random integers in the
    range [0, {RAND_MAX}] with a period of at least 232.

    The rand() function need not be reentrant. A function that is not required to
    be reentrant is not required to be thread-safe.

    The rand_r() function shall compute a sequence of pseudo-random integers in
    the range [0, {RAND_MAX}]. (The value of the {RAND_MAX} macro shall be at least
    32767.)

    If rand_r() is called with the same initial value for the object pointed to by
    seed and that object is not modified between successive returns and calls to
    rand_r(), the same sequence shall be generated.

    The srand() function uses the argument as a seed for a new sequence of
    pseudo-random numbers to be returned by subsequent calls to rand(). If srand()
    is then called with the same seed value, the sequence of pseudo-random numbers
    shall be repeated. If rand() is called before any calls to srand() are made,
    the same sequence shall be generated as when srand() is first called with a seed
    value of 1.

    The implementation shall behave as if no function defined in this volume of
    IEEE Std 1003.1-2001 calls rand() or srand().

    The rand() function shall return the next pseudo-random number in the sequence.

    The rand_r() function shall return a pseudo-random integer.

    The srand() function shall not return a value.
*/

/* Pseudo-random generator based on Minimal Standard by
   Lewis, Goodman, and Miller in 1969.

   I[j+1] = a*I[j] (mod m)

   where a = 16807
         m = 2147483647

   Using Schrage's algorithm, a*I[j] (mod m) can be rewritten as:

     a*(I[j] mod q) - r*{I[j]/q}      if >= 0
     a*(I[j] mod q) - r*{I[j]/q} + m  otherwise

   where: {} denotes integer division
          q = {m/a} = 127773
          r = m (mod a) = 2836

   note that the seed value of 0 cannot be used in the calculation as
   it results in 0 itself
*/

int rand_r(unsigned int *seed)
{
    long k;
    long s = (long)(*seed);
    if (s == 0) s = 0x12345987;
    k = s / 127773;
    s = 16807 * (s - k * 127773) - 2836 * k;
    if (s < 0) s += 2147483647;
    (*seed) = (unsigned int)s;
    return (int)(s & RAND_MAX);
}


static unsigned int seed = 1;

int rand(void)
{
    return rand_r(&seed);
}

void srand(unsigned int new_seed)
{
    seed = new_seed;
}
