/*
 * File:  commands.c
 * Author:  Francisco Salgueiro
 * Description: Functions for handling commands
 */

#include <stdio.h>
#include <stdlib.h>
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
	if (gbsystem.airports_count == MAX_AIRPORTS) {
		printf(TOO_MANY_AIPORTS);
		return;
	}
	if ((ins_index = get_airport(airport_id)) >= 0) {
		printf(DUPLICATE_AIRPORT);
		return;
	}

	ins_index = -(ins_index + 1); /* Convert index to positive */

	/* Inserts the airport in the correct position alphabetically */
	memmove(&gbsystem.airports[ins_index + 1], &gbsystem.airports[ins_index],
			(gbsystem.airports_count - ins_index) * sizeof(Airport));
	init_airport(&gbsystem.airports[ins_index], airport_id, country, city);

	gbsystem.airports_count++;

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
		print_airport(&gbsystem.airports[i]);
	}
	if (!has_argument) /* Only prints all airports if no argument is given */
		for (i = 0; i < gbsystem.airports_count; i++)
			print_airport(&gbsystem.airports[i]);
}

/*----------------------
 |  -V COMMAND
 -----------------------*/

/* Adds a new flight to the system */
void add_flight() {
	Flight* new_flight;

	new_flight = &gbsystem.flights[gbsystem.flights_count];
	if (read_flight(new_flight) == ERROR) return;
	calculate_arrival(new_flight);

	gbsystem.arr_flights[gbsystem.flights_count] = new_flight;
	gbsystem.dep_flights[gbsystem.flights_count] = new_flight;
	gbsystem.flights_count++;

	/* Resets flags as the array may no longer be sorted */
	gbsystem.is_dep_sorted = FALSE;
	gbsystem.is_arr_sorted = FALSE;
}

/* Lists all flights in the system */
void list_all_flights() {
	int i;
	for (i = 0; i < gbsystem.flights_count; i++) {
		print_flight_full(&gbsystem.flights[i]);
	}
}

/*----------------------
 |  - P AND C COMMANDS  |
 -----------------------*/

/* Lists all flights departing from the specified airport */
void list_departures() {
	insertion_sort(gbsystem.is_dep_sorted, gbsystem.dep_flights, &dep_date_key,
				   &dep_time_key);
	list_flights(gbsystem.dep_flights, &dep_id_key, &arr_id_key, &dep_date_key,
				 &dep_time_key);
}

/* Lists all flights arriving to the specified airport */
void list_arrivals() {
	insertion_sort(gbsystem.is_arr_sorted, gbsystem.arr_flights, &arr_date_key,
				   &arr_time_key);
	list_flights(gbsystem.arr_flights, &arr_id_key, dep_id_key, &arr_date_key,
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

	for (i = 0; i < gbsystem.flights_count; i++) {
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
	gbsystem.date = new_date;

	print_date(&gbsystem.date);
	putchar('\n');
}

/*----------------------
 |  -R COMMAND
 -----------------------*/

void list_reserves() {
	char flight_id[FLIGHT_ID_LENGTH];
	Date date;
	Flight* f;
	int i;

	scanf("%s", flight_id);
	read_date(&date);

	if ((i = get_flight(flight_id, &date)) == -1) {
		printf(NO_SUCH_FLIGHT, flight_id);
		return;
	}

	f = &gbsystem.flights[i];

	if (getchar() != '\n')
		add_reserve(f);
	else
		list_flight_reserves(f);
}

void add_reserve(Flight* flight) {
	char reserve_id[MAX_CMD_LEN];
	Reserve *reserve, *tmp;
	int i, passengers, b = FALSE;

	scanf("%s", reserve_id);
	scanf("%d", &passengers);

	if (!isvalid_reserve_id(reserve_id)) {
		printf(INVALID_RESERVE);
		return;
	}

	if (res_id_already_exists(reserve_id)) {
		printf(RESERVE_ALREADY_EXISTS, reserve_id);
		return;
	}

	if (flight->taken_seats + passengers > flight->capacity) {
		printf(TOO_MANY_RESERVES);
		return;
	}

	if (!isvalid_date(&flight->dep_date)) {
		printf(INVALID_DATE);
		return;
	}

	if (passengers < 0) {
		printf(INVALID_PASSENGER);
		return;
	}

	reserve = malloc(sizeof(Reserve));
	reserve->id = malloc(sizeof(char) * (strlen(reserve_id) + 1));
	reserve->passengers = passengers;
	strcpy(reserve->id, reserve_id);

	/* Insert in list alphabetically */
	for (i = 0; i < flight->reserves.size; i++) {
		tmp = (Reserve*)list_get(&flight->reserves, i);
		if (strcmp(reserve->id, tmp->id) < 0) {
			list_insert(&flight->reserves, reserve, i);
			b = TRUE;
			break;
		}
	}
	if (!b) list_add(&flight->reserves, reserve);

	flight->taken_seats += reserve->passengers;

	return;
}

/*----------------------
 |  -E COMMAND
 -----------------------*/

void delete_reserve() {
	char id[MAX_CMD_LEN];
	int i, j, found = FALSE;
	List lf, *lr;
	Reserve* r;
	ListNode* n;

	scanf("%s", id);

	if (strlen(id) < 10) {
		list_init(&lf);
		get_all_flights(&lf, id);
		for (n = lf.head; n != NULL; n = n->next) {
			found = TRUE;
			delete_flight(*(int*)n->data);
		}
		list_destroy(&lf);
	} else {
		for (i = 0; i < gbsystem.flights_count; i++) {
			lr = &gbsystem.flights[i].reserves;
			for (j = 0; j < lr->size; j++) {
				r = (Reserve*)list_get(lr, j);
				if (strcmp(r->id, id) == 0) {
					found = TRUE;
					free(r->id);
					list_remove(lr, j);
					lr->size--;
				}
			}
		}
	}
	if (!found) printf(NOT_FOUND);
}