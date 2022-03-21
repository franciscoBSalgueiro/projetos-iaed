/*
 * File:  proj1.c
 * Author:  Francisco Salgueiro
 * Description: A program for creating airports and flights
*/

#include "proj1.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	Global_State global;
	Date initial_date;

	/* Set initial state */
	initial_date.day = 1;
	initial_date.month = 1;
	initial_date.year = 2022;

	global.airports_count = 0;
	global.flights_count = 0;
	global.date = initial_date;

	while (commands(&global)) {
	}
	return 0;
}

/* handles commands, returns 0 to quit program */
int commands(Global_State* global) {
	char c;
	c = getchar();
	switch (c) {
		case 'q':
			return 0;
		case 'a':
			add_airport(global);
			return 1;
		case 'l':
			list_airports(global);
			return 1;
		case 'v':
			add_flight(global);
			return 1;
		case 'p':
			list_flights(global, 'p');
			return 1;
		case 'c':
			list_flights(global, 'c');
			return 1;
		case 't':
			change_date(global);
			return 1;
		default:
			return 1;
	}
}

/* Adds an airport to the global state */
int add_airport(Global_State* global) {
	size_t i;
	char airport_id[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];
	Airport airport;

	scanf("%s", airport_id);
	scanf("%s", country);
	scanf(" %[^\n]", city); /* includes everything until end of line */

	for (i = 0; i < strlen(airport_id); i++) {
		if (!isupper((airport_id[i]))) {
			printf(INVALID_AIRPORT_ID);
			return -1;
		}
	}
	if (global->airports_count == MAX_AIRPORTS) {
		printf(TOO_MANY_AIPORTS);
		return -1;
	}
	if (get_airport(global, airport_id) >= 0) {
		printf(DUPLICATE_AIRPORT);
		return -1;
	}

	strcpy(airport.id, airport_id);
	strcpy(airport.city, city);
	strcpy(airport.country, country);

	for (i = global->airports_count;
		 i > 0 && strcmp(airport_id, global->airports[i - 1].id) < 0; i--) {
		global->airports[i] = global->airports[i - 1];
	}
	global->airports[i] = airport;
	global->airports_count++;

	printf(AIRPORT_ADDED_MESSAGE, airport_id);
	return 0;
}

/* Adds a flight to the global state */
int add_flight(Global_State* global) {
	int i;
	char flight_id[FLIGHT_ID_LENGTH];
	char departure_id[AIRPORT_ID_LENGTH];
	char arrival_id[AIRPORT_ID_LENGTH];
	Date departure_date;
	Date arrival_date;
	Date future_date;
	Time departure_time;
	Time arrival_time;
	int day, month, year, hour, minutes;
	Time duration;
	int duration_hours, duration_minutes;
	int capacity;
	Flight flight;
	char c;

	c = getchar();
	if (c == '\n') {
		list_flights(global, 'n');
		return 0;
	}
	scanf("%s", flight_id);
	scanf("%s", departure_id);
	scanf("%s", arrival_id);
	scanf("%d%*c", &day);
	scanf("%d%*c", &month);
	scanf("%d", &year);
	scanf("%d%*c", &hour);
	scanf("%d", &minutes);
	scanf("%d%*c", &duration_hours);
	scanf("%d", &duration_minutes);
	scanf("%d", &capacity);
	departure_date.year = year;
	departure_date.month = month;
	departure_date.day = day;
	departure_time.hours = hour;
	departure_time.minutes = minutes;
	duration.hours = duration_hours;
	duration.minutes = duration_minutes;

	if (check_flight_id(flight_id)) {
		printf(INVALID_FLIGHT);
		return -1;
	}
	if (get_flight(global, flight_id, departure_date) != -1) {
		printf(FLIGHT_ALREADY_EXISTS);
		return -1;
	}
	if (get_airport(global, arrival_id) == -1) {
		printf(NO_SUCH_AIRPORT, arrival_id);
		return -1;
	}
	if (get_airport(global, departure_id) == -1) {
		printf(NO_SUCH_AIRPORT, departure_id);
		return -1;
	}
	if (global->flights_count == MAX_FLIGHTS) {
		printf(TOO_MANY_FLIGHTS);
		return -1;
	}

	future_date = global->date;
	future_date.year++;
	if (compare_dates(departure_date, global->date) < 0 ||
		compare_dates(departure_date, future_date) > 0) {
		printf(INVALID_DATE);
		return -1;
	}
	if (duration_hours > 12 || (duration_hours == 12 && duration_minutes > 0)) {
		printf(INVALID_DURATION);
		return -1;
	}
	if (capacity < 10 || capacity > 100) {
		printf(INVALID_CAPACITY);
		return -1;
	}

	strcpy(flight.id, flight_id);
	flight.arrival = global->airports[get_airport(global, arrival_id)];
	flight.departure = global->airports[get_airport(global, departure_id)];
	flight.departure_date = departure_date;
	flight.departure_time = departure_time;
	flight.duration = duration;
	flight.capacity = capacity;

	arrival_time = sum_time(departure_time, duration);
	if (arrival_time.hours >= 24) {
		arrival_time.hours -= 24;
		arrival_date = increment_date(departure_date);
	} else {
		arrival_date = departure_date;
	}

	flight.arrival_date = arrival_date;
	flight.arrival_time = arrival_time;

	global->flights[global->flights_count] = flight;
	global->flights_count++;

	/* TODO improve insert sorted */
	for (i = global->flights_count - 1;
		 i > 0 &&
		 (compare_date_and_time(
			  arrival_date, global->sorted_flights_arr[i - 1]->arrival_date,
			  arrival_time,
			  global->sorted_flights_arr[i - 1]->arrival_time) < 0);
		 i--) {
		global->sorted_flights_arr[i] = global->sorted_flights_arr[i - 1];
	}
	global->sorted_flights_arr[i] = &global->flights[global->flights_count - 1];

	for (i = global->flights_count - 1;
		 i > 0 &&
		 (compare_date_and_time(
			  departure_date, global->sorted_flights_dep[i - 1]->departure_date,
			  departure_time,
			  global->sorted_flights_dep[i - 1]->departure_time) < 0);
		 i--) {
		global->sorted_flights_dep[i] = global->sorted_flights_dep[i - 1];
	}
	global->sorted_flights_dep[i] = &global->flights[global->flights_count - 1];

	return 0;
}

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

int check_flight_id(char* id) {
	size_t i;
	size_t length_id;
	length_id = strlen(id);
	if (length_id < 3 || length_id > 6) {
		return 1;
	}
	if (id[0] < 'A' || id[0] > 'Z' || id[1] < 'A' || id[1] > 'Z') {
		return 1;
	}
	for (i = 2; i < length_id; i++) {
		if (id[i] < '0' || id[i] > '9') {
			return 1;
		}
	}
	return 0;
}

/* returns index of airport or -1 if it doesn't exist */
int get_airport(Global_State* global, char* id) {
	int i;
	for (i = 0; i < global->airports_count; i++) {
		if (strcmp(global->airports[i].id, id) == 0) {
			return i;
		}
	}
	return -1;
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

int list_airports(Global_State* global) {
	int i, num_flights;
	char c;
	char airportID[AIRPORT_ID_LENGTH];
	Airport airport;

	c = getchar();
	if (c == '\n') {
		for (i = 0; i < global->airports_count; i++) {
			airport = global->airports[i];
			num_flights = get_num_flights(global, airport.id);
			printf(AIRPORT_STRING, airport.id, airport.city, airport.country,
				   num_flights);
		}
	} else {
		while (scanf("%s", airportID) != EOF) {
			i = get_airport(global, airportID);
			if (i == -1) {
				printf(NO_SUCH_AIRPORT, airportID);
				continue;
			}
			airport = global->airports[i];
			num_flights = get_num_flights(global, airport.id);
			printf(AIRPORT_STRING, airport.id, airport.city, airport.country,
				   num_flights);
			if (getchar() == '\n') {
				break;
			}
		}
	}
	return 0;
}

int list_flights(Global_State* global, char mode) {
	int i;
	char airport_id[AIRPORT_ID_LENGTH];
	Flight flight;

	if (mode != 'n') {
		scanf("%s", airport_id);
		if (get_airport(global, airport_id) == -1) {
			printf(NO_SUCH_AIRPORT, airport_id);
			return -1;
		}
	}

	for (i = 0; i < global->flights_count; i++) {
		if (mode == 'n') {
			flight = global->flights[i];
		} else if (mode == 'c') {
			flight = *global->sorted_flights_arr[i];
		} else {
			flight = *global->sorted_flights_dep[i];
		}
		if (mode == 'n' ||
			(mode == 'c' && strcmp(flight.arrival.id, airport_id) == 0) ||
			(mode == 'p' && strcmp(flight.departure.id, airport_id) == 0)) {
			switch (mode) {
				case 'c':
					printf(FLIGHT_STRING_REDUCED, flight.id,
						   flight.departure.id);
					break;
				case 'p':
					printf(FLIGHT_STRING_REDUCED, flight.id, flight.arrival.id);
					break;
				default:
					printf(FLIGHT_STRING, flight.id, flight.departure.id,
						   flight.arrival.id);
					break;
			}
			if (mode == 'c') {
				print_date(flight.arrival_date);
				putchar(' ');
				print_time(flight.arrival_time);
				putchar('\n');
			} else {
				print_date(flight.departure_date);
				putchar(' ');
				print_time(flight.departure_time);
				putchar('\n');
			}
		}
	}
	return 0;
}

int change_date(Global_State* global) {
	Date date;
	Date future_date;
	int year, month, day;

	scanf("%d%*c", &day);
	scanf("%d%*c", &month);
	scanf("%d", &year);

	date.year = year;
	date.month = month;
	date.day = day;

	future_date = global->date;
	future_date.year++;
	if (compare_dates(date, global->date) < 0 ||
		compare_dates(date, future_date) > 0) {
		printf(INVALID_DATE);
		return -1;
	}
	global->date = date;

	print_date(date);
	putchar('\n');

	return 0;
}

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

/* Prints date in formatted form */
void print_date(Date date) {
	printf("%02d-%02d-%04d", date.day, date.month, date.year);
}

/* Prints time in formatted form */
void print_time(Time time) { printf("%02d:%02d", time.hours, time.minutes); }