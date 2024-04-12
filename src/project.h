/*
 * iaed-24 - ist1106909 - project
 * Header file with all  the structs and main functions prototypes.
 */
#ifndef PROJECT_H
#define PROJECT_H

#include "constants.h"

/**
 * @brief Represents a date with day, month, and year components.
 */
typedef struct date {
    int day; 
    int month; 
    int year;  
} Date;

/**
 * @brief Represents a time with hour and minute components.
 */
typedef struct time {
    int hour;   
    int minute;  
} Time;

/**
 * @brief Represents a log for entry or exit of a vehicle from a park.
 */
typedef struct Log {
    Date *entryDate;    /**<  Date of entry. */
    Date *exitDate;     /**<  Date of exit. */
    Time *entryTime;    /**<  Time of entry. */
    Time *exitTime;     /**<  Time of exit. */
    char *reg;          /**<  Vehicle registration. */
    char *parkName;     /**<  Name of the park. */
    int type;           /**<  Type of log (0 for entry, 1 for exit). */
    double value;       /**<  Value associated with the log. */
} Log;

/**
 * @brief Represents a node in a log linked list.
 */
typedef struct LogNode {
    Log *log;               /**< Log */
    struct LogNode *next;   /**< Pointer to the next log node. */
    struct LogNode *prev;   /**< Pointer to the previous log node. */
} LogNode;

/**
 * @brief Represents a vehicle.
 */
typedef struct Vehicle {
    char *parkName;     /**< Name of the park where the vehicle is parked. */
    char *registration; /**< Vehicle registration number. */
    int isParked;       /**< Flag indicating whether the vehicle is parked. */
    LogNode *lHead;     /**< Pointer to the head of the log linked list. */
    LogNode *lTail;     /**< Pointer to the tail of the log linked list. */
    Log *lastLog;       /**< Pointer to the last log entry. */
} Vehicle;

/**
 * @brief Represents a node in a vehicle linked list.
 */
typedef struct VehicleNode {
    Vehicle *vehicle;           /**< Vehicle. */
    struct VehicleNode *next;   /**< Pointer to the next vehicle node. */
    struct VehicleNode *prev;   /**< Pointer to the previous vehicle node. */
} VehicleNode;

/**
 * @brief Represents a Park.
 */
typedef struct Park {
    char *name;             /**< Name of the park. */
    int maxCapacity;        /**< Maximum capacity of the park. */
    int currentLots;        /**< Current occupancy of the parking lot. */
    double quarterCost;     /**< Cost per quarter-hour. */
    double afterHourCost;   /**< Cost after first hour. */
    double dailyCost;       /**< Daily cost. */
    int isSorted;           /**< Flag indicating whether logs are sorted. */
    LogNode *lHead;         /**< Head of the log linked list. */
    LogNode *lTail;         /**< Tail of the log linked list. */
} Park;

/**
 * @brief Represents a node in a parking lot linked list.
 */
typedef struct ParkNode {
    Park *parking;              /**< Park */
    struct ParkNode *next;      /**< Next Park node */
    struct ParkNode *prev;      /**< Previous Park node */
} ParkNode;

/**
 * @brief Represents a Buffer.
 */
typedef struct Buffer {
    char *buffer;   /**<  Buffer storing character data. */
    int index;      /**<  Current position in the buffer. */
} Buffer;

/**
 * @brief Represents a node in a hash table for storing vehicles.
 */
typedef struct VehicleHashNode {
    Vehicle *vehicle;               /**< Vehicle. */
    struct VehicleHashNode *next;   /**< Next hash node. */
} VehicleHashNode;

/**
 * @brief Represents the  System.
 */
typedef struct System { 
    Park *parks[MAX_PARKING_LOTS];              /**< Array of parking lots. */
    ParkNode *pHead;                            /**< Head of the parks linked list. */
    VehicleNode *vHead;                         /**< Head of the vehicle linked list. */
    VehicleNode *vTail;                         /**< Tail of the vehicle linked list. */
    VehicleHashNode *hashTable[HASH_TABLE_SIZE];/**< Hash table for quick vehicle lookup. */
    int numParks;                               /**< Number of parks. */
    Date *lastDate;                             /**< Last date recorded in the system. */
    Time *lastTime;                             /**< Last time recorded in the system. */
} System;


/*
 *  Initialization functions
 */
Buffer *getBuffer(Buffer *buffer);
System* init();
void initParksArray(System* sys);
void initHashTable(System* sys);

/*
 *  Park related functions
 */
void addParkToArray(System *sys, Park *park);
void addParkToList(System *sys, ParkNode *parking);
void addPark(System *sys, ParkNode *parking);
void updateParksArray(System *sys, int index);
void removeParkFromArray(System *sys, char *name);
void removeParkFromList(System *sys, char *name);
void removePark(System *sys, char *name);
void printParks(System* sys);
void printRemainingParks(System* sys);
Park* getPark(System* sys, char* name);
ParkNode *createPark(char *name, char *maxCapacity, char *quarterCost, char *afterHourCost, char *dailyCost);
Park *createParkData(char *name, char *maxCapacity, char *quarterCost, char *afterHourCost, char *dailyCost);
void newParkRequest(System* sys, Buffer* buffer, char* name);

/*
 *  Vehicle related functions
 */
void addVehicleToList(System *sys, Vehicle *vehicle); 
unsigned int hash(const char *reg);
void addToHashTable(System *sys, Vehicle *vehicle);
void addVehicle(System *sys, Vehicle *vehicle);
Vehicle *getVehicle(System *sys, char *reg);
Vehicle *createVehicleData(char *reg);
Vehicle *createVehicle(System *sys, char *reg);

/*
 *  Requests related functions
 */
int enterPark(System *sys, Park *p, Vehicle *v, char *date, char *time);
void printExit(Vehicle *v);
int exitPark(System *sys, Park *p, Vehicle *v, char *date, char *time);
void updateSystem(System *sys, char *date, char *time);

/*
 *  Log related functions
 */
Log *addLogToVehicle(Vehicle *v, Log *l);
void addLogToPark(Park *p, Log *log);
void addLog(Log *newLog, Vehicle *v, Park *p);
Log *updateEntryLog(Log *l, char *date, char *time, Park *park);
Log *createLog(Vehicle *v, Park *p, char *d, char *t);

/*
 *  Print and revenue related functions
 */
void printExits(LogNode *cur, char *dEntry, char *tEntry);
void printVehicleLogs(Vehicle *v);
void printLogsByDate(LogNode *head);
void printRevenue(LogNode *cur, Date *date);
void showParkRevenue(Park* p, Date* date);
void processRevenueCheck(System *sys, Park *park, char *date);
double calculateValue(Log *log, Park *park);

/*
 *  Command functions
 */
void commandP(System* sys, Buffer* buffer);
void commandR(System* sys, Buffer* buffer);
void commandE(System* sys, Buffer* buffer);
void commandS(System* sys, Buffer* buffer);
void commandV(System* sys, Buffer* buffer);
void commandF(System* sys, Buffer* buffer);

/*
 *  Free memory functions
 */
void freeArgs(char *name, char *reg, char *time, char *date);
void freeHashTable(System *sys);
void freeLogNode(LogNode *node);
void freeVehicles(System *sys);
void freeParks(System *sys);
void terminate(System* sys, Buffer* buffer);

#endif // PROJECT_H

