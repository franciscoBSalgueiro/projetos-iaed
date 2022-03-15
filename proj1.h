#define MAX_FLIGHTS 30000
#define MAX_AIRPORTS 40

#define AIRPORT_ID_LENGTH 4
#define FLIGHT_ID_LENGTH 7
#define MAX_COUNTRY_NAME_LENGTH 31
#define MAX_CITY_NAME_LENGTH 51

/* ERRORS */
#define INVALID_AIRPORT_ID "invalid airport ID\n"
#define TOO_MANY_AIPORTS "too many airports\n"
#define DUPLICATE_AIRPORT "duplicate airport\n"

/* MESSAGES */
#define AIRPORT_STRING "%s %s %s\n"
#define AIRPORT_ADDED_MESSAGE "airport %s\n"

typedef struct Airport {
	char id[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];
} Airport;


typedef struct Date {
	int year;
	int month;
	int day;
} Date;

typedef struct Time {
	int hours;
	int minutes;
} Time;

typedef struct Flight {
	char id[FLIGHT_ID_LENGTH];
	Airport departure;
	Airport arrival;
	Date date;
	Time time;
	int duration;
	int passengers;
} Flight;


typedef struct Global_State {
    Airport airports[MAX_AIRPORTS];
    int airports_count;
    Flight flights[MAX_FLIGHTS];
    int flights_count;
    Date date;    
} Global_State;

int commands(Global_State* global);
int add_airport(Global_State* global);
int list_airports(Global_State* global);
int get_airport(Global_State* global, char* id);