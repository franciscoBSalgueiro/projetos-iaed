/*
 * File:  proj2.c
 * Author:  Francisco Salgueiro
 * Description: Main project file
 */

#include "proj2.h"

#include <stdio.h>
#include <stdlib.h>

System gbsystem;

/* Initializes the system, waits for user commands then frees used memory */
int main() {
	gbsystem.airports_count = 0;
	gbsystem.flights_count = 0;
	gbsystem.is_dep_sorted = FALSE;
	gbsystem.is_arr_sorted = FALSE;
	init_date(&gbsystem.date, INITIAL_DATE);

	gbsystem.reservation_ids = hashtable_create(res_key);

	while (cmd_triage()) {
	}

	clear_memory();
	return 0;
}

/* Reads the command prefix and calls the respective function */
int cmd_triage() {
	switch (getchar()) {
		case QUIT_CMD: return 0;
		case ADD_AIRPORT_CMD: add_airport(); break;
		case LIST_AIRPORTS_CMD: list_airports(); break;
		case ADD_FLIGHT_CMD:
			if (getchar() == '\n')
				list_all_flights();
			else
				add_flight();
			break;
		case LIST_DEPARTURES_CMD: list_departures(); break;
		case LIST_ARRIVALS_CMD: list_arrivals(); break;
		case CHANGE_DATE_CMD: change_date(); break;
		case LIST_RESERVATIONS_CMD: list_reservations(); break;
		case DELETE_RESERVATION_CMD: delete_reservation(); break;
		default: break;
	}
	return 1;
}