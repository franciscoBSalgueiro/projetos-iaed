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

/* Adds an airport to the global state */
int add_airport(Global_State* global) {
	size_t i;
	int n, ins_index;
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
	if ((ins_index = get_airport(global, airport_id)) >= 0) {
		printf(DUPLICATE_AIRPORT);
		return -1;
	}

	init_airport(&airport, airport_id, country, city);

	ins_index = -(ins_index + 1);
	n = global->airports_count;
	while (n > ins_index) {
		global->airports[n] = global->airports[n - 1];
		n--;
	}

	global->airports[ins_index] = airport;
	global->airports_count++;

	printf(AIRPORT_ADDED_MESSAGE, airport_id);
	return 0;
}

/*----------------------
 |  -L COMMAND
 -----------------------*/

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
			if (i < 0) {
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

/*----------------------
 |  -V, P AND C COMMAND
 -----------------------*/

int list_flights(Global_State* global, char mode) {
	int i;
	char airport_id[AIRPORT_ID_LENGTH];
	Flight flight;

	if (mode != 'n') {
		scanf("%s", airport_id);
		if (get_airport(global, airport_id) < 0) {
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

/*----------------------
 |  -V COMMAND
 -----------------------*/

/* Adds a flight to the global state */
int add_flight(Global_State* global) {
	int i;
	char flight_id[FLIGHT_ID_LENGTH];
	char departure_id[AIRPORT_ID_LENGTH];
	char arrival_id[AIRPORT_ID_LENGTH];
	Airport departure, arrival;
	Date departure_date;
	Date arrival_date;
	Date future_date;
	Time departure_time;
	Time arrival_time;
	Time duration;
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

	read_date(&departure_date);
	read_time(&departure_time);
	read_time(&duration);
	scanf("%d", &capacity);

	if (check_flight_id(flight_id)) {
		printf(INVALID_FLIGHT);
		return -1;
	}
	if (get_flight(global, flight_id, departure_date) >= 0) {
		printf(FLIGHT_ALREADY_EXISTS);
		return -1;
	}
	if (get_airport(global, arrival_id) < 0) {
		printf(NO_SUCH_AIRPORT, arrival_id);
		return -1;
	}
	if (get_airport(global, departure_id) < 0) {
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
	if (duration.hours > 12 || (duration.hours == 12 && duration.minutes > 0)) {
		printf(INVALID_DURATION);
		return -1;
	}
	if (capacity < 10 || capacity > 100) {
		printf(INVALID_CAPACITY);
		return -1;
	}

	arrival = global->airports[get_airport(global, arrival_id)];
	departure = global->airports[get_airport(global, departure_id)];

	init_flight(&flight, flight_id, departure, arrival, departure_date,
				departure_time, duration, arrival_date, arrival_time, capacity);

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

/*----------------------
 |  -T COMMAND
 -----------------------*/

int change_date(Global_State* global) {
	Date date;
	Date future_date;

	read_date(&date);
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