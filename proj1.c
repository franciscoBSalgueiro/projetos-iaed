#include "proj1.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	Global_State global;
    global.airports_count = 0;
	while (commands(&global)) { }
	return 0;
}

int commands(Global_State* global)
{
	char c = getchar();
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
		printf("alright\n");
		return 1;
	case 'p':
		printf("alright\n");
		return 1;
	case 'c':
		printf("alright\n");
		return 1;
	case 't':
		printf("alright\n");
		return 1;
	default:
		return 1;
	}
}

int add_airport(Global_State* global)
{
	char airportID[AIRPORT_ID_LENGTH];
	char country[MAX_COUNTRY_NAME_LENGTH];
	char city[MAX_CITY_NAME_LENGTH];
	Airport airport;

    if (global->airports_count==40) {
        printf(TOO_MANY_AIPORTS);
        return -1;
    }
    if (get_airport(global, airportID)) {
        printf(DUPLICATE_AIRPORT);
        return -2;
    }
	scanf("%s", airportID);
	scanf("%s", country);
	scanf(" %[^\n]", city);

	strcpy(airport.id, airportID);
	strcpy(airport.city, city);
	strcpy(airport.country, country);

	global->airports[global->airports_count] = airport;
    global->airports_count++;

	printf(AIRPORT_ADDED_MESSAGE, airportID);
	return 0;
}

int get_airport(Global_State* global, char* id) {
    int i;
    printf("%s",id);
    for (i = 0; i<global->airports_count; i++) {
        if (global->airports[i].id==id) {
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
