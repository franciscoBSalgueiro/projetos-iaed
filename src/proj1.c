/*
 * File:  proj1.c
 * Author:  Francisco Salgueiro
 * Description: Main project file
 */

#include "proj1.h"

#include <stdio.h>

/* Declare global variables */
int airports_count = 0;
int flights_count = 0;
Airport airports[MAX_AIRPORTS];
Airport* sorted_airports[MAX_AIRPORTS];
Flight flights[MAX_FLIGHTS];
Flight* sorted_flights_dep[MAX_FLIGHTS];
Flight* sorted_flights_arr[MAX_FLIGHTS];
int is_departures_sorted = 0;
int is_arrivals_sorted = 0;
Date date = {1, 1, 2022}; /* Default initial date */

/* Reads command prefixes and calls the respective function */
int main() {
	char c;
	while ((c = getchar()) != 'q') {
		switch (c) {
			case 'a':
				add_airport();
				break;
			case 'l':
				list_airports();
				break;
			case 'v':
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
			default:
				/* ignores unknown commands */
				break;
		}
	}
	return 0;
}
