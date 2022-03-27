/*
 * File:  commands.c
 * Author:  Francisco Salgueiro
 * Description: Functions for handling commands
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "proj1.h"

/*----------------------
 |  -A COMMAND
 -----------------------*/

/* Adds a new airport to the system */
int add_airport() {
	unsigned int i, l;
	int n, ins_index;
	char airport_id[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];
	Airport airport;

	scanf("%s", airport_id);
	scanf("%s", country);
	scanf(" %[^\n]", city); /* includes everything until end of line */

	l = strlen(airport_id);
	for (i = 0; i < l; i++) {
		if (!isupper((airport_id[i]))) {
			printf(INVALID_AIRPORT_ID);
			return -1;
		}
	}
	if (airports_count == MAX_AIRPORTS) {
		printf(TOO_MANY_AIPORTS);
		return -1;
	}
	if ((ins_index = get_airport(airport_id)) >= 0) {
		printf(DUPLICATE_AIRPORT);
		return -1;
	}

	init_airport(&airport, airport_id, country, city);
	airports[airports_count] = airport;

	ins_index = -(ins_index + 1);
	n = airports_count;
	while (n > ins_index) {
		sorted_airports[n] = sorted_airports[n - 1];
		n--;
	}
	sorted_airports[ins_index] = &airports[airports_count];
	airports_count++;

	printf(AIRPORT_ADDED_MESSAGE, airport_id);
	return 0;
}

/*----------------------
 |  -L COMMAND
 -----------------------*/

/* Lists all airports in the system */
int list_airports() {
	int i, has_argument;
	char airportID[AIRPORT_ID_LENGTH];

	has_argument = 0;
	while (getchar() != '\n') {
		has_argument = 1;
		scanf("%s", airportID);
		i = get_airport(airportID);
		if (i < 0) {
			printf(NO_SUCH_AIRPORT, airportID);
			continue;
		}
		print_airport(sorted_airports[i]);
	}
	if (!has_argument) {
		for (i = 0; i < airports_count; i++) {
			print_airport(sorted_airports[i]);
		}
	}
	return 0;
}

/*----------------------
 |  -V, P AND C COMMAND
 -----------------------*/

/* Lists all flights in the system */
int list_flights(char mode) {
	int i;
	char airport_id[AIRPORT_ID_LENGTH];
	Flight* flight;

	if (mode != 'n') {
		scanf("%s", airport_id);
		if (get_airport(airport_id) < 0) {
			printf(NO_SUCH_AIRPORT, airport_id);
			return -1;
		}
	}

	for (i = 0; i < flights_count; i++) {
		if (mode == 'n') {
			flight = &flights[i];
		} else if (mode == 'c') {
			flight = sorted_flights_arr[i];
		} else {
			flight = sorted_flights_dep[i];
		}
		if (mode == 'n' ||
			(mode == 'c' && strcmp(flight->arrival->id, airport_id) == 0) ||
			(mode == 'p' && strcmp(flight->departure->id, airport_id) == 0)) {
			print_flight(flight, mode);
		}
	}
	return 0;
}

/*----------------------
 |  -V COMMAND
 -----------------------*/

/* Adds a new flight to the system */
int add_flight() {
	char flight_id[FLIGHT_ID_LENGTH];
	char departure_id[AIRPORT_ID_LENGTH];
	char arrival_id[AIRPORT_ID_LENGTH];
	Airport* departure;
	Airport* arrival;
	Date departure_date, arrival_date, future_date;
	Time departure_time, arrival_time, duration;
	int capacity;
	Flight flight;
	char c;

	c = getchar();
	if (c == '\n') {
		list_flights('n');
		return 0;
	}
	scanf("%s", flight_id);
	scanf("%s", departure_id);
	scanf("%s", arrival_id);

	read_date(&departure_date);
	read_time(&departure_time);
	read_time(&duration);
	scanf("%d", &capacity);

	if (flight_error_handler(flight_id, &departure_date, arrival_id,
							 departure_id))
		return -1;

	future_date = date;
	future_date.year++;
	if (compare_date(&departure_date, &date) < 0 ||
		compare_date(&departure_date, &future_date) > 0) {
		printf(INVALID_DATE);
		return -1;
	}

	if (duration.hours > MAX_DURATION ||
		(duration.hours == MAX_DURATION && duration.minutes > 0)) {
		printf(INVALID_DURATION);
		return -1;
	}
	if (capacity < MIN_CAPACITY || capacity > MAX_CAPACITY) {
		printf(INVALID_CAPACITY);
		return -1;
	}

	arrival = sorted_airports[get_airport(arrival_id)];
	departure = sorted_airports[get_airport(departure_id)];

	init_flight(&flight, flight_id, departure, arrival, &departure_date,
				&departure_time, &duration, &arrival_date, &arrival_time,
				capacity);

	arrival_time = sum_time(&departure_time, &duration);
	if (arrival_time.hours >= NUM_HOURS) {
		arrival_time.hours -= NUM_HOURS;
		arrival_date = increment_date(departure_date);
	} else {
		arrival_date = departure_date;
	}

	flight.arrival_date = arrival_date;
	flight.arrival_time = arrival_time;

	flights[flights_count] = flight;
	sorted_flights_arr[flights_count] = &flights[flights_count];
	sorted_flights_dep[flights_count] = &flights[flights_count];
	flights_count++;
	is_departures_sorted = 0;
	is_arrivals_sorted = 0;
	return 0;
}

/*----------------------
 |  -T COMMAND
 -----------------------*/

/* Changes the system date */
int change_date() {
	Date new_date;
	Date future_date;

	read_date(&new_date);
	future_date = date;
	future_date.year++;
	if (compare_date(&new_date, &date) < 0 ||
		compare_date(&new_date, &future_date) > 0) {
		printf(INVALID_DATE);
		return -1;
	}
	date = new_date;

	print_date(&date);
	putchar('\n');

	return 0;
}