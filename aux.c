/*
 * File:  aux.c
 * Author:  Francisco Salgueiro
 * Description: Auxiliary functions
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "proj1.h"

/* returns index of airport or negative index-1 if it doesn't exist */
int get_airport(Global_State* global, char* id) {
	int f, l, m, cmp;
	f = 0;
	l = global->airports_count - 1;
	m = (f + l) / 2;
	while (f <= l) {
		cmp = strcmp(global->airports[m].id, id);
		if (cmp < 0)
			f = m + 1;
		else if (cmp == 0) {
			return m;
		} else
			l = m - 1;
		m = (f + l) / 2;
	}
	return -f - 1;
}

/* returns index of flight or -1 if it doesn't exist */
int get_flight(Global_State* global, char* id, Date date) {
	int i;
	for (i = 0; i < global->flights_count; i++) {
		if (strcmp(global->flights[i].id, id) == 0 &&
			compare_dates(global->flights->departure_date, date) == 0) {
			return i;
		}
	}
	return -1;
}

int get_num_flights(Global_State* global, char* id) {
	int i, count;
	count = 0;
	for (i = 0; i < global->flights_count; i++) {
		if (strcmp(global->flights[i].departure.id, id) == 0) {
			count++;
		}
	}
	return count;
}

int check_flight_id(char* id) {
	size_t i, length_id;
	length_id = strlen(id);
	if (length_id < 3 || length_id > 6 || !isupper(id[0]) || !isupper(id[1])) {
		return 1;
	}
	for (i = 2; i < length_id; i++) {
		if (!isdigit(id[i])) {
			return 1;
		}
	}
	return 0;
}

/*----------------------
 |  INTIALIAZING FUNTIONS
 -----------------------*/

void init_time(Time* time, int hours, int minutes) {
	time->hours = hours;
	time->minutes = minutes;
}
void init_date(Date* date, int day, int month, int year) {
	date->day = day;
	date->month = month;
	date->year = year;
}
void init_flight(Flight* flight, char* id, Airport departure, Airport arrival,
				 Date departure_date, Time departure_time, Time duration,
				 Date arrival_date, Time arrival_time, int capacity) {
	strcpy(flight->id, id);
	flight->departure = departure;
	flight->arrival = arrival;
	flight->departure_date = departure_date;
	flight->arrival_date = arrival_date;
	flight->departure_time = departure_time;
	flight->arrival_time = arrival_time;
	flight->duration = duration;
	flight->capacity = capacity;
}

void init_airport(Airport* airport, char* id, char* country, char* city) {
	strcpy(airport->id, id);
	strcpy(airport->country, country);
	strcpy(airport->city, city);
}

/*----------------------
 |  COMPARISON FUNTIONS
 -----------------------*/

/* Returns -1 if date1 happens before date2, 0 if the dates are equal and 1 if
 * date1 happens after date2 */
int compare_dates(Date date1, Date date2) {
	if (date1.year < date2.year) {
		return -1;
	}
	if (date1.year > date2.year) {
		return 1;
	}
	if (date1.month < date2.month) {
		return -1;
	}
	if (date1.month > date2.month) {
		return 1;
	}
	if (date1.day < date2.day) {
		return -1;
	}
	if (date1.day > date2.day) {
		return 1;
	}
	return 0;
}

/* Returns -1 if time1 happens before time2, 0 if the dates are equal and 1 if
 * time1 happens after time2 */
int compare_time(Time time1, Time time2) {
	if (time1.hours < time2.hours) {
		return -1;
	}
	if (time1.hours > time2.hours) {
		return 1;
	}
	if (time1.minutes < time2.minutes) {
		return -1;
	}
	if (time1.minutes > time2.minutes) {
		return 1;
	}
	return 0;
}

int compare_date_and_time(Date date1, Date date2, Time time1, Time time2) {
	int d, t;
	if ((d = compare_dates(date1, date2)) < 0) {
		return -1;
	}
	if (d == 0) {
		if ((t = compare_time(time1, time2)) < 0) {
			return -1;
		} else if (t == 0) {
			return 0;
		}
	}
	return 1;
}

/*----------------------
 |  DATE AND TIME FUNTIONS
 -----------------------*/

/* Adds one day to date */
Date increment_date(Date date) {
	Date result;
	result.day = date.day + 1;
	result.month = date.month;
	result.year = date.year;
	if (result.day > DAYS_BY_MONTH[result.month - 1]) {
		result.month++;
		result.day = 1;
		if (result.month > 12) {
			result.year++;
			result.month = 1;
		}
	}
	return result;
}

/* Sums two times */
Time sum_time(Time time1, Time time2) {
	Time result;
	result.hours = time1.hours + time2.hours;
	result.minutes = time1.minutes + time2.minutes;
	if (result.minutes >= 60) {
		result.hours++;
		result.minutes -= 60;
	}
	return result;
}

/*----------------------
 |  READ FUNTIONS
 -----------------------*/

void read_date(Date* date) {
	int year, month, day;

	scanf(DATE_FORMAT, &day, &month, &year);
	init_date(date, day, month, year);
}

void read_time(Time* time) {
	int minutes, hours;

	scanf(TIME_FORMAT, &minutes, &hours);
	init_time(time, minutes, hours);
}

/*----------------------
 |  PRINT FUNTIONS
 -----------------------*/

/* Prints date in formatted form */
void print_date(Date date) {
	printf("%02d-%02d-%04d", date.day, date.month, date.year);
}

/* Prints time in formatted form */
void print_time(Time time) { printf("%02d:%02d", time.hours, time.minutes); }
