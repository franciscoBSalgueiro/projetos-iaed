/*
 * File:  proj1.c
 * Author:  Francisco Salgueiro
 * Description: Main project file
 */

#include "proj1.h"

#include <stdio.h>

/* System declaration */
System system;

/* Initializes the system and waits for user commands */
int main() {
	/* System initialization with initial values */
	system.airports_count = 0;
	system.flights_count = 0;
	system.is_departures_sorted = 0;
	system.is_arrivals_sorted = 0;
	init_date(&system.date, INITIAL_DATE);

	/* Continues until the user enters the command 'q' */
	while (cmd_triage()) {
	}

	return 0;
}

/* Reads the command prefix and calls the respective function */
int cmd_triage() {
	char c = getchar();
	switch (c) {
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
		default: break;
	}
	return 1;
}