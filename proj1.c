#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proj1.h"

int add_airport(Airport *aeroportos)
{
    char airportID[4];
    char country[31];
    char city[51];
    Airport airport;

    scanf("%s",airportID);
    scanf("%s",country);
    scanf(" %[^\n]",city);

    strcpy(airport.id, airportID);
    strcpy(airport.city, city);
    strcpy(airport.country, country);

    aeroportos[0] = airport;

    printf(AIRPORT_ADDED_MESSAGE, airportID);
    return 0;
}

int list_airports(Airport *aeroportos)
{
    int i;
    Airport airport;
    for(i=0; i<MAX_AIRPORTS;i++) {
        airport=aeroportos[i];
        printf(AIRPORT_STRING, airport.id, airport.city, airport.country);
    }
    return 0;
}

int commands(Airport *aeroportos)
{
    char c = getchar();
    switch (c)
    {
    case 'q':
        return 0;
    case 'a':
        add_airport(aeroportos);
        return 1;
    case 'l':
        list_airports(aeroportos);
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

int main()
{
    Airport aeroportos[MAX_AIRPORTS];
    while (commands(aeroportos))
    {
    }
    return 0;
}
