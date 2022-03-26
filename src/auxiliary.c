/*
 * File:  aux.c
 * Author:  Francisco Salgueiro
 * Description: Auxiliary functions
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "proj1.h"

/* returns index of airport or -index-1 if it doesn't exist */
int get_airport(char id[]) {
	int f, l, m, cmp;
	f = 0;
	l = airports_count - 1;
	m = (f + l) / 2;
	while (f <= l) {
		cmp = strcmp(sorted_airports[m]->id, id);
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

int get_flight(char id[], Date* date) {
	int i;
	for (i = 0; i < flights_count; i++) {
		if (strcmp(flights[i].id, id) == 0 &&
			compare_dates(&flights->departure_date, date) == 0) {
			return i;
		}
	}
	return -1;
}

int get_num_flights(char* id) {
	int i, count;
	count = 0;
	for (i = 0; i < flights_count; i++) {
		if (strcmp(flights[i].departure->id, id) == 0) {
			count++;
		}
	}
	return count;
}

/* Returns 1 if the flightID is valid, otherwise returns 0 */
int isvalid_flight_id(char* id) {
	unsigned int i, l;
	l = strlen(id);
	if (l < 3 || l > 6 || !isupper(id[0]) || !isupper(id[1])) {
		return 0;
	}
	for (i = 2; i < l; i++) {
		if (!isdigit(id[i])) {
			return 0;
		}
	}
	return 1;
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
void init_flight(Flight* flight, char* id, Airport* departure, Airport* arrival,
				 Date* departure_date, Time* departure_time, Time* duration,
				 Date* arrival_date, Time* arrival_time, int capacity) {
	strcpy(flight->id, id);
	flight->departure = departure;
	flight->arrival = arrival;
	flight->departure_date = *departure_date;
	flight->arrival_date = *arrival_date;
	flight->departure_time = *departure_time;
	flight->arrival_time = *arrival_time;
	flight->duration = *duration;
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
int compare_dates(Date* date1, Date* date2) {
	if (date1->year < date2->year) {
		return -1;
	}
	if (date1->year > date2->year) {
		return 1;
	}
	if (date1->month < date2->month) {
		return -1;
	}
	if (date1->month > date2->month) {
		return 1;
	}
	if (date1->day < date2->day) {
		return -1;
	}
	if (date1->day > date2->day) {
		return 1;
	}
	return 0;
}

/* Returns -1 if time1 happens before time2, 0 if the dates are equal and 1 if
 * time1 happens after time2 */
int compare_time(Time* time1, Time* time2) {
	if (time1->hours < time2->hours) {
		return -1;
	}
	if (time1->hours > time2->hours) {
		return 1;
	}
	if (time1->minutes < time2->minutes) {
		return -1;
	}
	if (time1->minutes > time2->minutes) {
		return 1;
	}
	return 0;
}

/* Combines the two previous funcitons */
int compare_date_and_time(Date* date1, Date* date2, Time* time1, Time* time2) {
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
Time sum_time(Time* time1, Time* time2) {
	Time result;
	result.hours = time1->hours + time2->hours;
	result.minutes = time1->minutes + time2->minutes;
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

	scanf(IN_DATE_FORMAT, &day, &month, &year);
	init_date(date, day, month, year);
}

void read_time(Time* time) {
	int minutes, hours;

	scanf(IN_TIME_FORMAT, &minutes, &hours);
	init_time(time, minutes, hours);
}

/*----------------------
 |  PRINT FUNTIONS
 -----------------------*/

/* Prints date in formatted form */
void print_date(Date* date) {
	printf(OUT_DATE_FORMAT, date->day, date->month, date->year);
}

/* Prints time in formatted form */
void print_time(Time* time) {
	printf(OUT_TIME_FORMAT, time->hours, time->minutes);
}

/*----------------------
 |  ERROR FUNTIONS
 -----------------------*/

int flight_error_handler(char* flight_id, Date* departure_date,
						 char* arrival_id, char* departure_id) {
	if (!isvalid_flight_id(flight_id)) {
		printf(INVALID_FLIGHT);
		return 1;
	}
	if (get_flight(flight_id, departure_date) >= 0) {
		printf(FLIGHT_ALREADY_EXISTS);
		return 1;
	}
	if (get_airport(arrival_id) < 0) {
		printf(NO_SUCH_AIRPORT, arrival_id);
		return 1;
	}
	if (get_airport(departure_id) < 0) {
		printf(NO_SUCH_AIRPORT, departure_id);
		return 1;
	}
	if (flights_count == MAX_FLIGHTS) {
		printf(TOO_MANY_FLIGHTS);
		return 1;
	}
	return 0;
}

void sort_arrivals() {
	int i, j;
	Flight* temp;

	for (i = 1; i < flights_count; i++) {
		temp = sorted_flights_arr[i];
		for (j = i - 1;
			 j >= 0 &&
			 compare_date_and_time(
				 &sorted_flights_arr[j]->arrival_date, &temp->arrival_date,
				 &sorted_flights_arr[j]->arrival_time, &temp->arrival_time) > 0;
			 j--) {
			sorted_flights_arr[j + 1] = sorted_flights_arr[j];
		}
		sorted_flights_arr[j + 1] = temp;
	}
}
void sort_departures() {
	int i, j;
	Flight* temp;

	for (i = 1; i < flights_count; i++) {
		temp = sorted_flights_dep[i];
		for (j = i - 1; j >= 0 && compare_date_and_time(
									  &sorted_flights_dep[j]->departure_date,
									  &temp->departure_date,
									  &sorted_flights_dep[j]->departure_time,
									  &temp->departure_time) > 0;
			 j--) {
			sorted_flights_dep[j + 1] = sorted_flights_dep[j];
		}
		sorted_flights_dep[j + 1] = temp;
	}
}