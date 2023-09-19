#include <stdio.h>
//#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define COMMAND_LENGTH 20
#define MAX_CAR 512
#define STOP_SIGNAL 0

typedef struct station{
    int location;
    int car_number;
    int *cars;
    int max_distance;
    struct station *next;
    struct station *prev;
} station;


void addStation(station *first, int locationToAdd, int numOfCarsToAdd,int max_car, int carsToAdd[512]);

void deleteStation(int location, station *scanStation);

void addCar(station *scanStation, int location, int distance);

int main(){

    station *firstStation = NULL;

    FILE *file_in;

    file_in = freopen("archivio_test_aperti/open_2.txt", "r", stdin);

    if(file_in == NULL)
        return 2;


    char input[COMMAND_LENGTH];
    int location, numOfCars, distance, arrival;
    int tripDistance[MAX_CAR];
    int max_car = 0;

    while (scanf("%s", input) != EOF) {
        if (strcmp(input, "aggiungi-stazione") == 0) {
            if (scanf("%d %d", &location, &numOfCars) != EOF) {
                for (int i = 0; i < numOfCars; i++){
                    scanf("%d", &tripDistance[i]);
                    if(*(tripDistance+i)>max_car)
                        max_car = *(tripDistance+i);
                }
                addStation(firstStation, location, numOfCars,max_car, tripDistance);

            }
        } else if (strcmp(input, "demolisci-stazione") == 0) {
            if (scanf("%d", &location) != EOF) {
                deleteStation(location,firstStation);
            }
        } else if (strcmp(input, "aggiungi-auto") == 0) {
            if (scanf("%d %d", &location, &distance)) {
                addCar(firstStation, location, distance);
            }
        } else if (strcmp(input, "rottama-auto") == 0) {
            if (scanf("%d %d", &location, &distance)) {
                destroyCar(location, distance, firstStation);
            }
        } else if (strcmp(input, "pianifica-percorso") == 0) {
            if (scanf("%d %d", &distance, &arrival)) {
                planRoute( distance, arrival,&firstStation);
            }
        }
    }
    return 0;
}

void addCar(station *scanStation, int location, int distance) {
    while(scanStation->next != NULL ){
        if(scanStation->location == location){
            (scanStation->car_number + scanStation->car_number + 1) = distance;
            for (int i = 0; i < scanStation->car_number; i++){
                if( )
                    max_car = *(tripDistance+i);
            }
            return;
        }
        scanStation = scanStation->next;
    }
    printf("non rottamata\n");
}

void deleteStation(int location, station *scanStation) {
    while(scanStation->next != NULL ){
        if(scanStation->location == location){
            station *tmp = scanStation->next;
            tmp->prev = scanStation->prev;
            printf("demolita\n");
            free(scanStation);
            return;
        }
        scanStation = scanStation->next;
    }
    printf("non demolita\n");
}

void addStation(station *first, int locationToAdd, int numOfCarsToAdd, int max_car, int *carsToAdd) {
    if(first == NULL){
        first = malloc(sizeof(station));
        first->location = locationToAdd;
        first->car_number = numOfCarsToAdd;
        first->max_distance = max_car;
        first->cars = carsToAdd;
        first->next = NULL;
        first->prev = NULL;
        return;
    }
    if(first->location > locationToAdd){
        station *tmp = malloc(sizeof (station));
        tmp->location = locationToAdd;
        tmp->car_number = numOfCarsToAdd;
        tmp->cars = carsToAdd;
        tmp->max_distance = max_car;
        tmp->prev = NULL;
        first->prev = tmp;
        tmp->next = first;
        first = tmp;
        return;
    }
    else{
        station *scanStation = first;
        while(scanStation->next != NULL ){
            if(scanStation->location == locationToAdd){
                printf("non aggiunta");
                return;
            }
            if(scanStation->location > locationToAdd){
                station *tmp = malloc(sizeof (station));
                tmp->location = locationToAdd;
                tmp->car_number = numOfCarsToAdd;
                tmp->cars = carsToAdd;
                tmp->max_distance = max_car;
                tmp->prev = scanStation->prev;
                tmp->next = scanStation;
                printf("aggiunta");
                return;
            }
            scanStation = scanStation->next;
        }
    }
}







