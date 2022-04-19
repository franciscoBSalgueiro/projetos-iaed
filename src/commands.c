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

	ins_index = -(ins_index + 1);

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
	if (!has_argument)
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

	hashtable_add(gbsystem.flight_ht, new_flight);
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
	insertion_sort(&gbsystem.is_dep_sorted, gbsystem.dep_flights, &dep_date_key,
				   &dep_time_key);
	list_flights(gbsystem.dep_flights, &dep_id_key, &arr_id_key, &dep_date_key,
				 &dep_time_key);
}

/* Lists all flights arriving to the specified airport */
void list_arrivals() {
	insertion_sort(&gbsystem.is_arr_sorted, gbsystem.arr_flights, &arr_date_key,
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

/* Lists all the reservations from flight or adds a new one */
void list_reservations() {
	char flight_id[FLIGHT_ID_LENGTH];
	Date date;
	Flight* f;

	scanf("%s", flight_id);
	read_date(&date);

	if ((f = hashtable_get_flight(gbsystem.flight_ht, flight_id, &date)) == NULL) {
		printf(NO_SUCH_FLIGHT, flight_id);
		return;
	}

	if (getchar() != '\n')
		add_reservation(f);
	else
		list_flight_reservations(f);
}

/* Adds a new reservation to the flight */
void add_reservation(Flight* flight) {
	char reservation_id[MAX_CMD_LEN];
	Reservation *reservation;
	int passengers;

	scanf("%s", reservation_id);
	scanf("%d", &passengers);

	if (!isvalid_reservation_id(reservation_id)) {
		printf(INVALID_RESERVATION);
		return;
	}

	if (hashtable_contains(gbsystem.reservation_ht, reservation_id)) {
		printf(RESERVATION_ALREADY_EXISTS, reservation_id);
		return;
	}

	if (flight->taken_seats + passengers > flight->capacity) {
		printf(TOO_MANY_RESERVATIONS);
		return;
	}

	if (!isvalid_date(&flight->dep_date)) {
		printf(INVALID_DATE);
		return;
	}

	if (passengers < 1) {
		printf(INVALID_PASSENGER);
		return;
	}

	reservation = custom_alloc(sizeof(Reservation));
	reservation->id = custom_alloc(sizeof(char) * (strlen(reservation_id) + 1));
	reservation->passengers = passengers;
	strcpy(reservation->id, reservation_id);
	reservation->flight = flight;

	hashtable_add(gbsystem.reservation_ht, reservation);
	list_insert(&flight->reservations, reservation, NULL);

	flight->taken_seats += reservation->passengers;

	return;
}

/*----------------------
 |  -E COMMAND
 -----------------------*/

/* Deletes reservation or flight by id */
void delete_reservation() {
	char id[MAX_CMD_LEN];
	int i, found = FALSE;
	List *lr;
	Reservation* r;
	ListNode *node, *prev;
	Flight* f;

	scanf("%s", id);

	if (strlen(id) < 10) {
		for(i = 0; i < gbsystem.flights_count; i++) {
			if (strcmp(gbsystem.flights[i].id, id) == 0) {
				found = TRUE;
				delete_flight(i);
				i--;
			}
		}
	} else if ((f = hashtable_get(gbsystem.reservation_ht, id)) != NULL) {
		lr = &f->reservations;
		found = TRUE;

		for (node = lr->head, prev = NULL; node != NULL;
			 prev = node, node = node->next) {
			r = (Reservation*)node->data;
			if (strcmp(r->id, id) == 0) {
				f->taken_seats -= r->passengers;
				hashtable_remove(gbsystem.reservation_ht, r);
				list_remove(lr, node, prev);
				break;
			}
		}
	}
	if (!found) printf(NOT_FOUND);
}