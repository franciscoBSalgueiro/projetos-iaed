/*
 * File:  auxiliary.c
 * Author:  Francisco Salgueiro
 * Description: Auxiliary functions
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proj2.h"

/*--------------------------
 |     MEMORY FUNCTIONS     |
 ---------------------------*/

/* Allocates memory and ends program with error code if no more memory is
 * available */
void* custom_alloc(long unsigned int size) {
	void* ptr = malloc(size);
	if (!ptr) {
		printf(NO_MEMORY);
		clear_memory();
		exit(1);
	}
	return ptr;
}

/* Frees all allocated memory */
void clear_memory() {
	int i;
	hashtable_destroy(gbsystem.reservation_ids);
	for (i = 0; i < gbsystem.flights_count; i++) {
		list_destroy(&gbsystem.flights[i].reservations);
	}
}

/*--------------------------
 |      GET FUNCTIONS       |
 ---------------------------*/

/* returns index of airport or -index-1 if it doesn't exist */
int get_airport(char id[]) {
	int left, right, middle, cmp;
	left = 0;
	right = gbsystem.airports_count - 1;
	middle = (left + right) / 2;
	while (left <= right) {
		cmp = strcmp(gbsystem.airports[middle].id, id);
		if (cmp < 0)
			left = middle + 1;
		else if (cmp == 0)
			return middle;
		else
			right = middle - 1;
		middle = (left + right) / 2;
	}
	return -left - 1;
}

/* returns index of flight or -1 if it doesn't exist */
int get_flight(char id[], Date* date) {
	int i;
	for (i = 0; i < gbsystem.flights_count; i++)
		if (strcmp(gbsystem.flights[i].id, id) == 0 &&
			cmp_date(&gbsystem.flights[i].dep_date, date) == 0) {
			return i;
		}
	return -1;
}

/* returns number of flights by airport */
int get_num_flights(char* id) {
	int i, count = 0;
	for (i = 0; i < gbsystem.flights_count; i++)
		if (strcmp(gbsystem.flights[i].departure->id, id) == 0) count++;
	return count;
}

/*--------------------------
 |    DELETE FUNCTIONS       |
 ---------------------------*/

/* Removes flight pointer from ordered flight arrays */
void array_remove_flight(Flight* arr[], Flight* flight) {
	int i;
	for (i = 0; i < gbsystem.flights_count; i++) {
		if (arr[i] == flight) {
			memcpy(&arr[i], &arr[i + 1],
				   (gbsystem.flights_count - i) * sizeof(Flight*));
			return;
		}
	}
}

/* Deletes reservation with given id in flight */
void delete_reservation_id(char* id, Flight* f) {
	Reservation* r;
	List* lr;

	ListNode *node, *prev;
	lr = &f->reservations;

	for (node = lr->head, prev = NULL; node != NULL;
		 prev = node, node = node->next) {
		r = (Reservation*)node->data;
		if (strcmp(r->id, id) == 0) {
			f->taken_seats -= r->passengers;
			hashtable_remove(gbsystem.reservation_ids, r);
			list_remove(lr, node, prev);
			break;
		}
	}
}

/* Remove flight from system lists */
int delete_flight(int index) {
	Flight* flight = &gbsystem.flights[index];
	ListNode* n;

	for (n = flight->reservations.head; n != NULL; n = n->next)
		hashtable_remove(gbsystem.reservation_ids, n->data);
	list_destroy(&flight->reservations);

	gbsystem.flights_count--;

	array_remove_flight(gbsystem.dep_flights, flight);
	array_remove_flight(gbsystem.arr_flights, flight);

	if (index < gbsystem.flights_count) {
		memcpy(&gbsystem.flights[index], &gbsystem.flights[index + 1],
			   (gbsystem.flights_count - index) * sizeof(Flight));
	}

	return 0;
}

/*--------------------------
 |  VALIDATION FUNCTIONS	|
 ---------------------------*/

/* Checks if the first two letters are uppercase and the rest are digits */
int isvalid_flight_id(char* id) {
	unsigned int i, l;
	l = strlen(id);
	if (l < 3 || l > 6 || !is_upper(id[0]) || !is_upper(id[1])) return FALSE;
	for (i = 2; i < l; i++)
		if (!is_digit(id[i])) return FALSE;
	return TRUE;
}

/* Checks if there are no lowercase letters in the id */
int isvalid_airport_id(char* id) {
	unsigned int i, l = strlen(id);
	for (i = 0; i < l; i++)
		if (is_lower(id[i])) return FALSE;
	return TRUE;
}

/* Checks if string is longer than 10 char and
 * contains only digits or uppercase letters */
int isvalid_reservation_id(char* id) {
	unsigned int i, l;
	l = strlen(id);
	if (l < 10) return FALSE;
	for (i = 0; i < l; i++)
		if (!(is_digit(id[i]) || is_upper(id[i]))) return FALSE;
	return TRUE;
}

/*--------------------------
 |  INITIALIZING FUNCTIONS	|
 ---------------------------*/

/* Initializes values for the Time struct */
void init_time(Time* time, int hours, int minutes) {
	time->hours = hours;
	time->minutes = minutes;
}

/* Initializes values for the Date struct */
void init_date(Date* date, int day, int month, int year) {
	date->day = day;
	date->month = month;
	date->year = year;
}

/* Initializes values for the Airport struct */
void init_airport(Airport* airport, char* id, char* country, char* city) {
	strcpy(airport->id, id);
	strcpy(airport->country, country);
	strcpy(airport->city, city);
}

/* Initializes values for the Flight struct */
void init_flight(Flight* flight, char* id, Airport* departure, Airport* arrival,
				 Date* dep_date, Time* dep_time, Time* duration, int capacity) {
	strcpy(flight->id, id);
	flight->departure = departure;
	flight->arrival = arrival;
	flight->dep_date = *dep_date;
	flight->dep_time = *dep_time;
	flight->duration = *duration;
	flight->capacity = capacity;
	list_init(&flight->reservations);
	flight->taken_seats = 0;
}

/*----------------------
 |  COMPARISON FUNCTIONS	|
 -----------------------*/

/* Returns -1 if date1 happens before date2, 0 if the dates are equal and 1 if
 * date1 happens after date2 */
int cmp_date(Date* date1, Date* date2) {
	if (date1->year < date2->year) return -1;
	if (date1->year > date2->year) return 1;
	if (date1->month < date2->month) return -1;
	if (date1->month > date2->month) return 1;
	if (date1->day < date2->day) return -1;
	if (date1->day > date2->day) return 1;
	return 0;
}

/* Returns -1 if time1 happens before time2, 0 if the dates are equal and 1 if
 * time1 happens after time2 */
int cmp_time(Time* time1, Time* time2) {
	if (time1->hours < time2->hours) return -1;
	if (time1->hours > time2->hours) return 1;
	if (time1->minutes < time2->minutes) return -1;
	if (time1->minutes > time2->minutes) return 1;
	return 0;
}

/* Combines the two previous functions */
int cmp_date_time(Date* date1, Date* date2, Time* time1, Time* time2) {
	int d, t;
	if ((d = cmp_date(date1, date2)) < 0) return -1;
	if (d > 0) return 1;
	if ((t = cmp_time(time1, time2)) < 0) return -1;
	if (t > 0) return 1;
	return 0;
}

/*--------------------------
 |   DATE & TIME FUNCTIONS	|
 ---------------------------*/

/* Return date incremented by one day */
Date increment_date(Date date) {
	date.day++;
	if (date.day > MONTH_DAYS[date.month - 1]) {
		date.day = 1;
		date.month++;
		if (date.month > 12) {
			date.month = 1;
			date.year++;
		}
	}
	return date;
}

/* Returns the sum of two times */
Time sum_time(Time* time1, Time* time2) {
	Time sum;
	sum.hours = time1->hours + time2->hours;
	sum.minutes = time1->minutes + time2->minutes;
	if (sum.minutes >= NUM_MINUTES) {
		sum.hours++;
		sum.minutes -= NUM_MINUTES;
	}
	return sum;
}

/* Calculates the arrival date and time of a flight */
void calculate_arrival(Flight* flight) {
	flight->arr_time = sum_time(&flight->dep_time, &flight->duration);
	if (flight->arr_time.hours >= NUM_HOURS) {
		flight->arr_time.hours -= NUM_HOURS;
		flight->arr_date = increment_date(flight->dep_date);
	} else {
		flight->arr_date = flight->dep_date;
	}
}

/* Checks if date isn't in the past or more than one year in the future */
int isvalid_date(Date* date) {
	Date future_date;
	future_date = gbsystem.date;
	future_date.year++;
	return !(cmp_date(date, &gbsystem.date) < 0 ||
			 cmp_date(date, &future_date) > 0);
}

/*----------------------
 |    READ FUNCTIONS	|
 -----------------------*/

/* Reads all the flight data from stdin and puts it in new_flight */
int read_flight(Flight* new_flight) {
	char flight_id[FLIGHT_ID_LENGTH];
	char departure_id[AIRPORT_ID_LENGTH];
	char arrival_id[AIRPORT_ID_LENGTH];
	Date dep_date;
	Time dep_time, duration;
	int capacity;
	Airport *departure, *arrival;

	scanf(IN_FLIGHT_FORMAT, flight_id, departure_id, arrival_id);
	read_date(&dep_date);
	read_time(&dep_time);
	read_time(&duration);
	scanf("%d", &capacity);

	if (has_error_flight(flight_id, &dep_date, arrival_id, departure_id,
						 duration, capacity))
		return ERROR;

	departure = &gbsystem.airports[get_airport(departure_id)];
	arrival = &gbsystem.airports[get_airport(arrival_id)];
	init_flight(new_flight, flight_id, departure, arrival, &dep_date, &dep_time,
				&duration, capacity);
	return 0;
}

/* Reads day, month and year from stdin and puts them into date */
void read_date(Date* date) {
	int day, month, year;

	scanf(IN_DATE_FORMAT, &day, &month, &year);
	init_date(date, day, month, year);
}

/* Reads minutes and hours from stdin and puts them into time */
void read_time(Time* time) {
	int minutes, hours;

	scanf(IN_TIME_FORMAT, &hours, &minutes);
	init_time(time, hours, minutes);
}

/*----------------------
 |  PRINT FUNCTIONS		|
 -----------------------*/

/* Prints airport in formatted form */
void print_airport(Airport* airport) {
	int num_flights = get_num_flights(airport->id);
	printf(AIRPORT_STRING, airport->id, airport->city, airport->country,
		   num_flights);
}

/* Prints flight in formatted form for the c and p commands */
void print_flight(char* flight_id, char* airport_id, Date* date, Time* time) {
	printf(FLIGHT_STRING_REDUCED, flight_id, airport_id);
	print_date(date);
	putchar(' ');
	print_time(time);
	putchar('\n');
}

/* Prints flight in formatted form for the v command */
void print_flight_full(Flight* flight) {
	printf(FLIGHT_STRING, flight->id, flight->departure->id,
		   flight->arrival->id);
	print_date(&flight->dep_date);
	putchar(' ');
	print_time(&flight->dep_time);
	putchar('\n');
}

/* Prints date in formatted form */
void print_date(Date* date) {
	printf(OUT_DATE_FORMAT, date->day, date->month, date->year);
}

/* Prints time in formatted form */
void print_time(Time* time) {
	printf(OUT_TIME_FORMAT, time->hours, time->minutes);
}

void print_reservation(Reservation* reservation) {
	printf(RESERVATION_STRING, reservation->id, reservation->passengers);
}

/* Lists all the reservations from a flight */
void list_flight_reservations(Flight* flight) {
	ListNode* n;
	if (!isvalid_date(&flight->dep_date)) {
		printf(INVALID_DATE);
		return;
	}

	list_sort(&flight->reservations);

	for (n = flight->reservations.head; n != NULL; n = n->next) {
		print_reservation((Reservation*)n->data);
	}
}

/*----------------------
 |    ERROR FUNCTIONS	|
 -----------------------*/

/* Handles all errors for add_reservation */
int has_error_reservation(int passengers, Flight* flight,
						  char* reservation_id) {
	if (!isvalid_reservation_id(reservation_id)) {
		printf(INVALID_RESERVATION);
		return ERROR;
	}

	if (hashtable_contains(gbsystem.reservation_ids, reservation_id)) {
		printf(RESERVATION_ALREADY_EXISTS, reservation_id);
		return ERROR;
	}

	if (flight->taken_seats + passengers > flight->capacity) {
		printf(TOO_MANY_RESERVATIONS);
		return ERROR;
	}

	if (!isvalid_date(&flight->dep_date)) {
		printf(INVALID_DATE);
		return ERROR;
	}

	if (passengers < 1) {
		printf(INVALID_PASSENGER);
		return ERROR;
	}
	return 0;
}

/* Handles all errors for add_flight */
int has_error_flight(char* flight_id, Date* dep_date, char* arrival_id,
					 char* departure_id, Time duration, int capacity) {
	int t;

	if (!isvalid_flight_id(flight_id)) return printf(INVALID_FLIGHT);

	if (get_flight(flight_id, dep_date) >= 0)
		return printf(FLIGHT_ALREADY_EXISTS);

	if ((t = get_airport(departure_id)) < 0 || get_airport(arrival_id) < 0)
		return printf(NO_SUCH_AIRPORT, t < 0 ? departure_id : arrival_id);

	if (gbsystem.flights_count == MAX_FLIGHTS) return printf(TOO_MANY_FLIGHTS);

	if (!isvalid_date(dep_date)) return printf(INVALID_DATE);

	if (duration.hours > MAX_DURATION ||
		(duration.hours == MAX_DURATION && duration.minutes > 0))
		return printf(INVALID_DURATION);

	if (capacity < MIN_CAPACITY) return printf(INVALID_CAPACITY);
	return FALSE;
}

/*----------------------
 |   SORTING FUNCTIONS	|
 -----------------------*/

/* Sorts flights by departure or arrival date and time */
void insertion_sort(int* is_sorted, Flight* arr[], Date* (*date_key)(Flight*),
					Time* (*time_key)(Flight*)) {
	int i, j;
	Flight* temp;

	if (*is_sorted) return;
	*is_sorted = TRUE;

	for (i = 1; i < gbsystem.flights_count; i++) {
		temp = arr[i];
		for (j = i - 1;
			 j >= 0 && cmp_date_time(date_key(arr[j]), date_key(temp),
									 time_key(arr[j]), time_key(temp)) > 0;
			 j--) {
			arr[j + 1] = arr[j];
		}
		arr[j + 1] = temp;
	}
}

/*----------------------
 |     KEY FUNCTIONS	|
 -----------------------*/

char* dep_id_key(Flight* flight) { return flight->departure->id; }
char* arr_id_key(Flight* flight) { return flight->arrival->id; }
Date* dep_date_key(Flight* flight) { return &flight->dep_date; }
Date* arr_date_key(Flight* flight) { return &flight->arr_date; }
Time* dep_time_key(Flight* flight) { return &flight->dep_time; }
Time* arr_time_key(Flight* flight) { return &flight->arr_time; }
char* res_key(void* data) { return ((Reservation*)data)->id; }

/*----------------------
 |       C TYPES      	|
 -----------------------*/

int is_lower(char c) { return c >= 'a' && c <= 'z'; }
int is_upper(char c) { return c >= 'A' && c <= 'Z'; }
int is_digit(char c) { return c >= '0' && c <= '9'; }