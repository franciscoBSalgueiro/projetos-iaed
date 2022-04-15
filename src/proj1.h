/*
 * File:  proj1.h
 * Author:  Francisco Salgueiro
 * Description: Constants, globals, structs and prototypes for proj1.c
 */

/*----------------------
 |  CONSTANTS & STRINGS  |
 -----------------------*/

#define TRUE 1
#define FALSE 0
#define ERROR -1

#define MAX_CMD_LEN 65535

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

/* COMMANDS */
#define QUIT_CMD 'q'
#define ADD_AIRPORT_CMD 'a'
#define LIST_AIRPORTS_CMD 'l'
#define ADD_FLIGHT_CMD 'v'
#define LIST_DEPARTURES_CMD 'p'
#define LIST_ARRIVALS_CMD 'c'
#define CHANGE_DATE_CMD 't'
#define LIST_RESERVES_CMD 'r'
#define DELETE_RESERVE_CMD 'e'

/* ERRORS */
#define INVALID_AIRPORT_ID "invalid airport ID\n"
#define INVALID_DATE "invalid date\n"
#define INVALID_FLIGHT "invalid flight code\n"
#define INVALID_DURATION "invalid duration\n"
#define INVALID_CAPACITY "invalid capacity\n"
#define INVALID_RESERVE "invalid reservation code\n"
#define INVALID_PASSENGER "invalid passager number\n"
#define TOO_MANY_AIPORTS "too many airports\n"
#define TOO_MANY_FLIGHTS "too many flights\n"
#define TOO_MANY_RESERVES "too many reservations\n"
#define DUPLICATE_AIRPORT "duplicate airport\n"
#define NO_SUCH_AIRPORT "%s: no such airport ID\n"
#define NO_SUCH_FLIGHT "%s: flight does not exist\n"
#define FLIGHT_ALREADY_EXISTS "flight already exists\n"
#define RESERVE_ALREADY_EXISTS "%s: flight reservation already used\n"
#define NOT_FOUND "not found\n"

/* MESSAGES */
#define AIRPORT_STRING "%s %s %s %d\n"
#define FLIGHT_STRING_REDUCED "%s %s "
#define FLIGHT_STRING "%s %s %s "
#define RESERVE_STRING "%s %d\n"
#define AIRPORT_ADDED_MESSAGE "airport %s\n"

/* ARGUMENT FORMATS */
#define IN_AIRPORT_FORMAT "%s %s %[^\n]"
#define IN_FLIGHT_FORMAT "%s %s %s"
#define IN_DATE_FORMAT "%d-%d-%d"
#define OUT_DATE_FORMAT "%02d-%02d-%d"
#define IN_TIME_FORMAT "%d:%d"
#define OUT_TIME_FORMAT "%02d:%02d"

/*--------------
 |   STRUCTS 	|
 ---------------*/


typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

typedef struct List {
    ListNode* head;
    ListNode* tail;
    int size;
} List;

/* Airport struct */
typedef struct {
	char id[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];
} Airport;

/* Date struct */
typedef struct {
	int day;
	int month;
	int year;
} Date;

/* Time struct */
typedef struct {
	int hours;
	int minutes;
} Time;

/* Flight struct */
typedef struct {
	char id[FLIGHT_ID_LENGTH];
	Airport* departure;
	Airport* arrival;
	Date dep_date;
	Time dep_time;
	Time duration;
	Date arr_date;
	Time arr_time;
	int capacity;
	List reserves;
	int taken_seats;
} Flight;

/* Time struct */
typedef struct {
	char* id;
	int passengers;
} Reserve;

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
	Flight* dep_flights[MAX_FLIGHTS]; /* Sorted by departure date */
	Flight* arr_flights[MAX_FLIGHTS]; /* Sorted by arrival date */

	/* Flag to avoid sorting if no changes were made to the arrays */
	int is_dep_sorted, is_arr_sorted;

	/* Current system date */
	Date date;
} System;

extern System gbsystem;

/*----------------------
 | FUNCTION PROTOTYPES	|
 -----------------------*/

/* proj1.c */
int cmd_triage();

/* commands.c */
void add_airport();
void list_airports();
void add_flight();
void list_all_flights();
void list_departures();
void list_arrivals();
void list_flights(Flight* arr[], char* (*airport_key_in)(Flight*),
				  char* (*airport_key_out)(Flight*), Date* (*date_key)(Flight*),
				  Time* (*time_key)(Flight*));
void change_date();
void list_reserves();
void add_reserve(Flight* flight);
void delete_reserve();

/* auxiliary.c */
int get_airport(char id[]);
int get_flight(char id[], Date* date);
void get_all_flights(List* l, char id[]);
int delete_flight(int index);
void list_flight_reserves(Flight* flight);
int get_num_flights(char* id);
int isvalid_flight_id(char* id);
int isvalid_airport_id(char* id);
int isvalid_reserve_id(char* id);
int res_id_already_exists(char reserve_id[]);

void init_time(Time* time, int hours, int minutes);
void init_date(Date* date, int day, int month, int year);
void init_airport(Airport* airport, char* id, char* country, char* city);
void init_flight(Flight* flight, char* id, Airport* departure, Airport* arrival,
				 Date* dep_date, Time* dep_time, Time* duration, int capacity);

int cmp_date(Date* date1, Date* date2);
int cmp_time(Time* time1, Time* time2);
int cmp_date_time(Date* date1, Date* date2, Time* time1, Time* time2);
void calculate_arrival(Flight* flight);
int isvalid_date(Date* date);

Date increment_date(Date date);
Time sum_time(Time* time, Time* duration);

int read_flight(Flight* new_flight);
void read_date(Date* date);
void read_time(Time* time);

void print_airport(Airport* airport);
void print_flight(char* id, char* airport_id, Date* date, Time* time);
void print_flight_full(Flight* flight);
void print_date(Date* date);
void print_time(Time* time);
void print_reserve(Reserve* reserve);

int has_error_flight(char* flight_id, Date* dep_date, char* arrival_id,
					 char* departure_id, Time duration, int capacity);

void insertion_sort(int is_sorted, Flight* arr[], Date* (*date_key)(Flight*),
					Time* (*time_key)(Flight*));
void sort_arrivals();
void sort_departures();

char* dep_id_key(Flight* flight);
char* arr_id_key(Flight* flight);
Date* dep_date_key(Flight* flight);
Date* arr_date_key(Flight* flight);
Time* dep_time_key(Flight* flight);
Time* arr_time_key(Flight* flight);

int is_lower(char s);
int is_upper(char s);
int is_digit(char s);

void* custom_alloc(long unsigned int size);
void clear_memory();

void list_init(List* list);
void list_add(List* list, void* data);
void list_insert(List* list, void* data, int index);
void list_remove(List* list, int index);
void* list_get(List* list, int index);
void list_destroy(List* list);