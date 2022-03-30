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
	/* System initialization with default values */
	system.airports_count = 0;
	system.flights_count = 0;
	system.is_departures_sorted = 0;
	system.is_arrivals_sorted = 0;
	init_date(&system.date, INITIAL_DATE);

	/* Continues until the user enters the command "q" */
	while (cmd_triage()) {
	}

	return 0;
}

/* Reads the command prefix and calls the respective function */
int cmd_triage() {
	char c = getchar();
	switch (c) {
		case 'q': /* Ends the program */
			return 0;
		case 'a':
			add_airport();
			break;
		case 'l':
			list_airports();
			break;
		case 'v':
			if (getchar() == '\n')
				list_all_flights();
			else
				add_flight();
			break;
		case 'p':
			sort_departures();
			list_flights('p');
			break;
		case 'c':
			sort_arrivals();
			list_flights('c');
			break;
		case 't':
			change_date();
			break;
		default: /* ignores unknown commands */
			break;
	}
	return 1;
}