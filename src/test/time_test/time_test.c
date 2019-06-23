#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ALL MCU-LIBC files and symbols are prefixed by MY_
#include "MY_time.h"


void print_tm(struct tm* tmp, char* str)
{
    if(str)
    {
        sprintf(str, "TM: %04u-%02u-%02u %02u:%02u:%02u %03u %u",
          tmp->tm_year + 1900,
          tmp->tm_mon + 1,
          tmp->tm_mday,
          tmp->tm_hour,
          tmp->tm_min,
          tmp->tm_sec,
          tmp->tm_yday,
          tmp->tm_wday);
    }
    else
    {
        printf("TM: %04u-%02u-%02u %02u:%02u:%02u %03u %u\n",
          tmp->tm_year + 1900,
          tmp->tm_mon + 1,
          tmp->tm_mday,
          tmp->tm_hour,
          tmp->tm_min,
          tmp->tm_sec,
          tmp->tm_yday,
          tmp->tm_wday);
    }
}

void print_mytm(struct MY_tm* tmp, char* str)
{
    if(str)
    {
        sprintf(str, "MY: %04u-%02u-%02u %02u:%02u:%02u %03u %u",
          tmp->tm_year + 1900,
          tmp->tm_mon + 1,
          tmp->tm_mday,
          tmp->tm_hour,
          tmp->tm_min,
          tmp->tm_sec,
          tmp->tm_yday,
          tmp->tm_wday);
    }
    else
    {
        printf("MY: %04u-%02u-%02u %02u:%02u:%02u %03u %u\n",
          tmp->tm_year + 1900,
          tmp->tm_mon + 1,
          tmp->tm_mday,
          tmp->tm_hour,
          tmp->tm_min,
          tmp->tm_sec,
          tmp->tm_yday,
          tmp->tm_wday);
    }
}


int mktime_test1(MY_time_t mynow, int force_print)
{
    struct MY_tm* mtm = MY_gmtime(&mynow);
    struct tm stm;
    char mystr[32];
    char* stat = "OK";
    int err = 0;

    /*
        mktime ignores tm_wday and tm_yday fields on input.
        mktime normalize all values and sets tm_wday and tm_yday
        according to other fields
    */
    stm.tm_hour = mtm->tm_hour;
    stm.tm_isdst = mtm->tm_isdst;
    stm.tm_mday = mtm->tm_mday;
    stm.tm_min = mtm->tm_min;
    stm.tm_mon = mtm->tm_mon;
    stm.tm_sec = mtm->tm_sec;
    stm.tm_wday = -1;
    stm.tm_yday = -1;
    stm.tm_year = mtm->tm_year;

    print_mytm(mtm, mystr);

    mtm->tm_wday = -1;
    mtm->tm_yday = -1;

    time_t st = mktime(&stm);
    MY_time_t myt = MY_mktime(mtm);

    if((time_t) myt != st) err = 1;

    if(err) stat = "##";

    if(err || force_print)
    {
        printf("MYTIME %u, SYSTIME %u %s for GMTIME %s\n", myt, (unsigned) st, stat, mystr + 3);
    }
    return err;
}


// delete all space \t, \r, \n characters in string str in-place
// string str will be the same or shorter than it was
static void delete_space(char* dst, const char* src)
{
    const char *p1;
    char *p2;
    char c;

    p1 = src;
    p2 = dst;

    while((c = *p1++) != 0)
    {
        if(c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;
        *p2++ = c;
    }
    *p2 = 0;
}


int asctime_test1(MY_time_t mynow, int force_print)
{
    struct MY_tm* mtm = MY_gmtime(&mynow);
    struct tm stm;
    char mystr[32];
    char str1[32];
    char str2[32];
    char* stat = "OK";
    char *myasct, *sysasct;
    int err = 0;

    /*
        asctime returns pointer to string like this one
        "Wed Jun 30 21:49:08 1993\n"
    */
    stm.tm_hour = mtm->tm_hour;
    stm.tm_isdst = mtm->tm_isdst;
    stm.tm_mday = mtm->tm_mday;
    stm.tm_min = mtm->tm_min;
    stm.tm_mon = mtm->tm_mon;
    stm.tm_sec = mtm->tm_sec;
    stm.tm_wday = mtm->tm_wday;
    stm.tm_yday = mtm->tm_yday;
    stm.tm_year = mtm->tm_year;

    print_mytm(mtm, mystr);

    sysasct = asctime(&stm);
    myasct = MY_asctime(mtm);

    // since some implementations have different rules for alignement of fields
    // we have to delete all spece, \r, \n chars in the strings
    delete_space(str1, sysasct);
    delete_space(str2, myasct);

    err = strcmp(str1, str2);
    if(err) stat = "##";

    if(err || force_print)
    {
        printf("MY '%s', SYS '%s' %s for GMTIME %s\n", myasct, sysasct, stat, mystr + 3);
    }
    return err;
}



int gmtime_test1(MY_time_t mynow, int force_print)
{
    time_t now = mynow;
    char mystr[32];
    char tmstr[32];
    char* stat = "OK";
    int err = 0;

    struct MY_tm* mtm = MY_gmtime(&mynow);
    struct tm* stm = gmtime(&now);

    print_tm(stm, tmstr);
    print_mytm(mtm, mystr);

    err = strcmp(tmstr + 3, mystr + 3);
    if(err) stat = "##";

    if(err || force_print)
    {
        printf("TIME %s: %u, %s, %s\n", stat, mynow, tmstr, mystr);
    }
    return err;
}



int localtime_test1(MY_time_t mynow, int force_print)
{
    time_t now = mynow;
    char mystr[32];
    char tmstr[32];
    char* stat = "OK";
    int err = 0;

    struct MY_tm* mtm = MY_localtime(&mynow);
    struct tm* stm = localtime(&now);

    print_tm(stm, tmstr);
    print_mytm(mtm, mystr);

    err = strcmp(tmstr + 3, mystr + 3);
    if(err) stat = "##";

    if(err || force_print)
    {
        printf("TIME %s: %u, %s, %s\n", stat, mynow, tmstr, mystr);
    }
    return err;
}



void gmtime_test(void)
{
    printf("testing GMTIME\n");
    MY_time_t mynow;
    int i;

    // 300000 iteration from year 1974 to 2034
    for(i = 1, mynow = 126230400; mynow < 2019686400; mynow += 6311, i++)
    {
        if(gmtime_test1(mynow, 0))
        {
            printf("Test %d failed!\n", i);
            return;
        }
    }

    // 320000 iteration from year 1984 to 1985
    for(mynow = 441763200; mynow < 441763200 + 32000000; mynow += 99, i++)
    {
        if(gmtime_test1(mynow, 0))
        {
            printf("Test %d failed!\n", i);
            return;
        }
    }

    printf("OK, tested %d times\n", i);
}



void localtime_test(void)
{
    printf("testing LOCALTIME\n");
    MY_time_t mynow;
    int i;

    // 180000 iteration from year 1996 to 2034
    for(i = 1, mynow = 820450800; mynow < 2019686400; mynow += 6311, i++)
    {
        if(localtime_test1(mynow, 0))
        {
            printf("Test %d failed!\n", i);
            return;
        }
    }

    // 320000 iteration from year 2000 to 2001
    for(mynow = 946681200; mynow < 978303600; mynow += 99, i++)
    {
        if(localtime_test1(mynow, 0))
        {
            printf("Test %d failed!\n", i);
            return;
        }
    }

    printf("OK, tested %d times\n", i);
}



void mktime_test(void)
{
    printf("testing MKTIME\n");
    MY_time_t mynow;
    int i;

    // 300000 iteration from year 1974 to 2034
    for(i = 1, mynow = 126230400; mynow < 2019686400; mynow += 6311, i++)
    {
        if(mktime_test1(mynow, 0))
        {
            printf("Test %d failed!\n", i);
            return;
        }
    }

    // 320000 iteration from year 1984 to 1985
    for(mynow = 441763200; mynow < 441763200 + 32000000; mynow += 99, i++)
    {
        if(mktime_test1(mynow, 0))
        {
            printf("Test %d failed!\n", i);
            return;
        }
    }

    printf("OK, tested %d times\n", i);
}


void asctime_test(void)
{
    printf("testing ASCTIME\n");
    MY_time_t mynow;
    int i;

    // 300000 iteration from year 1974 to 2034
    for(i = 1, mynow = 126230400; mynow < 2019686400; mynow += 6311, i++)
    {
        if(asctime_test1(mynow, 0))
        {
            printf("Test %d failed!\n", i);
            return;
        }
    }

    // 320000 iteration from year 1984 to 1985
    for(mynow = 441763200; mynow < 441763200 + 32000000; mynow += 99, i++)
    {
        if(asctime_test1(mynow, 0))
        {
            printf("Test %d failed!\n", i);
            return;
        }
    }

    printf("OK, tested %d times\n", i);
}



int main()
{
    MY_time_t my;

    // CET timezone
    MY__time_set_timezone(1, 1, 1, 89, 1, 303, 1);
    setenv("TZ", ":Europe/Sarajevo", 1);

    //for(my = 133927150; my < 133927150 + 40000; my += 8899) print_time(my);
    //for(my = 214969371 - 2 * 6311; my < 214969371 + 2 * 6311; my += 99) localtime_test1(my, 1);
    //     for(my = 214966649; my < 214966960; my += 99) localtime_test1(my, 1);

    // 2018-10-27 20:23:15 CEST (GMT+02:00) to 2018-10-28 20:23:15 CET (GMT+01:00)
    //for(my = 1540664595; my < 1540754595; my += 923) localtime_test1(my, 1);

    // 2018-03-24 20:23:15 CET (GMT+01:00) to 2018-13-25 20:23:15 CEST (GMT+02:00)
    //for(my = 1521919395; my < 1522002195; my += 1743) localtime_test1(my, 1);

    // for test
    //for(my = 1521935000; my < 1521939900; my += 2443) localtime_test1(my, 1);

    //mktime_test1(214966649, 1);
    //gmtime_test1(133929820);
    //localtime_test1(465356429, 1);
    //asctime_test1(214969371, 1);

    //gmtime_test();    // OK
    //localtime_test();
    //mktime_test();    // OK
    asctime_test();   // OK



    return 0;
}
