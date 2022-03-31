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
void add_airport() {
	int ins_index;
	char airport_id[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];

	scanf(IN_AIRPORT_FORMAT, airport_id, country, city);

	/* Error checks */
	if (!isvalid_airport_id(airport_id)) {
		printf(INVALID_AIRPORT_ID);
		return;
	}
	if (system.airports_count == MAX_AIRPORTS) {
		printf(TOO_MANY_AIPORTS);
		return;
	}
	if ((ins_index = get_airport(airport_id)) >= 0) {
		printf(DUPLICATE_AIRPORT);
		return;
	}

	ins_index = -(ins_index + 1); /* Convert index to positive */

	/* Inserts the airport in the correct position alphabetically */
	memmove(&system.airports[ins_index + 1], &system.airports[ins_index],
			(system.airports_count - ins_index) * sizeof(Airport));
	init_airport(&system.airports[ins_index], airport_id, country, city);

	system.airports_count++;

	printf(AIRPORT_ADDED_MESSAGE, airport_id);
}

/*----------------------
 |  -L COMMAND
 -----------------------*/

/* Lists all airports in the system */
void list_airports() {
	int i, has_argument;
	char airportID[AIRPORT_ID_LENGTH];

	has_argument = FALSE;
	while (getchar() != '\n') {
		has_argument = TRUE;
		scanf("%s", airportID);
		i = get_airport(airportID);
		if (i < 0) {
			printf(NO_SUCH_AIRPORT, airportID);
			continue;
		}
		print_airport(&system.airports[i]);
	}
	if (!has_argument) /* Only prints all airports if no argument is given */
		for (i = 0; i < system.airports_count; i++)
			print_airport(&system.airports[i]);
}

/*----------------------
 |  -V COMMAND
 -----------------------*/

/* Adds a new flight to the system */
void add_flight() {
	Flight* new_flight;

	new_flight = &system.flights[system.flights_count];
	if (read_flight(new_flight) == ERROR) return;
	calculate_arrival(new_flight);

	system.arr_flights[system.flights_count] = new_flight;
	system.dep_flights[system.flights_count] = new_flight;
	system.flights_count++;

	/* Resets flags as the array may no longer be sorted */
	system.is_dep_sorted = FALSE;
	system.is_arr_sorted = FALSE;
}

/* Lists all flights in the system */
void list_all_flights() {
	int i;
	for (i = 0; i < system.flights_count; i++) {
		print_flight_full(&system.flights[i]);
	}
}

/*----------------------
 |  - P AND C COMMANDS  |
 -----------------------*/

/* Lists all flights departing from the specified airport */
void list_departures() {
	insertion_sort(system.is_dep_sorted, system.dep_flights, &dep_date_key,
				   &dep_time_key);
	list_flights(system.dep_flights, &dep_id_key, &arr_id_key, &dep_date_key,
				 &dep_time_key);
}

/* Lists all flights arriving to the specified airport */
void list_arrivals() {
	insertion_sort(system.is_arr_sorted, system.arr_flights, &arr_date_key,
				   &arr_time_key);
	list_flights(system.arr_flights, &arr_id_key, dep_id_key, &arr_date_key,
				 &arr_time_key);
}

/* Auxiliary function for listing flights from specified airport */
void list_flights(Flight* array[], char* (*airport_key_in)(Flight*),
				  char* (*airport_key_out)(Flight*), Date* (*date_key)(Flight*),
				  Time* (*time_key)(Flight*)) {
	int i;
	char airport_id[AIRPORT_ID_LENGTH];

	scanf("%s", airport_id);
	if (get_airport(airport_id) < 0) {
		printf(NO_SUCH_AIRPORT, airport_id);
		return;
	}

	for (i = 0; i < system.flights_count; i++) {
		/* Checks for flights from the specified airport */
		if (strcmp(airport_key_in(array[i]), airport_id) == 0) {
			print_flight(array[i]->id, airport_key_out(array[i]),
						 date_key(array[i]), time_key(array[i]));
		}
	}
}

/*----------------------
 |  -T COMMAND
 -----------------------*/

/* Changes the system date */
void change_date() {
	Date new_date;

	read_date(&new_date);
	if (!isvalid_date(&new_date)) {
		printf(INVALID_DATE);
		return;
	}
	system.date = new_date;

	print_date(&system.date);
	putchar('\n');
}