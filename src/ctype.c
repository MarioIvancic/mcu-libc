#include <ctype.h>

// from musl

/*
    Check if character is alphanumeric.
    0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
*/
int isalnum(int c)
{
	return isalpha(c) || isdigit(c);
}

/*
    Check if character is alphabetic.
    ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
*/
int isalpha(int c)
{
	return ((unsigned)c|32)-'a' < 26;
}
// int isalpha(int c) { return ((c>='A'&&c<='Z')||(c>='a'&&c<='z')); }


/*
    Check if character is blank.
    ' ' and '\t'
*/
int isblank(int c)
{
	return (c == ' ' || c == '\t');
}

/*
    Check if character is a control character.
    0x00 .. 0x1f, 0x7f
*/
int iscntrl(int c)
{
	return (unsigned)c < 0x20 || c == 0x7f;
}


/*
    Check if character is decimal digit.
    0123456789
*/
int isdigit(int c)
{
	return (unsigned)c-'0' < 10;
}


/*
    Check if character has graphical representation.
    !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ
    [\]^_`abcdefghijklmnopqrstuvwxyz{|}~
    0x21 .. 0x7e
*/
int isgraph(int c)
{
	return (unsigned)c-0x21 < 0x5e;
}


/*
    Check if character is lowercase letter.
    abcdefghijklmnopqrstuvwxyz
*/
int islower(int c)
{
	return (unsigned)c-'a' < 26;
}


/*
    Check if character is printable.
    ' ' and !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ
    [\]^_`abcdefghijklmnopqrstuvwxyz{|}~
    0x20 .. 0x7e
*/
int isprint(int c)
{
	return (unsigned)c-0x20 < 0x5f;
}


/*
    Check if character is a punctuation character.
    !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
*/
int ispunct(int c)
{
	return isgraph(c) && !isalnum(c);
}


/*
    Check if character is a white-space.
    ' ', '\t', '\v', '\f', '\r', '\n'
*/
int isspace(int c)
{
	return c == ' ' || (unsigned)c-'\t' < 5;
}


/*
    Check if character is uppercase letter.
    ABCDEFGHIJKLMNOPQRSTUVWXYZ
*/
int isupper(int c)
{
	return (unsigned)c-'A' < 26;
}


/*
    Check if character is hexadecimal digit.
    0123456789abcdefABCDEF
*/
int isxdigit(int c)
{
	return isdigit(c) || ((unsigned)c|32)-'a' < 6;
}


// Convert uppercase letter to lowercase
int tolower(int c)
{
	// if (isupper(c)) return c | 32;
	if ((unsigned)c-'A' < 26) return c | 32;
	return c;
}


// Convert lowercase letter to uppercase
int toupper(int c)
{
	// if (islower(c)) return c & 0x5f;
	if ((unsigned)c-'a' < 26) return c & 0x5f;
	return c;
}

