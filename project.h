#ifndef PROJECT_H
#define PROJECT_H

#include "constants.h"


typedef struct Vehicle {
    char registration[REGISTRATION_LENGTH + 1];
} Vehicle;

typedef struct ParkingLot {
    char name[256];
    int maxCapacity;
    double billingValue15;
    double billingValueAfter1Hour;
    double maxDailyValue;
} ParkingLot;

typedef struct ParkingNode {
    ParkingLot *parking;
    struct ParkingNode *next;
    struct ParkingNode *prev;
} ParkingNode;


typedef struct Buffer {
    char *buffer;
    int index;
} Buffer;

typedef struct ParkingSystem {
    ParkingNode *head;
    int numParkingLots;
} ParkingSystem;

ParkingSystem* init();
void printParkingLots(ParkingSystem* system);
int parkingExists(ParkingSystem* sys, char* name);
void addParking(ParkingSystem *system, ParkingNode *parking);
void removeParking(ParkingSystem *system, char *name);
void createParkingLot(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue);
void commandP(ParkingSystem* system, Buffer* buffer);
void commandR(ParkingSystem* system, Buffer* buffer);

#endif // PROJECT_H

