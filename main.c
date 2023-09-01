
#include <stdio.h>
#include <malloc.h>
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
    struct station *left;
    struct station *right;
    struct station *father;
} station;

typedef struct simplifiedStation{
    int location;
    int hopNumber;
    struct simplifiedStation *nextHop;
    struct simplifiedStation *prevHop;

} simplifiedStation;

typedef struct headStation{
    int hopNumber;
    struct headStation *nextRoute;
    struct headStation *prevRoute;
    simplifiedStation *firstHop;
} headStation;




void addCar(station *freewayStation, int location, int distance);
void addStation(station **freewayStation, int distance, int numOfCars, int *tripDistance);
void deleteStation(int location, station *freewayStation);
void destroyCar(int location, int distance, station *freewayStation);
void planRoute(int departure, int arrival, station *freewayStation);
char insertBST(station *stationToAdd, station *freewayStation);
void stationTransplant(station *freeWayStation, station *u, station *v);
void insertCarBST(int trip, car** carTree);
int extractMaxCar(car *fleet);
station *successor(station *pStation);
void findRoute(simplifiedStation *simplifiedStation, headStation *headRouteStation, station *departureStation, int arrival);

void addStation(station **freewayStation, int location, int numOfCars, int *tripDistance) {

    station *stationToAdd = malloc(sizeof(station));
    stationToAdd->right = NULL;
    stationToAdd->left = NULL;
    stationToAdd->father = NULL;
    stationToAdd->location = location;
    stationToAdd->car_number = numOfCars;

    for(int i=0;i<numOfCars;i++){
        if(i==0)
            stationToAdd->cars = NULL;
        insertCarBST(*tripDistance + i, &(stationToAdd->cars));
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

    printf( "aggiunta");
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

void addCar(station *freewayStation, int location, int distance) {

    station *tmp = freewayStation;
    while(tmp!=NULL){
        if(tmp->location > location)
            tmp=tmp->left;
        else if(tmp->location == location) {
            insertCarBST(distance, &tmp->cars);
        }
        else
            tmp=tmp->right;
    }
    if(tmp == NULL){
        fprintf(stdout, "non aggiunta");
        return;
    }

}

void stationTransplant(station *freeWayStation, station *u, station *v) {
    if(u->father == NULL)
        freeWayStation = v;
    else if(u == u->father->left)
        u->father->left = v;
    else
        u->father->right = v;
    if(v != NULL)
        v->father = u->father;
}

void deleteStation(int location, station *freewayStation) {
    station *toDelete = freewayStation;
    while(toDelete!=NULL){
        if(toDelete->location > location)
            toDelete=toDelete->left;
        else if(toDelete->location == location){
            if(toDelete->left == NULL)
                stationTransplant(freewayStation, toDelete, toDelete->right);
            else if(toDelete->right == NULL)
                stationTransplant(freewayStation, toDelete, toDelete->left);
            else{
                station *y = toDelete->right;
                while(y->left != NULL)
                    y = y->left;
                if(y->father != toDelete){
                    stationTransplant(freewayStation, y, y->right);
                    y->right = toDelete->right;
                    y->right->father = y;
                }
                stationTransplant(freewayStation, toDelete, y);
                y->left = toDelete->left;
                y->left->father = y;
            }
        }
        else
            toDelete=toDelete->right;
    }
    if(toDelete == NULL)
        fprintf(stdout, "non demolita");
    else
        fprintf(stdout, "demolita");
    free(toDelete);
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
                fprintf(stdout, "non demolita");
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


void planRoute(int departure, int arrival, station *freewayStation) {
if(freewayStation == NULL){
fprintf(stdout, "nessun percorso");
return;
}
station *tmp = freewayStation;
if(tmp->location > departure)
tmp=tmp->left;
else if(tmp->location == departure) {
simplifiedStation *simpStation = malloc(sizeof (simplifiedStation));
simpStation->nextHop = NULL;
headStation *headStat = malloc(sizeof (headStation));
headStat->hopNumber = 0;
headStat->prevRoute = NULL;
findRoute(simpStation,headStat,tmp,arrival);
}
else
tmp=tmp->right;
/*if(arrival >= departure){
    *
     * Crea un array di liste per la ricerca del percorso, scegli l'auto con più autonomia dal lotto, analizza tutti i percorsi raggiungibili e salva in una variabile il numero di hop
     * e la posizione dell'array della lista prescelta e scarta tutte le liste che ci mettono di più (flag)*/



//scegliere il massimo dall'heap, poi salvare la location della station, poi cercare la station successiva fino a trovare un percorso
//dopo aver trovato un percorso inizio a scorrere a ritroso le station per vedere se c'è un percorso con stazioni più vicine alla partenza
//se non c'è un percorso con stazioni più vicine alla partenza, allora si tiene il percorso trovato
//se c'è un percorso con stazioni più vicine alla partenza, allora si tiene il percorso con stazioni più vicine alla partenza
//se il percorso richiede più hop, ovvero non arriva all'hop successivo, allora si tiene il percorso con precedente
/*scegliere il minimo dal grado per arrivare alla stazione successiva
 *cambiare stazione e iterare fino a quando non si arriva a destinazione
 * */
//}
//else{
/*scegliere il massimo dal grado per arrivare alla stazione successiva
 *cambiare stazione e iterare fino a quando non si arriva a destinazione
 * */
//}
}

void findRoute(simplifiedStation *pSimplifiedStation, headStation *headRouteStation, station *departureStation, int arrival) {
    station *tmp = departureStation;
    int hopNumber = 0;
    bool checkArrived = false;
    pSimplifiedStation->location = departureStation->location;
    while(tmp->location < arrival){
        simplifiedStation *tmpSimp = pSimplifiedStation;
        //prima cerco il percorso minimo, cercando di passare sempre al tragitto più lungo per trovare un percorso
        int trip = extractMaxCar(tmp->cars);
        station *tmp2 = tmp;
        while(tmp2->location<=tmp->location+trip){
            tmp2 = successor(tmp2);
        }
        pSimplifiedStation->nextHop->location = tmp2->location;
        pSimplifiedStation->nextHop->prevHop = pSimplifiedStation;
        pSimplifiedStation = pSimplifiedStation->nextHop;
        tmp = tmp2;
        //poi dalla fine cerco di tornare indietro ed estrarre la max car per vedere se raggiungo
        //comunque il nodo successivo da una stazione più bassa
    }
    while(pSimplifiedStation->nextHop!=NULL){
        fprintf(stdout,"%d",pSimplifiedStation->location);
        pSimplifiedStation = pSimplifiedStation->nextHop;
    }
}

station *successor(station *pStation) {
    station *tmp = pStation;
    if(tmp->right!=NULL){
        tmp = tmp->right;
        while (tmp->left!=NULL)
            tmp = tmp->left;
    } else {
        if(tmp->father->left->location == tmp->location)
            return tmp->father;
        else{
            tmp = tmp->father;
            tmp = tmp->right;
            while (tmp->left!=NULL)
                tmp = tmp->left;
        }
    }
    return tmp;
}


int extractMaxCar(car *fleet) {
    int max = 0;
    while(fleet->right != NULL)
        max = fleet->distance;
    return max;
}



int main(){

    station *pStation = NULL;

    FILE *file_in;
    FILE *file_out;
    file_in = freopen("archivio_test_aperti/open_1.txt", "r", stdin);
    file_out = freopen("outMio.txt", "w", stdout);
    if(file_in == NULL)
        return 2;
    if(file_out == NULL)
        return 3;

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
                deleteStation(location,pStation);
            }
        } else if (strcmp(input, "aggiungi-auto") == 0) {
            if (scanf("%d %d", &location, &distance)) {
                addCar(pStation, location, distance);
            }
        } else if (strcmp(input, "rottama-auto") == 0) {
            if (scanf("%d %d", &location, &distance)) {
                destroyCar(location, distance,pStation);
            }
        } else if (strcmp(input, "pianifica-percorso") == 0) {
            if (scanf("%d %d", &distance, &arrival)) {
                planRoute( distance, arrival,pStation);
            }
        }
    }
    return 0;
}




