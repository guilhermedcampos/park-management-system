#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "project.h"
#include "constants.h"
#include "projectaux.h"

int bufferIndex = 0;


/*
TO-DO
define constants for error messages


*/

ParkingSystem* init() {
    ParkingSystem *system = (ParkingSystem *)malloc(sizeof(ParkingSystem));
    if (system == NULL) {
        return NULL;
    }
    system->pHead = NULL;
    system->vHead = NULL;
    system->vTail = NULL;
    system->lastDate = NULL;
    system->lastTime = NULL;
    system->numParks = 0;

    // Initialize parks array to NULL
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        system->parks[i] = NULL;
    }

    // Initialize hashTable to NULL
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        system->hashTable[i] = NULL;
    }

    return system;
}

void printParks(ParkingSystem* system) {
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        if (system->parks[i] != NULL) {
            printf("%s %d %d\n", system->parks[i]->name, system->parks[i]->maxCapacity, system->parks[i]->maxCapacity - system->parks[i]->currentLots);
        }
    }
}

/* Returns park, or Null if it doesn't exist */
Park* parkExists(ParkingSystem* sys, char* name) {
    ParkingNode *cur = sys->pHead;

    while (cur != NULL) {
        if (strcmp(cur->parking->name, name) == 0) {
            return cur->parking;
        }
        cur = cur->next;
    }
    return NULL;
}

/* Returns 1 if full, 0 if not full */
int isParkFull(ParkingSystem* sys, char* name) {
    ParkingNode *cur = sys->pHead;
    while (cur != NULL) {
        if (strcmp(cur->parking->name, name) == 0) {
            if (cur->parking->currentLots == cur->parking->maxCapacity) {
                return 1;
            }
        }
        cur = cur->next;
    }
    return 0;
}

void addParkToArray(ParkingSystem *system, Park *park) {
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        if (system->parks[i] == NULL) {
            system->parks[i] = park;
            return;
        }
    }
}

void addPark(ParkingSystem *system, ParkingNode *parking) {

    addParkToArray(system, parking->parking);
    // Iterate through the list to find an empty spot
    if (system->pHead == NULL) {
        system->pHead = parking;
        parking->next = NULL;
    } else {
        ParkingNode *cur = system->pHead;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = parking;
        parking->prev = cur;
        parking->next = NULL;
    }
    system->numParks++;
}

void updateParksArray(ParkingSystem *system, int index) {

    // Shift elements to the left starting from the index
    for (int i = index; i < MAX_PARKING_LOTS - 1; i++) {
        system->parks[i] = system->parks[i + 1];
    }

    // Set the last element to NULL (as it's shifted left)
    system->parks[MAX_PARKING_LOTS - 1] = NULL;
}


void removePark(ParkingSystem *system, char *name) {

    // Update the parks array
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        if (system->parks[i] != NULL && strcmp(system->parks[i]->name, name) == 0) {
            // First element of the list, shift left the array after removing first element
            updateParksArray(system, i);
        }
    }


    // Remove all logs of vehicles in the park
    if (system->pHead == NULL) {
        return;
    }

    ParkingNode *cur = system->pHead;
    ParkingNode *prev = NULL;
    while (cur != NULL) {
        if (strcmp(cur->parking->name, name) == 0) {
            // If the node to be removed is the head of the list
            if (prev == NULL) {
                system->pHead = cur->next;
            } else {
                prev->next = cur->next;
            }

            // If the node to be removed is not the last node in the list
            if (cur->next != NULL) {
                cur->next->prev = prev;
            }
            system->numParks--;
            free(cur->parking->name);
            cur->parking->name = NULL;
            free(cur->parking);
            cur->parking = NULL;
            free(cur);
            cur = NULL;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void createPark(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue) {
    ParkingNode *newParking = (ParkingNode *)malloc(sizeof(ParkingNode));
    if (newParking == NULL) {
        return;
    }

    newParking->parking = (Park *)malloc(sizeof(Park));
    if (newParking->parking == NULL) {
        free(newParking);
        return;
    }

    // Allocate memory for the name field and copy the string
    newParking->parking->name = strdup(name);
    if (newParking->parking->name == NULL) {
        free(newParking->parking);
        free(newParking);
        return;
    }

    newParking->next = NULL;
    newParking->prev = NULL;

    // Assign other fields
    newParking->parking->lHead = NULL;
    newParking->parking->maxCapacity = atoi(maxCapacity);
    newParking->parking->currentLots = 0;
    newParking->parking->billingValue15 = atof(billingValue15);
    newParking->parking->billingValueAfter1Hour = atof(billingValueAfter1Hour);
    newParking->parking->maxDailyValue = atof(maxDailyValue);

    addPark(system, newParking);
}


// Hash function to map registration strings to indices
unsigned int hash(const char *reg) {
    unsigned int hashValue = 0;
    while (*reg) {
        hashValue = (hashValue << 5) + *reg++;
    }
    return hashValue % HASH_TABLE_SIZE;
}


// Function to add a vehicle to the hash table
void addToHashTable(ParkingSystem *system, Vehicle *vehicle) {
    unsigned int index = hash(vehicle->registration);
    VehicleHashNode *newNode = (VehicleHashNode *)malloc(sizeof(VehicleHashNode));
    if (newNode == NULL) {
        // Handle memory allocation error
        return;
    }
    newNode->vehicle = vehicle;
    newNode->next = system->hashTable[index];
    system->hashTable[index] = newNode;
}

void addVehicle(ParkingSystem *system, Vehicle *vehicle) {
    // Initialize the vehicle node pointers
    VehicleNode *vehicleNode = (VehicleNode *)malloc(sizeof(VehicleNode));
    vehicleNode->prev = NULL;
    vehicleNode->next = NULL;
    vehicleNode->vehicle = vehicle;

    if (system->vHead == NULL) {
        // If the list is empty, set both head and tail to the new node
        system->vHead = vehicleNode;
        system->vTail = vehicleNode;
    } else {
        // Otherwise, append the new node to the tail
        system->vTail->next = vehicleNode;
        vehicleNode->prev = system->vTail;
        system->vTail = vehicleNode; // Update the tail pointer
    }
    addToHashTable(system, vehicle);
}

Vehicle *createVehicle(ParkingSystem *system, char *reg) {
    Vehicle *vehicle = (Vehicle *)malloc(sizeof(Vehicle));
    if (vehicle == NULL) {
        return NULL;
    }

    // Allocate memory for the registration field and copy the string
    vehicle->registration = strdup(reg);
    if (vehicle->registration == NULL) {
        free(vehicle);
        vehicle = NULL;
        return NULL;
    }

    // Initialize other fields
    vehicle->parkName = NULL;
    vehicle->isParked = 0;
    vehicle->lHead = NULL; 
    vehicle->lTail = NULL; 
    vehicle->lastLog = NULL;

    addVehicle(system, vehicle);
    return vehicle;
}



void commandP(ParkingSystem* system, Buffer* buffer) {   
    char *name, *maxCapacity, *billingValue15, *billingValueAfter1Hour, *maxDailyValue;

    name = nextWord(buffer);

    if (name == NULL) {
        // If there are no more arguments, list the parking lots
        printParks(system);
    } else {
        // Checks if the parking lot already exists
        if (parkExists(system, name) != NULL) {
            printf("%s: parking already exists.\n", name);
        } else {
            maxCapacity = nextWord(buffer);
            billingValue15 = nextWord(buffer);
            billingValueAfter1Hour = nextWord(buffer);
            maxDailyValue = nextWord(buffer);
            if (isValidParkRequest(system->numParks, atoi(maxCapacity), atof(billingValue15), atof(billingValueAfter1Hour), atof(maxDailyValue))) {
                createPark(system, name, maxCapacity, billingValue15, billingValueAfter1Hour, maxDailyValue);
            } 
            free(maxCapacity);
            free(billingValue15);
            free(billingValueAfter1Hour);
            free(maxDailyValue);
        }
        free(name);
    }
}

void printRemainingParks(ParkingSystem* system) {
    // Sort the list by park names alphabetically
    ParkingNode *cur = sortListName(system);
    while (cur != NULL) {
        printf("%s\n", cur->parking->name);
        cur = cur->next;
    }
}

void commandR(ParkingSystem* system, Buffer* buffer) {
    char *name;

    name = nextWord(buffer);
    Park *p = parkExists(system, name);
    if (p != NULL) {
        freeParkLogs(system, p);
        removePark(system, name);
        printRemainingParks(system);

    } else {
        printf("%s: no such parking.\n", name);
    }
    free(name);
}

void commandE(ParkingSystem* system, Buffer* buffer) {
    char *name, *reg, *date, *time;

    name = nextWord(buffer);
    reg = nextWord(buffer);
    date = nextWord(buffer);
    time = nextWord(buffer);
    // Check if the entry is valid (park exists, park is not full, registration is valid, time is valid)
    if (isValidRequest(system, name, reg, date, time, 0)) {
        Park *park = parkExists(system, name);
        Vehicle *vehicle = getVehicle(system, reg);
        if (vehicle == NULL) {
            Vehicle *v = createVehicle(system, reg);
            enterPark(system, park, v, date, time);
            printf("%s %d\n", name, park->maxCapacity - park->currentLots);
        } else {
            enterPark(system, park, vehicle, date, time);
            printf("%s %d\n", name, park->maxCapacity - park->currentLots);
        }
    }
    free(name);
    name = NULL;
    free(reg);
    reg = NULL;
    free(date);
    date = NULL;
    free(time);
    time = NULL;
}

int enterPark(ParkingSystem *sys, Park *p, Vehicle *v, char *date, char *time) {
    v->parkName = p->name;
    Date *d = createDateStruct(date);
    Time *t = createTimeStruct(time);

    Date *newDate = (Date *)realloc(sys->lastDate, sizeof(Date));
    if (newDate == NULL) {
        free(d); 
        free(t);
        return -1; 
    }
    sys->lastDate = newDate;
    memcpy(sys->lastDate, d, sizeof(Date)); 

    Time *newTime = (Time *)realloc(sys->lastTime, sizeof(Time));
    if (newTime == NULL) {
        free(d); 
        free(t);
        return -1; 
    }
    sys->lastTime = newTime;
    memcpy(sys->lastTime, t, sizeof(Time));

    free(d); 
    free(t);
    
    v->isParked = 1;
    p->currentLots++;
    v->lastLog = changeLog(v, p, date, time, 0);
    return 0;
}


void printExit(Vehicle *v) {
    char *d1 = dateToString(v->lastLog->entryDate);
    char *t1 = timeToString(v->lastLog->entryTime);
    char *d2 = dateToString(v->lastLog->exitDate);
    char *t2 = timeToString(v->lastLog->exitTime);
    printf("%s %s %s %s %s %.2f\n", v->registration, d1, t1, d2, t2, v->lastLog->value);
    free(d1);
    free(t1);
    free(d2);
    free(t2);
}

int exitPark(ParkingSystem *system, Park *p, Vehicle *v, char *date, char *time) {  
    v->parkName = NULL;
    v->isParked = 0;    
    Date *d = createDateStruct(date);
    Time *t = createTimeStruct(time);

    // Allocate memory for lastDate
    Date *newDate = (Date *)realloc(system->lastDate, sizeof(Date));
    if (newDate == NULL) {
        // Handle memory allocation failure
        free(d); // Free dynamically allocated memory
        free(t);
        return 1; // Indicate failure
    }
    system->lastDate = newDate;
    memcpy(system->lastDate, d, sizeof(Date)); // Copy data to newly allocated memory

    // Allocate memory for lastTime
    Time *newTime = (Time *)realloc(system->lastTime, sizeof(Time));
    if (newTime == NULL) {
        // Handle memory allocation failure
        free(d); // Free dynamically allocated memory
        free(t);
        return 1; // Indicate failure
    }
    system->lastTime = newTime;
    memcpy(system->lastTime, t, sizeof(Time)); // Copy data to newly allocated memory

    free(d); // Free dynamically allocated memory
    free(t);
    
    p->currentLots--;
    Log *l = changeLog(v, p, date, time, 1);
    if (l == NULL) {
        return 1;
    }
    printExit(v);
    return 0;
}

// Logs are added to the list in order of entry
Log *addLogVehicle(Vehicle *v, Log *l) {
    LogNode *newLog = (LogNode *)malloc(sizeof(LogNode));
    if (newLog == NULL) {
        return NULL;
    }
    
    newLog->log = l;
    newLog->next = NULL;
    newLog->prev = NULL;

    if (v->lHead == NULL) {
        // If the list is empty, set both head and tail to the new node
        v->lHead = newLog;
        v->lTail = newLog;
    } else {
        // Otherwise, append the new node to the tail
        v->lTail->next = newLog;
        newLog->prev = v->lTail;
        v->lTail = newLog; // Update the tail pointer
    }
    return l;
}

void addLogPark(Park *p, Log *l) {
    LogNode *newLog = (LogNode *)malloc(sizeof(LogNode));
    if (newLog == NULL) {
        return;
    }
    newLog->log = l;
    newLog->next = NULL;
    newLog->prev = NULL;
    p->isSorted = 0;

    if (p->lHead == NULL) {
        p->lHead = newLog;
        p->lTail = newLog;
    } else {
        // Otherwise, append the new node to the tail
        p->lTail->next = newLog;
        newLog->prev = p->lTail;
        p->lTail = newLog; // Update the tail pointer
    }
}

Log *updateEntryLog(Log *l, char *date, char *time, Park *park) {

    // Allocate memory for exitDate and exitTime and copy the new values
    l->exitDate = createDateStruct(date);
    if (l->exitDate == NULL) {
        // Handle allocation failure
        return NULL;
    }

    l->exitTime = createTimeStruct(time);
    if (l->exitTime == NULL) {
        // Handle allocation failure
        free(l->exitDate);  // Free previously allocated memory
        return NULL;
    }

    // Update other fields of the Log struct
    l->value = calculateValue(l, park);
    l->type = 1;

    return l;
}

Log *changeLog(Vehicle *v, Park *p, char *d, char *t, int type) {
    if (type == 0) {
        Log *newLog = (Log *)malloc(sizeof(Log));
        if (newLog == NULL) {
            return NULL;
        }

        newLog->entryDate = createDateStruct(d);
        newLog->entryTime = createTimeStruct(t);
        newLog->type = 0;

        newLog->reg = v->registration;

        newLog->parkName = p->name;

        // Add the new log to vehicle's log list
        newLog = addLogVehicle(v, newLog);

        // Add the new log to park's log list
        addLogPark(p, newLog);

        return newLog;

    } else if (type == 1) {

        Log *l = updateEntryLog(v->lastLog, d, t, p);
        return l;
    }
    return NULL;
}

void commandS(ParkingSystem* system, Buffer* buffer) {
    char *name, *reg, *date, *time;

    name = nextWord(buffer);
    reg = nextWord(buffer);
    date = nextWord(buffer);
    time = nextWord(buffer);

    // Check if the exit is valid (registration is valid, time is valid)
    if (isValidRequest(system, name, reg, date, time, 1)) {
        Park *park = parkExists(system, name);
        Vehicle *vehicle = getVehicle(system, reg);
        exitPark(system, park, vehicle, date, time);
    } 
    free(name);
    free(reg);
    free(date);
    free(time);
}


int printVehicleLogs(ParkingSystem* system, char* reg) {
    Vehicle *v = getVehicle(system, reg);
    if (v == NULL) {
        printf("%s: no entries found in any parking.\n", reg);
        return 0;
    }
    LogNode *cur = v->lHead;
    // Sort the list by park names, since the logs are in order of entry
    if (cur == NULL) {
        printf("%s: no entries found in any parking.\n", reg);
        return 0;
    }
    cur = sortLogListName(v);
    int numLogs = 0;
    while (cur != NULL) {
        if (strcmp(cur->log->reg, reg) == 0) {
            // If its an entry log, print only entry info
            char *dEntry = dateToString(cur->log->entryDate);
            char *tEntry = timeToString(cur->log->entryTime);
            if (cur->log->type == 0) {
                printf("%s %s %s\n", cur->log->parkName, dEntry, tEntry);
                free(dEntry);
                free(tEntry);
            } else {
                // If its an exit log, print entry and exit info
                char *dExit = dateToString(cur->log->exitDate);
                char *tExit = timeToString(cur->log->exitTime);
                printf("%s %s %s %s %s\n", cur->log->parkName, 
                dEntry, 
                tEntry, 
                dExit, 
                tExit);
                free(dEntry);
                free(tEntry);
                free(dExit);
                free(tExit);
            }
            numLogs++;
        }
        cur = cur->next;
    }
    return numLogs;   
}

void commandV(ParkingSystem* system, Buffer* buffer) {
    char *reg;

    reg = nextWord(buffer);

    if (reg == NULL || !isValidRegistration(reg)) {
        printf("%s: invalid licence plate.\n", reg);
        return;
    }

    printVehicleLogs(system, reg);
    free(reg);
}

void printLogsByDate(LogNode *head) {
    LogNode *cur = head;
    int prevDay = -1;
    int prevMonth = -1;
    int prevYear = -1;
    double totalRevenue = 0.0;

    while (cur != NULL) {
        if (cur->log->type == 0) {
            cur = cur->next;
            continue;
        }
        int curDay = cur->log->exitDate->day;
        int curMonth = cur->log->exitDate->month;
        int curYear = cur->log->exitDate->year;

        // If the current date is different from the previous one, print accumulated revenue
        if ((curDay != prevDay || curMonth != prevMonth || curYear != prevYear) && prevDay != -1 && prevMonth != -1 && prevYear != -1) {
            printf("%02d-%02d-%04d %.2f\n", prevDay, prevMonth, prevYear, totalRevenue);
            totalRevenue = 0.0; // Reset accumulated revenue for the new date
        }

        // Accumulate revenue
        totalRevenue += cur->log->value;
        prevDay = curDay; // Update previous date
        prevMonth = curMonth;
        prevYear = curYear;
        cur = cur->next;
    }


    // Print the last date and accumulated revenue
    if (prevDay != -1 && prevMonth != -1 && prevYear != -1) {
        printf("%02d-%02d-%04d %.2f\n", prevDay, prevMonth, prevYear, totalRevenue);
    }
}


void showParkRevenue(Park* p, Date* date) {
    LogNode *cur;
    if (p->isSorted == 0) {
        cur = sortListExitDate(p);
        p->isSorted = 1;
    } else {
        cur = p->lHead;
    }

    if (cur == NULL) {
        return;
    }
    
    while (cur != NULL) {
        if (strcmp(cur->log->parkName, p->name) == 0) {
            if (date == NULL) {
                if (cur->log->type == 1) {
                    printLogsByDate(cur);
                    break;
                } 
            } else {
                if (isValidDate(date) && cur->log->type == 1) {
                    if (isLogDateValid(date, cur->log->exitDate) && isSameDate(cur->log->exitDate, date)) {
                        char *t = timeToString(cur->log->exitTime);
                        printf("%s %s %.2f\n", cur->log->reg, t, cur->log->value);
                        free(t);
                    }
                } 
            }
        }
        cur = cur->next;
    }
}

void printParkLogs(Park* p) {
    LogNode *cur = p->lHead;
    if (cur == NULL) {
        return;
    }
    while (cur != NULL) {
            if (cur->log->type == 1) {
                char *dEntry = dateToString(cur->log->entryDate);
                char *tEntry = timeToString(cur->log->entryTime);
                char *dExit = dateToString(cur->log->exitDate);
                char *tExit = timeToString(cur->log->exitTime);
                printf("%s %s %s %s %s %.2f\n", cur->log->reg, dEntry, tEntry, dExit, tExit, cur->log->value);
                free(dEntry);
                free(tEntry);
                free(dExit);
                free(tExit);
            }
        cur = cur->next;
    }
}

// Checks total revenue of a park
void commandF(ParkingSystem* system, Buffer* buffer) {
    char *name, *date;

    name = nextWord(buffer);
    date = nextWord(buffer);

    Park *park = parkExists(system, name);
    if (park == NULL) {
        printf("%s: no such parking.\n", name);
        return;
    }
    if (date == NULL) {
        showParkRevenue(park, NULL);
    } else {
        Date *d = createDateStruct(date);
        if (isValidDate(d) && isLogDateValid((d),system->lastDate)) {  // and logDateValid of last entry date
            showParkRevenue(park, d);
        } else {
            printf("invalid date.\n");
        }
        free(d);
    }
    free(name);
    free(date);
}

void freeHashTable(ParkingSystem *system) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        VehicleHashNode *cur = system->hashTable[i];
        while (cur != NULL) {
            VehicleHashNode *temp = cur;
            cur = cur->next;
            free(temp);
            temp = NULL;
        }
    }
}

void freeLogs(ParkingSystem *system) {
    ParkingNode *cur = system->pHead;
    while (cur != NULL) {
        LogNode *logCur = cur->parking->lHead;
        while (logCur != NULL) {
            LogNode *temp = logCur;
            logCur = logCur->next;
            free(temp->log->entryDate);
            temp->log->entryDate = NULL;
            free(temp->log->exitDate);
            temp->log->exitDate = NULL;
            free(temp->log->entryTime);
            temp->log->entryTime = NULL;    
            free(temp->log->exitTime);
            temp->log->exitTime = NULL;
            free(temp->log);
            temp->log = NULL;
            free(temp);
            temp = NULL;
        }
        cur = cur->next;
    }
}

void freeVehicles(ParkingSystem *system) {
    VehicleNode *cur = system->vHead;
    while (cur != NULL) {
        free(cur->vehicle->registration);
        cur->vehicle->registration = NULL;

        LogNode *logCur = cur->vehicle->lHead;
        while (logCur != NULL) {
            LogNode *temp = logCur;
            logCur = logCur->next;
            free(temp);
            temp = NULL;
        }

        free(cur->vehicle);
        cur->vehicle = NULL;
        VehicleNode *temp = cur;
        cur = cur->next;
        free(temp);
        temp = NULL;
    }
}

void freeParks(ParkingSystem *system) {
    ParkingNode *cur = system->pHead;
    while (cur != NULL) {
        free(cur->parking->name);
        cur->parking->name = NULL;
        free(cur->parking);
        cur->parking = NULL;
        ParkingNode *temp = cur;
        cur = cur->next;
        free(temp);
        temp = NULL;
    }
}

void terminate(ParkingSystem* system, Buffer* buffer) {
    free(system->lastDate);
    system->lastDate = NULL;
    free(system->lastTime);
    system->lastTime = NULL;

    // Free all the logs in the system
    freeLogs(system);
    freeVehicles(system);
    freeParks(system);
    freeHashTable(system);

    free(system);
    free(buffer->buffer);
    free(buffer);
}

int main() {
    // Initializes the buffer
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    ParkingSystem *system;
    buffer->buffer = (char *)malloc(BUFSIZE);

    system = init();
    if (system == NULL) {
        exit(1);
    }
    
    while (1) {
        buffer = getBuffer(buffer);

        switch (buffer->buffer[0]) {

            // Closes the program
            case 'q':
                terminate(system, buffer);

                return 0;

            // Creates a parking lot or lists the existing parking lots
            case 'p':
                buffer->index = 2;

                commandP(system, buffer);
                break;

            // Registers the entry of a vehicle
            case 'e':
                buffer->index = 2;

                commandE(system, buffer);

                break;

            // Registers the exit of a vehicle
            case 's':
                buffer->index = 2;

                commandS(system, buffer);
                
                break;

            // Lists the entries and exits of a vehicle
            case 'v':
                 buffer->index = 2;
                commandV(system, buffer);
                break;
            
            // Shows revenue of a parking lot
            case 'f':
                buffer->index = 2;
                commandF(system, buffer);

                break;

             case 'r':
                buffer->index = 2;
                commandR(system, buffer);

                break;
            default:
                break;
        }
    }
    return 0;
}