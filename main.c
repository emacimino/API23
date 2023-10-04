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

typedef struct path{
    struct station *location;
    struct path *next;
    struct path *prev;
}path;

void addStation(station *first, int locationToAdd, int numOfCarsToAdd,int max_car, int carsToAdd[512]);

void deleteStation(int location, station *scanStation);

void addCar(station *scanStation, int location, int distance);

void destroyCar(int location, int distance, station *scanStation);

void planRoute(int distance, int arrival, station *route);

void initStructure(station *firstStation);

void goOnRoute(int distance, int arrival, station *firstStation);

void goBackRoute(int distance, int arrival, station *firstStation);

int main(){

    station firstStation;
    initStructure(&firstStation);
    station* pFirst = &firstStation;
    FILE *file_in;
    FILE *file_out;
    file_in = freopen("archivio_test_aperti/open_2.txt", "r", stdin);
    file_out = freopen("outMio.txt","w",stdout);

    if(file_in == NULL)
        return 2;


    char input[COMMAND_LENGTH];
    int location, numOfCars, distance, arrival;
    int tripDistance[MAX_CAR];


    while (scanf("%s", input) != EOF) {
        if (strcmp(input, "aggiungi-stazione") == 0) {
            if (scanf("%d %d", &location, &numOfCars) != EOF) {
                int max_car = 0;
                for (int i = 0; i < numOfCars; i++){
                    scanf("%d", &tripDistance[i]);
                    if(*(tripDistance+i)>max_car)
                        max_car = *(tripDistance+i);
                }
                addStation(pFirst, location, numOfCars,max_car, tripDistance); //done, to debug
                if(pFirst->prev != NULL){
                    pFirst = pFirst->prev;
                }

            }
        }
        else if (strcmp(input, "demolisci-stazione") == 0) {
            if (scanf("%d", &location) != EOF) {
                deleteStation(location,pFirst); //done, to debug
            }
        }
        else if (strcmp(input, "aggiungi-auto") == 0) {
            if (scanf("%d %d", &location, &distance)) {
                addCar(pFirst, location, distance); //done, to debug
            }
        } else if (strcmp(input, "rottama-auto") == 0) {
            if (scanf("%d %d", &location, &distance)) {
                destroyCar(location, distance, pFirst); //done, to debug
            }
        } else if (strcmp(input, "pianifica-percorso") == 0) {
            if (scanf("%d %d", &distance, &arrival)) {
                planRoute( distance, arrival,pFirst);
            }
        }
    }
    return 0;
}

void initStructure(station *firstStation) {
    firstStation->next = NULL;
    firstStation->prev = NULL;
    firstStation->location = 0;
    firstStation->car_number = 0;
    firstStation->cars = calloc(512,sizeof (int));
    firstStation->max_distance = 0;
}

void planRoute(int distance, int arrival, station *firstStation) {
    if(distance < arrival)
        goOnRoute(distance, arrival, firstStation);
    else
        goBackRoute(distance, arrival, firstStation);
}

void goBackRoute(int distance, int arrival, station *firstStation) {
    station* route = firstStation;
    //va creata la lista parallela sulla quale operare (vedi path), per poi implementare dijkstra
    while(route->location != arrival)
        route = route->next;
    while (route->location != distance){
        route = route->next;

        }

    }


void goOnRoute(int distance, int arrival, station *firstStation) {
    //si potrebbe migliorare contando le stazioni e creando un array di appoggio invece di fare le malloc
    station* route = firstStation;
    int numOfStations = 1;
    while(route->location != distance)
        route = route->next;
    while (route->location != arrival){
        route = route->next;
        numOfStations++;
    }
    int *path = calloc(numOfStations, sizeof(int));
    route = route->prev;
    int tmpNumOfStation = numOfStations-1;
    path[tmpNumOfStation] = arrival;
    while(distance < arrival) {
        tmpNumOfStation--;
        while (route->location + route->max_distance >= arrival && route->location >= distance) {
            path[tmpNumOfStation] = route->location;
            route = route->prev;
        }
        if(path[tmpNumOfStation] == 0){
            printf("nessun percorso\n");
            return;
            }
        arrival = route->next->location;
    }
for(int i = tmpNumOfStation; i<numOfStations-1;i++)
    printf("%d ",path[i]);
printf("%d",path[numOfStations-1]);
free(path);
printf("\n");
}

void destroyCar(int location, int distance, station *scanStation) {
    bool destroyed = false;
    while(scanStation->next != NULL ){
        if(scanStation->location == location){
            for (int i = 0; i <= scanStation->car_number; i++){
                if(*(scanStation->cars + i) == distance && !destroyed){
                    *(scanStation->cars + i) = 0;
                    printf("rottamata\n");
                    destroyed = true;
                    return;
                }
                if(destroyed){
                    *(scanStation->cars + i) = *(scanStation->cars + i + 1);
                }
            }
            scanStation->car_number = scanStation->car_number-1;
        }
        scanStation = scanStation->next;
    }
    printf("non rottamata\n");
}

void addCar(station *scanStation, int location, int distance) { //seems to work
    while(scanStation != NULL ){
        if(scanStation->location == location){
            for (int i = 0; i <= scanStation->car_number; i ++){
                if(i== scanStation->car_number){
                    *(scanStation->cars + i) = distance;
                    scanStation->car_number++;
                    if(*(scanStation->cars+i)>scanStation->max_distance)
                        scanStation->max_distance = *(scanStation->cars+i);
                    printf("aggiunta\n");
                    return;
                }
            }
        }
        scanStation = scanStation->next;
    }
    printf("non aggiunta\n");
}

void deleteStation(int location, station *scanStation) {
    while(scanStation->next != NULL ){
        if(scanStation->location == location){
            station *tmp = scanStation;
            scanStation->next->prev = scanStation->prev;
            printf("demolita\n");
            free(tmp);
            return;
        }
        scanStation = scanStation->next;
    }
    printf("non demolita\n");
}

void addStation(station *first, int locationToAdd, int numOfCarsToAdd, int max_car, int *carsToAdd) { //seems to work
    if(first->location == 0){
        first->location = locationToAdd;
        first->car_number = numOfCarsToAdd;
        first->max_distance = max_car;
        first->cars = calloc(512,sizeof (int));
        if(first->car_number!=0)
            memccpy(first->cars,carsToAdd,numOfCarsToAdd,sizeof (int));
        first->next = NULL;
        first->prev = NULL;
        printf("aggiunta\n",stdout);
        return;
    }
    if(first->location > locationToAdd){
        station *tmp = malloc(sizeof (station));
        tmp->location = locationToAdd;
        tmp->car_number = numOfCarsToAdd;
        tmp->cars = calloc(512,sizeof (int));
        if(tmp->car_number!=0)
            memccpy(tmp->cars,carsToAdd,numOfCarsToAdd,sizeof (int));
        tmp->max_distance = max_car;
        tmp->prev = NULL;
        tmp->next = first;
        first->prev = tmp;
        printf("aggiunta\n",stdout);
        return;
    }
    else{
        station *tmp = malloc(sizeof (station));
        tmp->location = locationToAdd;
        tmp->car_number = numOfCarsToAdd;
        tmp->cars = calloc(512,sizeof (int));
        if(tmp->car_number!=0)
            memccpy(tmp->cars,carsToAdd,numOfCarsToAdd,sizeof (int));
        tmp->max_distance = max_car;

        while(first != NULL ){
            if(first->location == locationToAdd){
                printf("non aggiunta\n",stdout);
                return;
            }
            if(first->location > locationToAdd){
                tmp->next = first;
                tmp->prev = first->prev;
                first->prev->next = tmp;
                first->prev = tmp;
                printf("aggiunta\n",stdout);
                return;
            }
            if(first->next == NULL){
                first->next = tmp;
                first->next->prev = first;
                first->next->next = NULL;
                printf("aggiunta\n",stdout);
                return;
            }
            first = first->next;
        }
    }
}







