/*
 * Copyright (c) 1998 Softweyr LLC.  All rights reserved.
 *
 * strtok_r, from Berkeley strtok
 * Oct 13, 1998 by Wes Peters <wes@softweyr.com>
 *
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notices, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notices, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *
 *	This product includes software developed by Softweyr LLC, the
 *      University of California, Berkeley, and its contributors.
 *
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY SOFTWEYR LLC, THE REGENTS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL SOFTWEYR LLC, THE
 * REGENTS, OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//#include <stddef.h>
#include <string.h>


/*
    A sequence of calls to strtok() breaks the string pointed to by s1 into
    a sequence of tokens, each of which is delimited by a byte from the string
    pointed to by s2. The first call in the sequence has s1 as its first argument,
    and is followed by calls with a null pointer as their first argument.
    The separator string pointed to by s2 may be different from call to call.

    The first call in the sequence searches the string pointed to by s1 for the
    first byte that is not contained in the current separator string pointed to by s2.
    If no such byte is found, then there are no tokens in the string pointed to
    by s1 and strtok() shall return a null pointer. If such a byte is found,
    it is the start of the first token.

    The strtok() function then searches from there for a byte that is contained
    in the current separator string. If no such byte is found, the current token
    extends to the end of the string pointed to by s1, and subsequent searches
    for a token shall return a null pointer. If such a byte is found, it is
    overwritten by a null byte, which terminates the current token.
    The strtok() function saves a pointer to the following byte, from which the
    next search for a token shall start.

    Each subsequent call, with a null pointer as the value of the first argument,
    starts searching from the saved pointer and behaves as described above.

    The implementation shall behave as if no function defined in this volume
    of IEEE Std 1003.1-2001 calls strtok().

    The strtok() function need not be reentrant. A function that is not required
    to be reentrant is not required to be thread-safe.

    The strtok_r() function considers the null-terminated string s as a sequence
    of zero or more text tokens separated by spans of one or more characters from
    the separator string sep. The argument lasts points to a user-provided pointer
    which points to stored information necessary for strtok_r() to continue scanning
    the same string.

    In the first call to strtok_r(), s points to a null-terminated string, sep to
    a null-terminated string of separator characters, and the value pointed to by
    lasts is ignored. The strtok_r() function shall return a pointer to the first
    character of the first token, write a null character into s immediately
    following the returned token, and update the pointer to which lasts points.

    In subsequent calls, s is a NULL pointer and lasts shall be unchanged from the
    previous call so that subsequent calls shall move through the string s,
    returning successive tokens until no tokens remain. The separator string sep
    may be different from call to call. When no token remains in s, a NULL pointer
    shall be returned.

    Upon successful completion, strtok() shall return a pointer to the first byte
    of a token. Otherwise, if there is no token, strtok() shall return a null pointer.

    The strtok_r() function shall return a pointer to the token found, or a NULL
    pointer when no token is found.

*/

char * strtok_r(char *s, const char *delim, char **last)
{
    const char *spanp;
    int c, sc;
    char *tok;

    if (s == NULL && (s = *last) == NULL)
    {
        return NULL;
    }

    /*
     * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
cont:
    c = *s++;
    for (spanp = delim; (sc = *spanp++) != 0; )
    {
        if (c == sc)
        {
            goto cont;
        }
    }

    if (c == 0)		/* no non-delimiter characters */
    {
        *last = NULL;
        return NULL;
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for (;;)
    {
        c = *s++;
        spanp = delim;
        do
        {
            if ((sc = *spanp++) == c)
            {
                if (c == 0)
                {
                    s = NULL;
                }
                else
                {
                    char *w = s - 1;
                    *w = '\0';
                }
                *last = s;
                return tok;
            }
        }
        while (sc != 0);
    }
    /* NOTREACHED */
}


char * strtok(char *s, const char *delim)
{
    static char *last;

    return strtok_r(s, delim, &last);
}


#if defined(DEBUG_STRTOK)

/*
 * Test the tokenizer.
 */
int main()
{
    char test[80], blah[80];
    char *sep = "\\/:;=-";
    char *word, *phrase, *brkt, *brkb;

    printf("String tokenizer test:\n");

    strcpy(test, "This;is.a:test:of=the/string\\tokenizer-function.");

    for (word = strtok(test, sep);
            word;
            word = strtok(NULL, sep))
    {
        printf("Next word is \"%s\".\n", word);
    }

    phrase = "foo";

    strcpy(test, "This;is.a:test:of=the/string\\tokenizer-function.");

    for (word = strtok_r(test, sep, &brkt);
            word;
            word = strtok_r(NULL, sep, &brkt))
    {
        strcpy(blah, "blah:blat:blab:blag");

        for (phrase = strtok_r(blah, sep, &brkb);
                phrase;
                phrase = strtok_r(NULL, sep, &brkb))
        {
            printf("So far we're at %s:%s\n", word, phrase);
        }
    }

    return 0;
}

#endif /* DEBUG_STRTOK */

