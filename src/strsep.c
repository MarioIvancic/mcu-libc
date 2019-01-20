/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <string.h>

/*
    strsep - extract token from string

    char *strsep(char **stringp, const char *delim);

    If *stringp is NULL, the strsep() function returns NULL and does
    nothing else.  Otherwise, this function finds the first token in the
    string *stringp, where tokens are delimited by symbols in the string
    delim.  This token is terminated by overwriting the delimiter with a
    null byte ('\0') and *stringp is updated to point past the token.  In
    case no delimiter was found, the token is taken to be the entire
    string *stringp, and *stringp is made NULL.

    The strsep() function returns a pointer to the token, that is, it
    returns the original value of *stringp.

    This function is 4.4BSD.

    Note: The strsep() function was introduced as a replacement for strtok(3),
    since the latter cannot handle empty fields.  However, strtok(3)
    conforms to C89/C99 and hence is more portable.

    Be cautious when using this function.  If you do use it, note that:

    * This function modifies its first argument.

    * This function cannot be used on constant strings.

    * The identity of the delimiting character is lost.
*/


// from musl
#if 0
char *strsep(char **str, const char *sep)
{
	char *s = *str, *end;
	if (!s) return NULL;
	end = s + strcspn(s, sep);
	if (*end) *end++ = 0;
	else end = 0;
	*str = end;
	return s;
}
#endif

// from MSP-430-LibC
char * strsep(char **stringp, const char *delim)
{
    register char *s;
    register const char *spanp;
    register int c, sc;
    char *tok;

    if ((s = *stringp) == NULL) return (NULL);
    for (tok = s; ; )
    {
        c = *s++;
        spanp = delim;
        do
        {
            if ((sc = *spanp++) == c)
            {
                if (c == 0) s = NULL;
                else s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        }
        while (sc != 0);
    }
    /* NOTREACHED */
}

