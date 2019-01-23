// gmtime.c

#include <time.h>
#include "_time_data.h"

/*
 * TODO: in speed optimized case half of the code comes from _time_year_start
 * array. We can reduce the size of the array and for years not in the array
 * we can use size optimized code.
 * TODO: in speed optimized code try to calculate year and month directly
 * using modulo and division.
 * */


struct tm _time_last_time;


#if !defined(GMTIME_OPTIMIZE_SIZE) && !defined(GMTIME_OPTIMIZE_SPEED)
#define GMTIME_OPTIMIZE_SIZE	1
#endif

// this option is only relevant if GMTIME_OPTIMIZE_SPEED is defined
//#define GMTIME_LOOP_DAYS


// #####################################################################

#ifdef GMTIME_OPTIMIZE_SIZE

/*
	using:
	* struct tm _time_last_time,
	* const unsigned char _time_month_days[];
	Performace:
	* 10.32uS, 336 bytes @ 60MHz, ARM code, O2 optimization
	* 44.32uS, 208 bytes @ 60MHz, Thumb code, Os optimization
*/

// convert UTC calendar time time_t to broken time struct tm
struct tm *gmtime_r (const time_t *__restrict timep, struct tm *__restrict tmp)
{
	time_t epoch = *timep;
	unsigned int year;
	unsigned char month;
	unsigned int days;

	tmp->tm_sec = epoch % 60;

	epoch /= 60; // now it is minutes

	tmp->tm_min = epoch % 60;

	epoch /= 60; // now it is hours

	tmp->tm_hour = epoch % 24;

	epoch /= 24; // now it is days

	tmp->tm_wday = (epoch + 4) % 7;

	year = 1970;
	days = 0;

	while((days += (LEAP_YEAR(year) ? 366 : 365)) <= epoch) year++;

	tmp->tm_year = year - 1900;

	days -= LEAP_YEAR(year) ? 366 : 365;

	epoch -= days; // now it is days in this year, starting at 0

	tmp->tm_yday = epoch;

	days = epoch;

	for(month = 0; month < 12; month++)
	{
		unsigned char monthLength = _time_month_days[LEAP_YEAR(year)][month];

    	if(days >= monthLength) days -= monthLength;
		else break;
	}
	tmp->tm_mon = month;
	tmp->tm_mday = days + 1;

	tmp->tm_isdst = 0;

	return &_time_last_time;
}

#else // ! GMTIME_OPTIMIZE_SIZE

// #####################################################################

#ifdef GMTIME_OPTIMIZE_SPEED

/*
	using:
	* struct tm _time_last_time,
	* const unsigned char _time_month_days[],
	* const time_t _time_year_start[],
	performace:
	* 5.48uS, 1056 bytes @ 60MHz, ARM code, O2 optimization
*/

#if defined(__TIME_UNSIGNED)

#define TIME_LOWEST_YEAR 1970

const time_t _time_year_start[] =
{
    0,          /* 1970 */    31536000,   /* 1971 */    63072000,   /* 1972 */    94694400,   /* 1973 */
    126230400,  /* 1974 */    157766400,  /* 1975 */    189302400,  /* 1976 */    220924800,  /* 1977 */
    252460800,  /* 1978 */    283996800,  /* 1979 */    315532800,  /* 1980 */    347155200,  /* 1981 */
    378691200,  /* 1982 */    410227200,  /* 1983 */    441763200,  /* 1984 */    473385600,  /* 1985 */
    504921600,  /* 1986 */    536457600,  /* 1987 */    567993600,  /* 1988 */    599616000,  /* 1989 */
    631152000,  /* 1990 */    662688000,  /* 1991 */    694224000,  /* 1992 */    725846400,  /* 1993 */
    757382400,  /* 1994 */    788918400,  /* 1995 */    820454400,  /* 1996 */    852076800,  /* 1997 */
    883612800,  /* 1998 */    915148800,  /* 1999 */    946684800,  /* 2000 */    978307200,  /* 2001 */
    1009843200, /* 2002 */    1041379200, /* 2003 */    1072915200, /* 2004 */    1104537600, /* 2005 */
    1136073600, /* 2006 */    1167609600, /* 2007 */    1199145600, /* 2008 */    1230768000, /* 2009 */
    1262304000, /* 2010 */    1293840000, /* 2011 */    1325376000, /* 2012 */    1356998400, /* 2013 */
    1388534400, /* 2014 */    1420070400, /* 2015 */    1451606400, /* 2016 */    1483228800, /* 2017 */
    1514764800, /* 2018 */    1546300800, /* 2019 */    1577836800, /* 2020 */    1609459200, /* 2021 */
    1640995200, /* 2022 */    1672531200, /* 2023 */    1704067200, /* 2024 */    1735689600, /* 2025 */
    1767225600, /* 2026 */    1798761600, /* 2027 */    1830297600, /* 2028 */    1861920000, /* 2029 */
    1893456000, /* 2030 */    1924992000, /* 2031 */    1956528000, /* 2032 */    1988150400, /* 2033 */
    2019686400, /* 2034 */    2051222400, /* 2035 */    2082758400, /* 2036 */    2114380800, /* 2037 */
    2145916800, /* 2038 */    2177452800, /* 2039 */    2208988800, /* 2040 */    2240611200, /* 2041 */
    2272147200, /* 2042 */    2303683200, /* 2043 */    2335219200, /* 2044 */    2366841600, /* 2045 */
    2398377600, /* 2046 */    2429913600, /* 2047 */    2461449600, /* 2048 */    2493072000, /* 2049 */
    2524608000, /* 2050 */    2556144000, /* 2051 */    2587680000, /* 2052 */    2619302400, /* 2053 */
    2650838400, /* 2054 */    2682374400, /* 2055 */    2713910400, /* 2056 */    2745532800, /* 2057 */
    2777068800, /* 2058 */    2808604800, /* 2059 */    2840140800, /* 2060 */    2871763200, /* 2061 */
    2903299200, /* 2062 */    2934835200, /* 2063 */    2966371200, /* 2064 */    2997993600, /* 2065 */
    3029529600, /* 2066 */    3061065600, /* 2067 */    3092601600, /* 2068 */    3124224000, /* 2069 */
    3155760000, /* 2070 */    3187296000, /* 2071 */    3218832000, /* 2072 */    3250454400, /* 2073 */
    3281990400, /* 2074 */    3313526400, /* 2075 */    3345062400, /* 2076 */    3376684800, /* 2077 */
    3408220800, /* 2078 */    3439756800, /* 2079 */    3471292800, /* 2080 */    3502915200, /* 2081 */
    3534451200, /* 2082 */    3565987200, /* 2083 */    3597523200, /* 2084 */    3629145600, /* 2085 */
    3660681600, /* 2086 */    3692217600, /* 2087 */    3723753600, /* 2088 */    3755376000, /* 2089 */
    3786912000, /* 2090 */    3818448000, /* 2091 */    3849984000, /* 2092 */    3881606400, /* 2093 */
    3913142400, /* 2094 */    3944678400, /* 2095 */    3976214400, /* 2096 */    4007836800, /* 2097 */
    4039372800, /* 2098 */    4070908800, /* 2099 */    4102444800, /* 2100 */    4133980800, /* 2101 */
    4165516800, /* 2102 */    4197052800, /* 2103 */    4228588800, /* 2104 */    4260211200, /* 2105 */
    4291747200, /* 2106 */
};

#else // ! __TIME_UNSIGNED

#define TIME_LOWEST_YEAR 1903

const time_t _time_year_start[] =
{
    -2114380800, /* 1903 */    -2082844800, /* 1904 */    -2051222400, /* 1905 */    -2019686400, /* 1906 */
    -1988150400, /* 1907 */    -1956614400, /* 1908 */    -1924992000, /* 1909 */    -1893456000, /* 1910 */
    -1861920000, /* 1911 */    -1830384000, /* 1912 */    -1798761600, /* 1913 */    -1767225600, /* 1914 */
    -1735689600, /* 1915 */    -1704153600, /* 1916 */    -1672531200, /* 1917 */    -1640995200, /* 1918 */
    -1609459200, /* 1919 */    -1577923200, /* 1920 */    -1546300800, /* 1921 */    -1514764800, /* 1922 */
    -1483228800, /* 1923 */    -1451692800, /* 1924 */    -1420070400, /* 1925 */    -1388534400, /* 1926 */
    -1356998400, /* 1927 */    -1325462400, /* 1928 */    -1293840000, /* 1929 */    -1262304000, /* 1930 */
    -1230768000, /* 1931 */    -1199232000, /* 1932 */    -1167609600, /* 1933 */    -1136073600, /* 1934 */
    -1104537600, /* 1935 */    -1073001600, /* 1936 */    -1041379200, /* 1937 */    -1009843200, /* 1938 */
    -978307200,  /* 1939 */    -946771200,  /* 1940 */    -915148800,  /* 1941 */    -883612800,  /* 1942 */
    -852076800,  /* 1943 */    -820540800,  /* 1944 */    -788918400,  /* 1945 */    -757382400,  /* 1946 */
    -725846400,  /* 1947 */    -694310400,  /* 1948 */    -662688000,  /* 1949 */    -631152000,  /* 1950 */
    -599616000,  /* 1951 */    -568080000,  /* 1952 */    -536457600,  /* 1953 */    -504921600,  /* 1954 */
    -473385600,  /* 1955 */    -441849600,  /* 1956 */    -410227200,  /* 1957 */    -378691200,  /* 1958 */
    -347155200,  /* 1959 */    -315619200,  /* 1960 */    -283996800,  /* 1961 */    -252460800,  /* 1962 */
    -220924800,  /* 1963 */    -189388800,  /* 1964 */    -157766400,  /* 1965 */    -126230400,  /* 1966 */
    -94694400,   /* 1967 */    -63158400,   /* 1968 */    -31536000,   /* 1969 */    0,           /* 1970 */
    31536000,    /* 1971 */    63072000,    /* 1972 */    94694400,    /* 1973 */    126230400,   /* 1974 */
    157766400,   /* 1975 */    189302400,   /* 1976 */    220924800,   /* 1977 */    252460800,   /* 1978 */
    283996800,   /* 1979 */    315532800,   /* 1980 */    347155200,   /* 1981 */    378691200,   /* 1982 */
    410227200,   /* 1983 */    441763200,   /* 1984 */    473385600,   /* 1985 */    504921600,   /* 1986 */
    536457600,   /* 1987 */    567993600,   /* 1988 */    599616000,   /* 1989 */    631152000,   /* 1990 */
    662688000,   /* 1991 */    694224000,   /* 1992 */    725846400,   /* 1993 */    757382400,   /* 1994 */
    788918400,   /* 1995 */    820454400,   /* 1996 */    852076800,   /* 1997 */    883612800,   /* 1998 */
    915148800,   /* 1999 */    946684800,   /* 2000 */    978307200,   /* 2001 */    1009843200,  /* 2002 */
    1041379200,  /* 2003 */    1072915200,  /* 2004 */    1104537600,  /* 2005 */    1136073600,  /* 2006 */
    1167609600,  /* 2007 */    1199145600,  /* 2008 */    1230768000,  /* 2009 */    1262304000,  /* 2010 */
    1293840000,  /* 2011 */    1325376000,  /* 2012 */    1356998400,  /* 2013 */    1388534400,  /* 2014 */
    1420070400,  /* 2015 */    1451606400,  /* 2016 */    1483228800,  /* 2017 */    1514764800,  /* 2018 */
    1546300800,  /* 2019 */    1577836800,  /* 2020 */    1609459200,  /* 2021 */    1640995200,  /* 2022 */
    1672531200,  /* 2023 */    1704067200,  /* 2024 */    1735689600,  /* 2025 */    1767225600,  /* 2026 */
    1798761600,  /* 2027 */    1830297600,  /* 2028 */    1861920000,  /* 2029 */    1893456000,  /* 2030 */
    1924992000,  /* 2031 */    1956528000,  /* 2032 */    1988150400,  /* 2033 */    2019686400,  /* 2034 */
    2051222400,  /* 2035 */    2082758400,  /* 2036 */    2114380800,  /* 2037 */    2145916800,  /* 2038 */
};
#endif // __TIME_UNSIGNED



// find index of year in _time_year_start in which t belongs
static int find_year(time_t t)
{
    // binary search
    int high, i, low;

    low = -1;
    high = (sizeof(_time_year_start) / sizeof(_time_year_start[0])) - 1;

    while(high - low > 1)
	{
    	i = low + (high - low) / 2;
        if(t < _time_year_start[i]) 	high = i;
        else                            low  = i;
	}

    if(t >= _time_year_start[low]) 	return low;
    else                         	return -1;
}



// convert UTC calendar time time_t to broken time struct tm
struct tm *gmtime_r (const time_t *__restrict timep, struct tm *__restrict tmp)
{

	time_t epoch = *timep;
	int year;
	unsigned int ydays;
	unsigned char month;

	// find index of the year in _time_year_start array
	year = find_year(epoch);
	if(year != -1)
	{
	    unsigned char days;

	    tmp->tm_wday = ((epoch / (24UL * 60UL * 60UL)) + 4) % 7;

	    epoch -= _time_year_start[year];

	    // add offset to get real year
	    year += TIME_LOWEST_YEAR;

	    tmp->tm_year = year - 1900;

        // now in epoch we have seconds from the start of the year
	    ydays = 0;
	    month = 0;
	    days = 0;
        while( (month < 12 ) && (epoch >= (24UL * 60UL * 60UL)) )
        {
            unsigned char monthLength = _time_month_days[LEAP_YEAR(year)][month];
#ifdef GMTIME_LOOP_DAYS
            days = 0;
            while(days < monthLength)
            {
                if(epoch >= (24UL * 60UL * 60UL))
                {
                    epoch -= (24UL * 60UL * 60UL);
                    days++;
                }
                else break;
            }
            ydays += days;
            if(days >= monthLength)
            {
                days -= monthLength;
                month++;
            }

            // if(epoch < (24UL * 60UL * 60UL)) break;

#else // ! GMTIME_LOOP_DAYS
            if(epoch >= (monthLength * 24UL * 60UL * 60UL))
            {
                epoch -= (monthLength * 24UL * 60UL * 60UL);
                ydays += monthLength;
                month++;
                days = 0;
            }
            else
            {
                days = epoch / (24UL * 60UL * 60UL);
                epoch = epoch % (24UL * 60UL * 60UL);
                ydays += days;
            }

            // if(epoch < (24UL * 60UL * 60UL)) break;
#endif // GMTIME_LOOP_DAYS
        }

        tmp->tm_mon = month;

        tmp->tm_yday = ydays;

        tmp->tm_mday = days + 1;

        tmp->tm_hour = epoch / (60UL * 60UL);

        epoch -= tmp->tm_hour * 60UL * 60UL;

        tmp->tm_min = epoch / 60UL;

        epoch -= tmp->tm_min * 60UL;

        tmp->tm_sec = epoch;

        tmp->tm_isdst = 0;

        return &_time_last_time;
	}
	return 0;
}


#else // ! GMTIME_OPTIMIZE_SPEED

// #####################################################################

#error "gmtime is not implemented!"
#error "Define one of GMTIME_OPTIMIZE_SIZE, GMTIME_OPTIMIZE_SPEED"

#endif // GMTIME_OPTIMIZE_SPEED

#endif // GMTIME_OPTIMIZE_SIZE


struct tm *gmtime (const time_t *timep)
{
    return gmtime_r(timep, &_time_last_time);
}


