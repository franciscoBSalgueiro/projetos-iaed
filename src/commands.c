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
	int n, ins_index;
	char airport_id[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];

	scanf(IN_AIRPORT_FORMAT, airport_id, country, city);
	if (!isvalid_airport_id(airport_id)) {
		printf(INVALID_AIRPORT_ID);
		return ERROR;
	}
	if (system.airports_count == MAX_AIRPORTS) {
		printf(TOO_MANY_AIPORTS);
		return ERROR;
	}
	if ((ins_index = get_airport(airport_id)) >= 0) {
		printf(DUPLICATE_AIRPORT);
		return ERROR;
	}

	ins_index = -(ins_index + 1); /* Convert index to positive */
	for (n = system.airports_count; n > ins_index; n--) {
		system.airports[n] = system.airports[n - 1];
	}
	init_airport(&system.airports[ins_index], airport_id, country, city);
	system.airports_count++;

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
		print_airport(&system.airports[i]);
	}
	if (!has_argument)
		for (i = 0; i < system.airports_count; i++)
			print_airport(&system.airports[i]);
	return 0;
}

/*----------------------
 |  -V COMMAND
 -----------------------*/

/* Adds a new flight to the system */
int add_flight() {
	Flight* new_flight;

	new_flight = &system.flights[system.flights_count];
	if (read_flight(new_flight) == ERROR) return ERROR;
	calculate_arrival(new_flight);

	system.sorted_flights_arr[system.flights_count] = new_flight;
	system.sorted_flights_dep[system.flights_count] = new_flight;
	system.flights_count++;
	system.is_departures_sorted = 0;
	system.is_arrivals_sorted = 0;
	return 0;
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
	insertion_sort(system.is_departures_sorted, system.sorted_flights_dep,
				   &dep_date_key, &dep_time_key);
	list_flights(system.sorted_flights_dep, &dep_id_key, &arr_id_key,
				 &dep_date_key, &dep_time_key);
}

/* Lists all flights arriving to the specified airport */
void list_arrivals() {
	insertion_sort(system.is_arrivals_sorted, system.sorted_flights_arr,
				   &arr_date_key, &arr_time_key);
	list_flights(system.sorted_flights_arr, &arr_id_key, dep_id_key,
				 &arr_date_key, &arr_time_key);
}

/* Auxiliary function for listing flights */
int list_flights(Flight* arr[], char* (*airport_key_in)(Flight*),
				 char* (*airport_key_out)(Flight*), Date* (*date_key)(Flight*),
				 Time* (*time_key)(Flight*)) {
	int i;
	char airport_id[AIRPORT_ID_LENGTH];

	scanf("%s", airport_id);
	if (get_airport(airport_id) < 0) {
		printf(NO_SUCH_AIRPORT, airport_id);
		return ERROR;
	}

	for (i = 0; i < system.flights_count; i++) {
		if (strcmp(airport_key_in(arr[i]), airport_id) == 0) {
			print_flight(arr[i]->id, airport_key_out(arr[i]), date_key(arr[i]),
						 time_key(arr[i]));
		}
	}
	return 0;
}

/*----------------------
 |  -T COMMAND
 -----------------------*/

/* Changes the system date */
int change_date() {
	Date new_date;

	read_date(&new_date);
	if (!isvalid_date(&new_date)) {
		printf(INVALID_DATE);
		return ERROR;
	}
	system.date = new_date;

	print_date(&system.date);
	putchar('\n');

	return 0;
}