#ifndef PROJECT_H
#define PROJECT_H

#include "constants.h"

typedef struct date {
    int day;
    int month;
    int year;
} Date;

typedef struct time {
    int hour;
    int minute;
} Time;

typedef struct Log {
    Date *entryDate;
    Date *exitDate;
    Time *entryTime;
    Time *exitTime;
    char *reg;
    char *parkName;
    int type;  // 0 corresponds to an entry, 1 to a leave
    double value;  // value to be paid
} Log;

typedef struct LogNode {
    Log *log;
    struct LogNode *next;
    struct LogNode *prev;
} LogNode;

typedef struct Vehicle {
    char *parkName;
    char *registration;
    Date *date;
    Time *time;
    int isParked;
    // logs of entries and exits from the vehicle, will be in entry order
    LogNode *lHead;
    LogNode *lTail;
    Log *lastLog;
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
    // logs of entries and exits in the park
    LogNode *lHead;
    LogNode *lTail;
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


typedef struct VehicleHashNode {
    Vehicle *vehicle;
    struct VehicleHashNode *next;
} VehicleHashNode;

typedef struct ParkingSystem {
    Park *parks[MAX_PARKING_LOTS];
    ParkingNode *pHead;
    VehicleNode *vHead;
    VehicleNode *vTail;
    VehicleHashNode *hashTable[HASH_TABLE_SIZE];
    int numParks;
    Date *lastDate;
    Time *lastTime;
} ParkingSystem;


ParkingSystem* init();
void printParks(ParkingSystem* system);
Park* parkExists(ParkingSystem* sys, char* name);
int isParkFull(ParkingSystem* sys, char* name);
void addPark(ParkingSystem *system, ParkingNode *parking);
Vehicle *createVehicle(ParkingSystem *system, char *reg);
void createPark(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue);
void removePark(ParkingSystem *system, char *name);
void addVehicle(ParkingSystem *system, Vehicle *vehicle);
int enterPark(ParkingSystem *system, Park *p, Vehicle *v, char *date, char *time);
int exitPark(ParkingSystem *system, Park *p, Vehicle *v, char *date, char *time);
void createPark(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue);
Log *changeLog(Vehicle *v, Park *p, int type);
int printVehicleLogs(ParkingSystem* system, char* reg);
void printRemainingParks(ParkingSystem* system);
void showParkRevenue(Park* p, Date* date);
unsigned int hash(const char *reg);
void commandP(ParkingSystem* system, Buffer* buffer);
void commandR(ParkingSystem* system, Buffer* buffer);
void commandE(ParkingSystem* system, Buffer* buffer);
void commandS(ParkingSystem* system, Buffer* buffer);
void commandV(ParkingSystem* system, Buffer* buffer);
void commandF(ParkingSystem* system, Buffer* buffer);

#endif // PROJECT_H

