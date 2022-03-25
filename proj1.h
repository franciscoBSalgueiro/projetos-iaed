/*
 * File:  proj1.h
 * Author:  Francisco Salgueiro
 * Description: Constants, structs and prototypes for proj1.c
 */

/* CONSTANTS */
#define MAX_FLIGHTS 30000
#define MAX_AIRPORTS 40

#define AIRPORT_ID_LENGTH 4
#define FLIGHT_ID_LENGTH 7
#define MAX_COUNTRY_NAME_LENGTH 31
#define MAX_CITY_NAME_LENGTH 51

#define MIN_CAPACITY 10
#define MAX_CAPACITY 100

static const int DAYS_BY_MONTH[12] = {31, 28, 31, 30, 31, 30,
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
#define DATE_FORMAT "%d-%d-%d"
#define TIME_FORMAT "%d:%d"

/* STRUCTS */
typedef struct Airport {
	char id[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];
} Airport;

typedef struct Date {
	int day;
	int month;
	int year;
} Date;

typedef struct Time {
	int hours;
	int minutes;
} Time;

typedef struct Flight {
	char id[FLIGHT_ID_LENGTH];
	Airport departure;
	Airport arrival;
	Date departure_date;
	Time departure_time;
	Time duration;
	Date arrival_date;
	Time arrival_time;
	int capacity;
} Flight;

typedef struct Global_State {
	int airports_count;
	int flights_count;
	Airport airports[MAX_AIRPORTS];
	Flight flights[MAX_FLIGHTS];
	Flight* sorted_flights_dep[MAX_FLIGHTS];
	Flight* sorted_flights_arr[MAX_FLIGHTS];
	Date date;
} Global_State;

/* Function Prototypes */
int commands(Global_State* global);
int add_airport(Global_State* global);
int add_flight(Global_State* global);
int list_airports(Global_State* global);
int list_flights(Global_State* global, char mode);
int isvalid_flight_id(char* id);
int get_flight(Global_State* global, char* id, Date date);
int get_num_flights(Global_State* global, char* id);
int get_airport(Global_State* global, char* id);
int change_date(Global_State* global);
int compare_dates(Date date1, Date date2);
int compare_time(Time time1, Time time2);
int compare_date_and_time(Date date1, Date date2, Time time1, Time time2);
void init_time(Time* time, int hours, int minutes);
void init_date(Date* date, int day, int month, int year);
void init_flight(Flight* flight, char* id, Airport departure, Airport arrival,
				 Date departure_date, Time departure_time, Time duration,
				 Date arrival_date, Time arrival_time, int capacity);
void init_airport(Airport* airport, char* id, char* country, char* city);
void read_date(Date* date);
void read_time(Time* time);
void read_airport(Airport* airport);
void print_date(Date date);
void print_time(Time time);
Time sum_time(Time time, Time duration);
Date increment_date(Date date);
int flight_error_handler(Global_State* global, char* flight_id,
						 Date departure_date, char* arrival_id,
						 char* departure_id);