/*
 * File:  proj1.h
 * Author:  Francisco Salgueiro
 * Description: Constants, globals, structs and prototypes for proj1.c
 */

/*----------------------
 |  CONSTANTS & MACROS  |
 -----------------------*/

#define TRUE 1
#define FALSE 0
#define ERROR -1

#define MAX_FLIGHTS 30000
#define MAX_AIRPORTS 40

#define AIRPORT_ID_LENGTH 4
#define FLIGHT_ID_LENGTH 7
#define MAX_COUNTRY_NAME_LENGTH 31
#define MAX_CITY_NAME_LENGTH 51

#define MIN_CAPACITY 10
#define MAX_CAPACITY 100
#define MAX_DURATION 12

#define NUM_MINUTES 60
#define NUM_HOURS 24
#define NUM_MONTHS 12

/* Default initial date */
#define INITIAL_DATE 1, 1, 2022

static const int MONTH_DAYS[NUM_MONTHS] = {31, 28, 31, 30, 31, 30,
										   31, 31, 30, 31, 30, 31};

/* ERRORS */
#define INVALID_AIRPORT_ID "invalid airport ID\n"
#define INVALID_DATE "invalid date\n"
#define INVALID_FLIGHT "invalid flight code\n"
#define INVALID_DURATION "invalid duration\n"
#define INVALID_CAPACITY "invalid capacity\n"
#define TOO_MANY_AIPORTS "too many airports\n"
#define TOO_MANY_FLIGHTS "too many flights\n"
#define DUPLICATE_AIRPORT "duplicate airport\n"
#define NO_SUCH_AIRPORT "%s: no such airport ID\n"
#define FLIGHT_ALREADY_EXISTS "flight already exists\n"

/* MESSAGES */
#define AIRPORT_STRING "%s %s %s %d\n"
#define FLIGHT_STRING_REDUCED "%s %s "
#define FLIGHT_STRING "%s %s %s "
#define AIRPORT_ADDED_MESSAGE "airport %s\n"

/* ARGUMENT FORMATS */
#define IN_AIRPORT_FORMAT "%s %s %[^\n]"
#define IN_DATE_FORMAT "%d-%d-%d"
#define OUT_DATE_FORMAT "%02d-%02d-%d"
#define IN_TIME_FORMAT "%d:%d"
#define OUT_TIME_FORMAT "%02d:%02d"

/*--------------
 |   STRUCTS 	|
 ---------------*/

typedef struct {
	char id[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];
} Airport;

typedef struct {
	int day;
	int month;
	int year;
} Date;

typedef struct {
	int hours;
	int minutes;
} Time;

typedef struct {
	char id[FLIGHT_ID_LENGTH];
	Airport* departure;
	Airport* arrival;
	Date departure_date;
	Time departure_time;
	Time duration;
	Date arrival_date;
	Time arrival_time;
	int capacity;
} Flight;

/*----------------------
 |     GLOBAL STATE		|
 -----------------------*/

typedef struct {
	/* Number of airports and flights added */
	int airports_count, flights_count;

	/* Airport array sorted by ID */
	Airport airports[MAX_AIRPORTS];

	/* Flight arrays */
	Flight flights[MAX_FLIGHTS];
	Flight* sorted_flights_dep[MAX_FLIGHTS]; /* Sorted by departure date */
	Flight* sorted_flights_arr[MAX_FLIGHTS]; /* Sorted by arrival date */

	/* Boolean to avoid sorting if no changes were made to the arrays */
	int is_departures_sorted, is_arrivals_sorted;

	/* Current system date */
	Date date;
} System;

extern System system;

/*----------------------
 | FUNCTION PROTOTYPES	|
 -----------------------*/

/* main.c */
int cmd_triage();

/* commands.c */
int add_airport();
int list_airports();
int add_flight();
void list_all_flights();
int list_flights(char mode);
int change_date();

/* auxiliary.c */
int get_airport(char id[]);
int get_flight(char id[], Date* date);
int get_num_flights(char* id);
int isvalid_flight_id(char* id);
int has_lowercase(char* str);

void init_time(Time* time, int hours, int minutes);
void init_date(Date* date, int day, int month, int year);
void init_airport(Airport* airport, char* id, char* country, char* city);
void init_flight(Flight* flight, char* id, Airport* departure, Airport* arrival,
				 Date* departure_date, Time* departure_time, Time* duration,
				 int capacity);

int compare_date(Date* date1, Date* date2);
int compare_time(Time* time1, Time* time2);
int compare_date_and_time(Date* date1, Date* date2, Time* time1, Time* time2);
void calculate_arrival(Flight* flight);

Date increment_date(Date date);
Time sum_time(Time* time, Time* duration);

void read_date(Date* date);
void read_time(Time* time);

void print_airport(Airport* airport);
void print_flight(char* id, char* airport_id, Date* date, Time* time);
void print_flight_full(Flight* flight);
void print_date(Date* date);
void print_time(Time* time);

int has_error_flight(char* flight_id, Date* departure_date, char* arrival_id,
					 char* departure_id, Time duration, int capacity);

void sort_arrivals();
void sort_departures();