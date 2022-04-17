/*
 * File:  proj1.c
 * Author:  Francisco Salgueiro
 * Description: Main project file
 */

#include "proj1.h"

#include <stdio.h>
#include <stdlib.h>

/* System declaration */
System gbsystem;

/* Initializes the system and waits for user commands */
int main() {

	/* System initialization with initial values */
	gbsystem.airports_count = 0;
	gbsystem.flights_count = 0;
	gbsystem.is_dep_sorted = FALSE;
	gbsystem.is_arr_sorted = FALSE;
	init_date(&gbsystem.date, INITIAL_DATE);

	gbsystem.reservation_ids = hashtable_create();

	/* Continues until the user enters the command 'q' */
	while (cmd_triage()) {
	}

	/* free all used memory */
	clear_memory();
	return 0;
}

/* Reads the command prefix and calls the respective function */
int cmd_triage() {
	switch (getchar()) {
		case QUIT_CMD: return 0; /* ends the program */
		case ADD_AIRPORT_CMD: add_airport(); break;
		case LIST_AIRPORTS_CMD: list_airports(); break;
		case ADD_FLIGHT_CMD:
			if (getchar() == '\n') /* No argument given */
				list_all_flights();
			else
				add_flight();
			break;
		case LIST_DEPARTURES_CMD: list_departures(); break;
		case LIST_ARRIVALS_CMD: list_arrivals(); break;
		case CHANGE_DATE_CMD: change_date(); break;
		case LIST_RESERVATIONS_CMD: list_reservations(); break;
		case DELETE_RESERVATION_CMD: delete_reservation(); break;
		default: break; /* ignore invalid commands */
	}
	return 1; /* waits for next command */
}