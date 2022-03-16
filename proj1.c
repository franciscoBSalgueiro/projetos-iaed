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

	for (i = 0; i < strlen(airportID); i++) {
		if (!isupper((airportID[i]))) {
			printf(INVALID_AIRPORT_ID);
			return -1;
		}
	}
	if (global->airports_count == MAX_AIRPORTS) {
		printf(TOO_MANY_AIPORTS);
		return -1;
	}
	if (get_airport(global, airportID) >= 0) {
		printf(DUPLICATE_AIRPORT);
		return -1;
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
	char flight_id[FLIGHT_ID_LENGTH];
	char departure_id[AIRPORT_ID_LENGTH];
	char arrival_id[AIRPORT_ID_LENGTH];
	Date departure_date;
	Time departure_time;
	int day, month, year, hour, minutes;
	int duration;
	int capacity;
	Flight flight;
	char c;

	c = getchar();
	if (c == '\n') {
		list_flights(global);
		return 0;
	}
	scanf("%s", flight_id);
	scanf("%s", arrival_id);
	scanf("%s", departure_id);
	scanf("%d%*c", &day);
	scanf("%d%*c", &month);
	scanf("%d", &year);
	scanf("%d%*c", &hour);
	scanf("%d", &minutes);
	scanf("%d", &duration);
	scanf("%d", &capacity);
	departure_date.year = year;
	departure_date.month = month;
	departure_date.day = day;
	departure_time.hours = hour;
	departure_time.minutes = minutes;

	if (check_flight_id(flight_id)) {
		printf(INVALID_FLIGHT);
		return -1;
	}
	/* TODO FLIGHT ALREADY EXISTS */
	if (!(get_airport(global, arrival_id) >= 0)) {
		printf(NO_SUCH_AIRPORT, arrival_id);
		return -1;
	}
	if (!(get_airport(global, departure_id) >= 0)) {
		printf(NO_SUCH_AIRPORT, departure_id);
		return -1;
	}
	if (global->flights_count == MAX_FLIGHTS) {
		printf(TOO_MANY_FLIGHTS);
		return -1;
	}
	if (compare_dates(departure_date, global->date) < 0) {
		printf(INVALID_DATE);
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

	strcpy(flight.id, flight_id);
	flight.arrival = global->airports[get_airport(global, arrival_id)];
	flight.departure = global->airports[get_airport(global, departure_id)];
	flight.date = departure_date;
	flight.time = departure_time;
	flight.duration = duration;
	flight.capacity = capacity;

	global->flights[global->flights_count] = flight;
	global->flights_count++;

	return 0;
}

/* Returns -1 if date1 happens before date2, 0 if the dates are equal and 1 if
 * date1 happens after date2 */
int compare_dates(Date date1, Date date2)
{
	if (date1.year < date2.year) {
		return -1;
	}
	if (date1.year > date2.year) {
		return 1;
	}
	if (date1.month < date2.month) {
		return -1;
	}
	if (date1.month > date2.month) {
		return 1;
	}
	if (date1.day < date2.day) {
		return -1;
	}
	if (date1.day > date2.day) {
		return 1;
	}
	return 0;
}

void sort_airports(Airport airports[], int num_airports)
{
	int i, j;
	Airport temp;

	for (i = 0; i < num_airports - 1; i++) {
		for (j = 0; j < num_airports - 1 - i; j++) {
			if (strcmp(airports[j].id, airports[j + 1].id) > 0) {
				temp = airports[j];
				airports[j] = airports[j + 1];
				airports[j + 1] = temp;
			}
		}
	}
}

int check_flight_id(char* id)
{
	size_t i;
	size_t length_id;
	length_id = strlen(id);
	if (length_id < 3 || length_id > 6) {
		return 1;
	}
	if (id[0] < 'A' || id[0] > 'Z' || id[1] < 'A' || id[1] > 'Z') {
		return 1;
	}
	for (i = 2; i < length_id; i++) {
		if (id[i] < '0' || id[i] > '9') {
			return 1;
		}
	}
	return 0;
}

/* returns index of airport exists, otherwise returns -1 */
int get_airport(Global_State* global, char* id)
{
	int i;
	for (i = 0; i < global->airports_count; i++) {
		if (strcmp(global->airports[i].id, id) == 0) {
			return i;
		}
	}
	return -1;
}

int list_airports(Global_State* global)
{
	int i;
	char c;
	char airportID[AIRPORT_ID_LENGTH];
	Airport airport;

	c = getchar();
	if (c == '\n') {
		sort_airports(global->airports, global->airports_count);
		for (i = 0; i < global->airports_count; i++) {
			airport = global->airports[i];
			printf(AIRPORT_STRING, airport.id, airport.city, airport.country);
		}
	} else {
		while (scanf("%s", airportID) != EOF) {
			i = get_airport(global, airportID);
			if (i == -1) {
				printf(NO_SUCH_AIRPORT, airportID);
				return -1;
			}
			airport = global->airports[i];
			printf(AIRPORT_STRING, airport.id, airport.city, airport.country);
			if (getchar() == '\n') {
				break;
			}
		}
	}
	return 0;
}

int list_flights(Global_State* global)
{
	int i;
	Flight flight;
	for (i = 0; i < global->flights_count; i++) {
		flight = global->flights[i];
		printf(
			FLIGHT_STRING, flight.id, flight.departure.id, flight.arrival.id);
		print_date(flight.date);
		putchar(' ');
		print_time(flight.time);
		putchar('\n');
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

	return 0;
}

void print_date(Date date)
{
	printf("%02d-%02d-%04d", date.day, date.month, date.year);
}

void print_time(Time time) { printf("%02d:%02d", time.hours, time.minutes); }