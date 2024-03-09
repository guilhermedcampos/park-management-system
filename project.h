#ifndef PROJECT_H
#define PROJECT_H

#include "constants.h"


typedef struct Vehicle {
    char *parkName;
    char *registration;
    char *entryTime;
    char *exitTime;
    char *entryDate;
    char *exitDate;
    int isParked;
} Vehicle;

typedef struct VehicleNode {
    Vehicle *vehicle;
    struct VehicleNode *next;
    struct VehicleNode *prev;
} VehicleNode;

typedef struct Park {
    char *name;
    int maxCapacity;
    int currentLots;
    double billingValue15;
    double billingValueAfter1Hour;
    double maxDailyValue;
    char **regs;
} Park;

typedef struct ParkingNode {
    Park *parking;
    struct ParkingNode *next;
    struct ParkingNode *prev;
} ParkingNode;


typedef struct Buffer {
    char *buffer;
    int index;
} Buffer;

typedef struct ParkingSystem {
    ParkingNode *head;
    VehicleNode *vehicles;
    int numParks;
} ParkingSystem;

ParkingSystem* init();
void printParks(ParkingSystem* system);
Park* parkExists(ParkingSystem* sys, char* name);
int isParkFull(ParkingSystem* sys, char* name);
void addPark(ParkingSystem *system, ParkingNode *parking);
int createVehicle(ParkingSystem *system, char *reg);
void createPark(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue);
void removePark(ParkingSystem *system, char *name);
int enterPark(Park *p, Vehicle *v, char *date, char *time);
void createPark(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue);
void commandP(ParkingSystem* system, Buffer* buffer);
void commandR(ParkingSystem* system, Buffer* buffer);
void commandE(ParkingSystem* system, Buffer* buffer);

#endif // PROJECT_H

