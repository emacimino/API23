#include <stdio.h>
//#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define COMMAND_LENGTH 20
#define MAX_CAR 512

typedef struct car{
    int distance;
    struct car* father;
    struct car* left;
    struct car* right;
}car;

typedef struct station{
    int location;
    int car_number;
    car* cars;
    int max_distance;
    struct station *left;
    struct station *right;
    struct station *father;
} station;

typedef struct simplifiedStation{
    struct station *staz;
    struct simplifiedStation *nextHop;
    struct simplifiedStation *prevHop;

} simplifiedStation;






void addCar(station **freewayStation, int location, int distance);
void addStation(station **freewayStation, int location, int numOfCars, int *tripDistance);
void deleteStation(int location, station **freewayStation);
void destroyCar(int location, int distance, station *freewayStation);
void planRoute(int departure, int arrival, station **freewayStation);
void stationTransplant(station **freeWayStation, station **u, station **v);
void insertCarBST(int trip, car** carTree);

void stationBSTDelete(station **treeRoot, station **toDelete);

station *treeMinimum(struct station **pStation);

station *findDeparture(int departure, station **headTree);


void appendToList(station *treeStation, simplifiedStation **routeStation);

void inOrderVisitAndCreateList(station **treeStation, simplifiedStation **listStation, int departure, int arrival);


void addStation(station **freewayStation, int location, int numOfCars, int *tripDistance) {

    station *stationToAdd = malloc(sizeof(station));
    stationToAdd->right = NULL;
    stationToAdd->left = NULL;
    stationToAdd->father = NULL;
    stationToAdd->cars = NULL;
    stationToAdd->location = location;
    stationToAdd->car_number = numOfCars;


    for(int i=0;i<numOfCars;i++){
        if(i == 0)
            stationToAdd->max_distance = *tripDistance;
        if(*(tripDistance + i)>stationToAdd->max_distance)
            stationToAdd->max_distance = *(tripDistance + i);
        insertCarBST(*(tripDistance + i), &(stationToAdd->cars));
    }
    if(numOfCars == 0){
        stationToAdd->max_distance = 0;
    }


    station *y = NULL;
    station *x = *freewayStation;

    while(x!=NULL){
        y=x;
        if(stationToAdd->location<x->location)
            x=x->left;
        else if(stationToAdd->location==x->location){
            printf( "non aggiunta");
            return;
        }else
            x=x->right;
    }
    stationToAdd->father = y;
    if(y==NULL){
        *freewayStation = stationToAdd;
    }else if(stationToAdd->location<y->location)
        y->left = stationToAdd;
    else
        y->right = stationToAdd;

    printf("aggiunta");
}

void insertCarBST(int trip, car** carTree) {
    car *y = NULL;
    car *x = *carTree;
    car *z = malloc(sizeof (car));
    z->distance = trip;
    z->right = NULL;
    z->left = NULL;
    while(x!=NULL){
        y=x;
        if(z->distance<x->distance)
            x=x->left;
        else
            x=x->right;
    }
    z->father = y;
    if(y==NULL){
        *carTree = z;
    }else if(z->distance<y->distance)
        y->left = z;
    else
        y->right = z;
}

void addCar(station **freewayStation, int location, int distance) {

    station *x = *freewayStation;

    while(x!=NULL){
        if(location<x->location)
            x=x->left;
        else if(location==x->location){
            if(distance>x->max_distance)
                x->max_distance = distance;
            insertCarBST(distance, &x->cars);
            printf( "aggiunta");
            return;
        }else
            x=x->right;
    }
    if(x == NULL){
        printf( "non aggiunta");
        return;
    }
}

void stationTransplant(station **freeWayStation, station **u, station **v) {
    station *ut = *u;
    station *vt = *v;
    station *treeR = *freeWayStation;
    if(ut->father == NULL)
        *treeR = *vt;
    else if(ut == ut->father->left)
        ut->father->left = vt;
    else
        ut->father->right = vt;
    if(vt != NULL)
        vt->father = ut->father;
    free(ut);
    free(vt);
}

void deleteStation(int location, station **freewayStation) {
    station *toDelete = *freewayStation;

    while(toDelete != NULL){
        if(location < toDelete->location)
            toDelete=toDelete->left;
        else if(location == toDelete->location){
            stationBSTDelete(&*freewayStation,&toDelete);
            printf("demolita");
            return;
        }else
            toDelete=toDelete->right;
    }
    printf("non demolita");
    free(toDelete);
}

void stationBSTDelete(station **treeRoot, station **toDelete) {
    station *toDel = *toDelete;
    if(toDel == NULL)
        stationTransplant(&*treeRoot,&toDel,&toDel->right);
    else if(toDel->right == NULL)
        stationTransplant(&*treeRoot,&toDel,&toDel->left);
    else{
        station *y = treeMinimum(&toDel->right);
        if(y->father != toDel){
            stationTransplant(&*treeRoot,&y,&y->right);
            y->right = toDel->right;
            y->right->father = toDel;
        }
        stationTransplant(&*treeRoot,&toDel,&y);
        y->left = toDel->left;
        y->left->father = y;
    }
}

station* treeMinimum(struct station **pStation) {
    station *result = *pStation;
    while(result->left!=NULL)
        result = result->left;
    return result;
}

void carTransplant(car *headCar, car *u, car *v) {
    if(u->father == NULL)
        headCar = v;
    else if(u == u->father->left)
        u->father->left = v;
    else
        u->father->right = v;
    if(v != NULL)
        v->father = u->father;
}


//rottama auto probabilmente da rifare
void destroyCar(int location, int distance, station *freewayStation) {
    station *stationNotToDelete = freewayStation;
    while(stationNotToDelete != NULL){
        if(stationNotToDelete->location > location)
            stationNotToDelete=stationNotToDelete->left;
        else if(stationNotToDelete->location == location){
            car *carToDelete = freewayStation->cars;
            while(carToDelete != NULL){
                if(carToDelete->distance > distance)
                    carToDelete=carToDelete->left;
                else if(carToDelete->distance == distance){
                    if(carToDelete->left == NULL)
                        carTransplant(stationNotToDelete->cars, carToDelete, carToDelete->right);
                    else if(carToDelete->right == NULL)
                        carTransplant(freewayStation->cars, carToDelete, carToDelete->left);
                    else{
                        car *y = carToDelete->right;
                        while(y->left != NULL)
                            y = y->left;
                        if(y->father != carToDelete){
                            carTransplant(freewayStation->cars, y, y->right);
                            y->right = carToDelete->right;
                            y->right->father = y;
                        }
                        carTransplant(freewayStation->cars, carToDelete, y);
                        y->left = carToDelete->left;
                        y->left->father = y;
                    }
                }
                else
                    carToDelete=carToDelete->right;
            }
            if(carToDelete == NULL)
                fprintf(stdout, "non demolita\n");
            else
                fprintf(stdout, "demolita");
            free(carToDelete);
        }
        else
            stationNotToDelete=stationNotToDelete->right;
    }
    if(stationNotToDelete == NULL)
        fprintf(stdout, "non rottamata");
    free(stationNotToDelete);
}


void planRoute(int departure, int arrival, station **freewayStation) {
    int realArrival = arrival;
    station *headRoute = findDeparture(departure, freewayStation);
    simplifiedStation *stationsAvailable = NULL;
    simplifiedStation *route = NULL;
    inOrderVisitAndCreateList(&headRoute,&stationsAvailable,departure,arrival);

    route = malloc(sizeof(simplifiedStation));
    route->prevHop = NULL;
    route->staz = NULL;
    route->nextHop = NULL;
    while(departure < arrival){
        simplifiedStation *curr = stationsAvailable;
        while(curr->staz->location + curr->staz->max_distance < arrival){
            curr = curr->nextHop;
        }
        if(curr->staz->location==arrival){
            printf("nessun percorso\n");
            free(route);
            return;
        }

        arrival = curr->staz->location;
        if(route == NULL){
            route = malloc(sizeof(simplifiedStation));
            route->prevHop = NULL;
            route->staz = curr->staz;
            route->nextHop = NULL;
        }
        else{
            simplifiedStation *currRoute = malloc(sizeof(simplifiedStation));
            currRoute->staz = curr->staz;
            currRoute->nextHop = curr;
            currRoute->prevHop = NULL;
            route = currRoute;
        }

    }



}

void inOrderVisitAndCreateList(station **treeStation, simplifiedStation **listStation, int departure, int arrival) {

    if(*treeStation!=NULL ){
        inOrderVisitAndCreateList(&((*treeStation)->left),listStation,departure,arrival);
        if((*treeStation)->location <= arrival && (*treeStation)->location >= departure){
            appendToList(*treeStation,&(*listStation));
        }
        inOrderVisitAndCreateList(&((*treeStation)->right),listStation,departure,arrival);
    }
}



void appendToList(station *treeStation, simplifiedStation **routeStation) {
    if(*routeStation == NULL){
        (*routeStation) = malloc(sizeof(simplifiedStation));
        (*routeStation)->prevHop = NULL;
        (*routeStation)->nextHop = NULL;
        (*routeStation)->staz = treeStation;
        return;
    }
    simplifiedStation *station = malloc(sizeof(simplifiedStation));
    simplifiedStation *curr = *routeStation;

    while (curr->nextHop!= NULL){
        curr = curr->nextHop;
    }
    curr->nextHop = station;
    station->prevHop = curr;
    station->nextHop = NULL;
    station->staz = treeStation;
}
station *findDeparture(int departure, station **headTree) {
    station *x = *headTree;
    station *y = x;
    while(y!=NULL){
        if(departure<y->location)
            y=y->left;
        else if(departure==y->location){
            return y;
        }else
            y=y->right;
    }
    return NULL;
}


int main(){

    station *pStation = NULL;

    FILE *file_in;

    file_in = freopen("archivio_test_aperti/open_2.txt", "r", stdin);

    if(file_in == NULL)
        return 2;


    char input[COMMAND_LENGTH];
    int location, numOfCars, distance, arrival;
    int tripDistance[MAX_CAR];

    while (scanf("%s", input) != EOF) {
        if (strcmp(input, "aggiungi-stazione") == 0) {
            if (scanf("%d %d", &location, &numOfCars) != EOF) {
                for (int i = 0; i < numOfCars; i++)
                    scanf("%d", &tripDistance[i]);
                addStation(&pStation, location, numOfCars, tripDistance);

            }
        } else if (strcmp(input, "demolisci-stazione") == 0) {
            if (scanf("%d", &location) != EOF) {
                deleteStation(location,&pStation);
            }
        } else if (strcmp(input, "aggiungi-auto") == 0) {
            if (scanf("%d %d", &location, &distance)) {
                addCar(&pStation, location, distance);
            }
        } else if (strcmp(input, "rottama-auto") == 0) {
            if (scanf("%d %d", &location, &distance)) {
                destroyCar(location, distance,pStation);
            }
        } else if (strcmp(input, "pianifica-percorso") == 0) {
            if (scanf("%d %d", &distance, &arrival)) {
                planRoute( distance, arrival,&pStation);
            }
        }
    }
    return 0;
}







