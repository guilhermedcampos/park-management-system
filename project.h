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
    ParkingNode *pHead;
    VehicleNode *vHead;
    LogNode *lHead;
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
int enterPark(ParkingSystem *system, Park *p, Vehicle *v, char *date, char *time);
int exitPark(ParkingSystem *system, Park *p, Vehicle *v, char *date, char *time);
void createPark(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue);
Log *changeLog(ParkingSystem *system, Date *date, Time *time, char *reg, char *name, int type);
void addLog(ParkingSystem *system, Log *log);
void commandP(ParkingSystem* system, Buffer* buffer);
void commandR(ParkingSystem* system, Buffer* buffer);
void commandE(ParkingSystem* system, Buffer* buffer);
void commandS(ParkingSystem* system, Buffer* buffer);

#endif // PROJECT_H

