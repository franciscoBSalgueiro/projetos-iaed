/*
 * File:  proj1.c
 * Author:  Francisco Salgueiro
 * Description: A program for creating airports and flights
 */

#include "proj1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	/* Set initial state */
	Global_State global;
	Date initial_date = {1, 1, 2022};

	global.airports_count = 0;
	global.flights_count = 0;
	global.date = initial_date;

	while (commands(&global)) {
	}
	return 0;
}

/* handles commands, returns 0 to quit program */
int commands(Global_State* global) {
	char c;
	c = getchar();
	switch (c) {
		case 'q':
			return 0;
		case 'a':
			add_airport(global);
			return 1;
		case 'l':
			list_airports(global);
			return 1;
		case 'v':
			add_flight(global);
			return 1;
		case 'p':
			list_flights(global, 'p');
			return 1;
		case 'c':
			list_flights(global, 'c');
			return 1;
		case 't':
			change_date(global);
			return 1;
		default:
			return 1;
	}
}