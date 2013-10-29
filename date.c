#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "date.h"

struct date {
    char *year;
    char *month;
    char *day;
};

/*
 * date_create creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
Date *date_create(char *datestr)
{
    Date *d;
    d = (Date *)malloc(sizeof(Date));
    if (d == NULL)
        return NULL;
    char *day = (char *)malloc(3*sizeof(char));
    char *month = (char *)malloc(3*sizeof(char));
    char *year = (char *)malloc(5*sizeof(char));
    int size = 0;
    char *str = datestr;
    while ( *str++ != '\0')
        size++;
    if (size < 10 || !isdigit(datestr[0]) || !isdigit(datestr[1]) || datestr[2] != '/'
        || !isdigit(datestr[3]) || !isdigit(datestr[4]) || datestr[5] != '/'
        || !isdigit(datestr[6]) || !isdigit(datestr[7]) || !isdigit(datestr[8]) || !isdigit(datestr[9]))
        return NULL;
    memcpy(day, datestr, 2 );
    day[2] = '\0';
    memcpy( month, datestr + 3, 2 );
    month[2] = '\0';
    memcpy( year, datestr + 6, 4 );
    year[4] = '\0';
    d->day = day;
    d->month = month;
    d->year = year;
    return d;
}

/*
 * date_duplicate creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
Date *date_duplicate(Date *d)
{
    Date *ddup;
    ddup = (Date *)malloc(sizeof(Date));
    if (ddup == NULL)
        return NULL;
    ddup->day = d->day;
    ddup->month = d->month;
    ddup->year = d->year;
    return ddup;
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2)
{
    int diff = atoi(date1->year) - atoi(date2->year);
    if (diff != 0)
        return diff;
    diff = atoi(date1->month) - atoi(date2->month);
    if (diff != 0)
        return diff;
    diff = atoi(date1->day) - atoi(date2->day);
    if (diff != 0)
        return diff;
    return 0;
}

/*
 * date_destroy returns any storage associated with `d' to the system
 */
void date_destroy(Date *d)
{
    free(d->day);
    free(d->month);
    free(d->year);
    free(d);
}

