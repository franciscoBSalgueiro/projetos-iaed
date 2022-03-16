#include "proj1.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	Global_State global;
	Date initial_date;

	initial_date.day = 1;
	initial_date.month = 1;
	initial_date.year = 2022;

	global.airports_count = 0;
	global.date = initial_date;
	while (commands(&global)) { }
	return 0;
}

int commands(Global_State* global)
{
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
		printf("alright\n");
		return 1;
	case 'c':
		printf("alright\n");
		return 1;
	case 't':
		change_date(global);
		return 1;
	default:
		return 1;
	}
}

int add_airport(Global_State* global)
{
	size_t i;
	char airportID[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];
	Airport airport;

	scanf("%s", airportID);
	scanf("%s", country);
	scanf(" %s[^\n]", city);

	if (global->airports_count == MAX_AIRPORTS) {
		printf(TOO_MANY_AIPORTS);
		return -1;
	}
	if (get_airport(global, airportID)) {
		printf(DUPLICATE_AIRPORT);
		return -2;
	}
	for (i = 0; i < strlen(airportID); i++) {
		if (!isupper((airportID[i]))) {
			printf(INVALID_AIRPORT_ID);
			return -3;
		}
	}

	strcpy(airport.id, airportID);
	strcpy(airport.city, city);
	strcpy(airport.country, country);

	global->airports[global->airports_count] = airport;
	global->airports_count++;

	printf(AIRPORT_ADDED_MESSAGE, airportID);
	return 0;
}

int add_flight(Global_State* global)
{
	char id[FLIGHT_ID_LENGTH];
	char departure_id[AIRPORT_ID_LENGTH];
	char arrival_id[AIRPORT_ID_LENGTH];
	int day, month, year, hour, minutes;
	int duration;
	int capacity;

	scanf("%s", id);
	scanf("%s", arrival_id);
	scanf("%s", departure_id);
	scanf("%d%*c", &day);
	scanf("%d%*c", &month);
	scanf("%d", &year);
	scanf("%d%*c", &hour);
	scanf("%d", &minutes);
	scanf("%d", &duration);
	scanf("%d", &capacity);

	if (global->flights_count == MAX_FLIGHTS) {
		printf(TOO_MANY_FLIGHTS);
		return -1;
	}
	if (duration > 12) {
		printf(INVALID_DURATION);
		return -1;
	}
	if (capacity < 10 || capacity > 100) {
		printf(INVALID_CAPACITY);
		return -1;
	}
	return 0;
}

int get_airport(Global_State* global, char* id)
{
	int i;
	for (i = 0; i < global->airports_count; i++) {
		if (strcmp(global->airports[i].id, id) == 0) {
			return 1;
		}
	}
	return 0;
}

int list_airports(Global_State* global)
{
	int i;
	Airport airport;
	for (i = 0; i < global->airports_count; i++) {
		airport = global->airports[i];
		printf(AIRPORT_STRING, airport.id, airport.city, airport.country);
	}
	return 0;
}

int change_date(Global_State* global)
{
	Date date;
	int year, month, day;

	scanf("%d%*c", &day);
	scanf("%d%*c", &month);
	scanf("%d", &year);

	date.year = year;
	date.month = month;
	date.day = day;

	/* TODO verify dates*/

	global->date = date;

	printf("%02d-%02d-%04d\n", day, month, year);
	return 0;
}