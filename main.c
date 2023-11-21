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
    int location;
    int max_dis;
    int dijkstra_succ;
    int dijkstra_pred;
    bool visited;
} path;

typedef struct dijkstraStruct{
    struct station* autogrill;
    struct station *next;
    struct station *prev;
} dijkstraStruct;

void addStation(station *first);

void deleteStation(int location, station *scanStation);

void addCar(station *scanStation, int location, int distance);

void destroyCar(int location, int distance, station *scanStation);

void planRoute(int distance, int arrival, station *firstStation);

void initStructure(station *firstStation);

void goOnRoute(int distance, int arrival, station *firstStation);

void goBackRoute(int distance, int arrival, station *firstStation);

int main(){

    station firstStation;
    initStructure(&firstStation);
    station* pFirst = &firstStation;
    FILE *file_in;
    FILE *file_out;
    file_in = freopen("cmake-build-release/archivio_test_aperti/open_49.txt", "r", stdin);
    file_out = freopen("outMio.txt","w",stdout);

    if(file_in == NULL)
        return 2;

    char input[COMMAND_LENGTH];
    int location, distance, arrival;

    while (scanf("%s", input) != EOF) {
        if (strcmp(input, "aggiungi-stazione") == 0) {
            addStation(pFirst); //done, to debug
            if(pFirst->prev != NULL){
                pFirst = pFirst->prev;
            }
        }
        else if (strcmp(input, "demolisci-stazione") == 0) {
            if (scanf("%d", &location) != EOF) {
                deleteStation(location,pFirst); //done, to debug
            }
            if(pFirst->location == -1234){
                station* tmp = pFirst;
                pFirst = pFirst->next;
                pFirst->prev = NULL;

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
    firstStation->location = -1;
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

    int numOfStations = 1;
    //esploro la lista per segnarmi il numero di stazioni interessate O(k)
    if(arrival < route->location){
        printf("nessun percorso\n");
        return;
    }
    while(route->location != arrival)
        route = route->next;

    while (route->location != distance){
        route = route->next;
        numOfStations++;
    }

    while (route->location != distance){
        route = route->next;
        numOfStations++;
        }
    path goBackFreeway[numOfStations];
    int tmpNumOfStation = numOfStations-1;


    //torno indietro e assegno ogni stazione a un cella di un array lungo quanto il percorso e inizio a operare su quell'array
    //con l'array ho una struttura di appoggio secondaria senza fare malloc
    while(route->location != arrival){
        goBackFreeway[tmpNumOfStation].location = route->location;
        goBackFreeway[tmpNumOfStation].max_dis = route->max_distance;
        goBackFreeway[tmpNumOfStation].dijkstra_succ = numOfStations; //equal to infinity
        goBackFreeway[tmpNumOfStation].dijkstra_pred = numOfStations; //like null, here we will store the index of predecessors
        goBackFreeway[tmpNumOfStation].visited = false;
        tmpNumOfStation--;
        route = route->prev;
        }
    goBackFreeway[0].location = route->location;
    goBackFreeway[0].max_dis = route->max_distance;
    goBackFreeway[0].dijkstra_succ = numOfStations;
    goBackFreeway[0].dijkstra_pred = numOfStations;
    goBackFreeway[0].visited = false;
    //initialized single source


    //tmpNumOfStation = numOfStations - 1;
    int max_arrival_index = tmpNumOfStation;
    int max_arrival_station = 0;
    // questi due indici servono per tener conto del massimo arrivo al giro precedente in modo da decidere
    // se saltare del tutto la visita di una stazione
    //ovvero controllo prima di scorrere se la stazione può portare a stazioni più piccole di quelle già raggiunte




    for(int tmpStation = numOfStations-1; tmpStation > 0; tmpStation--){

        max_arrival_station = goBackFreeway[tmpStation].location - goBackFreeway[tmpStation].max_dis;

        int min_index = tmpStation;
        while(goBackFreeway[min_index-1].location >= max_arrival_station && min_index >= 1){
            min_index--;
        }


        int tmp_min_reach = min_index;
        for( int j = tmpStation - 1; j >= min_index; j-- ){
            if(goBackFreeway[j].location - goBackFreeway[j].max_dis <= goBackFreeway[min_index-1].location){
                int min_reachable_from_station = tmp_min_reach;

                while(goBackFreeway[j].location - goBackFreeway[j].max_dis <= goBackFreeway[min_reachable_from_station-1].location && min_reachable_from_station > 0){
                    min_reachable_from_station--;
                }





                if(min_reachable_from_station <= tmp_min_reach){
                    goBackFreeway[tmpStation].dijkstra_pred = j;
                    goBackFreeway[j].dijkstra_pred = min_reachable_from_station;
                    goBackFreeway[min_reachable_from_station].dijkstra_succ = j;
                    tmp_min_reach = min_reachable_from_station;
                }

            }
        }

    }

    int pred = numOfStations-1;
    for(int j = goBackFreeway[numOfStations-1].dijkstra_pred; j > 0; j = goBackFreeway[j].dijkstra_pred){
          if(j == pred || j == numOfStations){
              printf("nessun percorso\n");
              return;
          }
          //if(goBackFreeway[j].dijkstra_pred > goBackFreeway[goBackFreeway[j].dijkstra_pred].dijkstra_succ)
          //    goBackFreeway[j].dijkstra_pred = goBackFreeway[goBackFreeway[j].dijkstra_pred].dijkstra_succ;
          pred = j;
    }


    for(int j = numOfStations-1; j > 0; j = goBackFreeway[j].dijkstra_pred){
        printf("%d ", goBackFreeway[j].location);
        if(goBackFreeway[j].dijkstra_pred == 0)
            printf("%d\n", goBackFreeway[0].location);
    }








}

void goOnRoute(int distance, int arrival, station *firstStation) {
    //si potrebbe migliorare contando le stazioni e creando un array di appoggio invece di fare le malloc
    station* route = firstStation;
    int numOfStations = 1;
    while(route->location != distance)
        route = route->next;

    //facciamo una prima iterazione per poi costruire l'array
    station* walk = route;
    while (walk->location + walk->max_distance < arrival && walk->location < arrival) {
        walk = walk->next;
        numOfStations++;
    }
    if(walk->location == arrival){
        printf("nessun percorso\n");
        return;
    }

    int *path = calloc(numOfStations, sizeof(int));
    int tmpNumOfStation = numOfStations - 1;
    path[tmpNumOfStation] = arrival;
    tmpNumOfStation--;
    path[tmpNumOfStation] = walk->location;
    arrival = walk->location;


    while(distance < arrival){
        walk = route;
        tmpNumOfStation--;
        while (walk->location + walk->max_distance < arrival && walk->location < arrival) {
            walk = walk->next;
        }
        if(walk->location + walk->max_distance >= arrival && walk->location < arrival ){
            path[tmpNumOfStation] = walk->location;
        }
        if(path[tmpNumOfStation] == 0 && walk->location != 0){
            //if(walk->location + walk->max_distance < arrival && walk->location < arrival) //non troppo sicuro di ciò ma dovrebbe funzionare
            printf("nessun percorso\n");
            return;
        }
        arrival = walk->location;
    }
for(int i = tmpNumOfStation; i<numOfStations-1;i++)
    printf("%d ",path[i]);
printf("%d",path[numOfStations-1]);
printf("\n");
} //works pretty well

void destroyCar(int location, int distance, station *scanStation) {
    bool destroyed = false;
    while(scanStation->next != NULL && scanStation->location <= location){
        /*to test
        if(scanStation->location == 2223){
            printf ("old cars: ");
            for(int j = 0;j<scanStation->car_number;j++)
                printf("%d ",scanStation->cars[j]);
            printf("\n");
        }
        to test*/
        if(scanStation->location == location){

            for (int i = 0; i < scanStation->car_number; i++){
                if(scanStation->cars[i] == distance && !destroyed){
                    if(i == scanStation->car_number - 1)
                        scanStation->cars[i] = 0;
                    else
                        scanStation->cars[i] = scanStation->cars[i+1];
                    scanStation->car_number = scanStation->car_number-1;
                    destroyed = true;
                    printf("rottamata\n");
                }
                else if (destroyed){
                    if(i == scanStation->car_number - 1)
                        scanStation->cars[i] = 0;
                    else
                        scanStation->cars[i] = scanStation->cars[i+1];
                }
            }
            if(!destroyed)
                printf("non rottamata\n");
            /*to test
            if(scanStation->location == 2223){
                            printf ("new cars: ");
                            for(int j = 0;j<scanStation->car_number;j++)
                                printf("%d ",scanStation->cars[j]);
                            printf("\n");
                        }
            to test*/
            return;
        }
        scanStation = scanStation->next;
    }
    if(scanStation->next == NULL && scanStation->location == location){
        for (int i = 0; i < scanStation->car_number; i++){
            if(scanStation->cars[i] == distance && !destroyed){
                if(i == scanStation->car_number - 1)
                    scanStation->cars[i] = 0;
                else
                    scanStation->cars[i] = scanStation->cars[i+1];
                scanStation->car_number = scanStation->car_number-1;
                destroyed = true;
                printf("rottamata\n");
            }
            else if (destroyed){
                if(i == scanStation->car_number - 1)
                    scanStation->cars[i] = 0;
                else
                    scanStation->cars[i] = scanStation->cars[i+1];
            }
        }
        if(!destroyed)
            printf("non rottamata\n");
        return;
    }
    printf("non rottamata\n");
}

void addCar(station *scanStation, int location, int distance) { //seems to work
    while(scanStation != NULL ){
        if(scanStation->location == location){
            /*to test
            printf ("old cars: ");
            for(int j = 0;j<scanStation->car_number;j++)
                printf("%d ",scanStation->cars[j]);
            printf("\n");
            //to test*/
            for (int i = 0; i <= scanStation->car_number; i ++){
                if(i == scanStation->car_number){
                    /*printf("%d",location);
                    printf("\t");
                    printf("%d",distance);
                    printf("\n");*/
                    scanStation->cars[i] = distance;
                    scanStation->car_number++;
                    if(scanStation->cars[i]>scanStation->max_distance)
                        scanStation->max_distance = scanStation->cars[i];
                    /*to test
                    printf ("new cars: ");
                    for(int j = 0;j<scanStation->car_number;j++)
                        printf("%d ",scanStation->cars[j]);
                    printf("\nmax: %d\n",scanStation->max_distance);

                    //to test*/

                    printf("aggiunta\n");
                    return;
                }
            }
        }
        scanStation = scanStation->next;
    }
    printf("non aggiunta\n");
} //works quite good

void deleteStation(int location, station *scanStation) {
    while(scanStation->next != NULL ){
        if(scanStation->location == location){
            station *tmp = scanStation;
            scanStation->next->prev = tmp->prev;
            if(scanStation->prev != NULL)
                scanStation->prev->next = tmp->next;
            else
                scanStation->location = -1234;
            printf("demolita\n");
            return;
        }
        scanStation = scanStation->next;
    }
    if(scanStation->location == location){
        station *tmp = scanStation;
        scanStation->prev->next = NULL;
        printf("demolita\n");
        free(tmp);
        return;
    }
    printf("non demolita\n");
} //need to look at the free() but it works

void addStation(station *first) {
    int locationToAdd,numOfCarsToAdd;
    if (scanf("%d %d", &locationToAdd, &numOfCarsToAdd) != EOF){
        //do nothing
    }
    else{
        printf("scanf not working in addStation");
        return;
    }
    //caso base lista vuota
    if(first->location == -1){
         first->location = locationToAdd;
         first->car_number = numOfCarsToAdd;
         first->max_distance = 0;
         first->cars = calloc(512,sizeof (int));
         for (int i = 0; i < first->car_number; i++){
            if(scanf("%d", &first->cars[i])){
                if(first->cars[i]>first->max_distance)
                    first->max_distance = first->cars[i];
            }

         }
        first->next = NULL;
        first->prev = NULL;
        printf("aggiunta\n");
        return;
    }
    //caso di inserimento in mezzo
    if(first->location > locationToAdd){
        station *tmp = malloc(sizeof (station));
        tmp->location = locationToAdd;
        tmp->car_number = numOfCarsToAdd;
        tmp->max_distance = 0;
        tmp->cars = calloc(512,sizeof (int));
        for (int i = 0; i < tmp->car_number; i++) {
            scanf("%d", &tmp->cars[i]);
            if (tmp->cars[i] > tmp->max_distance)
                tmp->max_distance = tmp->cars[i];
        }
        tmp->prev = NULL;
        tmp->next = first;
        first->prev = tmp;
        printf("aggiunta\n");
        return;
    }
    else{
        station *tmp = malloc(sizeof (station));
        tmp->location = locationToAdd;
        tmp->car_number = numOfCarsToAdd;
        tmp->max_distance = 0;
        tmp->cars = calloc(512,sizeof (int)); //tmp->next = NULL;        tmp->prev = NULL;
        for (int i = 0; i < tmp->car_number; i++) {
            scanf("%d", &tmp->cars[i]);
            if (tmp->cars[i] > tmp->max_distance)
                tmp->max_distance = tmp->cars[i];
        }
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
