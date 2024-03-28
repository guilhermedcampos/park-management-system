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
    system->numParks = 0;

    // Initialize lastDate and lastTime
    system->lastDate = (Date *)malloc(sizeof(Date));
    if (system->lastDate == NULL) {
        free(system);
        return NULL;
    }
    system->lastDate->day = 0;
    system->lastDate->month = 0;
    system->lastDate->year = 0;

    system->lastTime = (Time *)malloc(sizeof(Time));
    if (system->lastTime == NULL) {
        free(system->lastDate);
        free(system);
        return NULL;
    }
    system->lastTime->hour = 0;
    system->lastTime->minute = 0;

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

// Removes all logs of a vehicle in specific park
void removeLogsVehicle(Vehicle *v, char* parkName) {
    LogNode *cur = v->lHead;
    LogNode *prev = NULL;
    while (cur != NULL) {
        if (strcmp(cur->log->parkName, parkName) == 0) {
            // If the node to be removed is the head of the list
            if (prev == NULL) {
                v->lHead = cur->next;
            } else {
                prev->next = cur->next;
            }

            // If the node to be removed is not the last node in the list
            if (cur->next != NULL) {
                cur->next->prev = prev;
            }

            free(cur->log->reg);
            free(cur->log->parkName);
            free(cur->log->entryDate);
            free(cur->log->entryTime);
            free(cur->log->exitDate);
            free(cur->log->exitTime);
            free(cur->log);

            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}


// Removes all logs of a park
void removeLogs(ParkingSystem *system, Park *p) {
    freeParkLogs(system, p);
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

    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        if (system->parks[i] != NULL && strcmp(system->parks[i]->name, name) == 0) {
            // First element of the list, shift left the array after removing first element
            updateParksArray(system, i);
        }
    }


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
            free(cur->parking);
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void createPark(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue) {
    ParkingNode *newParking = (ParkingNode *)malloc(sizeof(Park));

    if (newParking == NULL) {
        return;
    }

    newParking->parking = (Park *)malloc(sizeof(Park));
    if (newParking->parking == NULL) {
        free(newParking);
        return;
    }

    // Allocate memory for the name field and copy the string
    newParking->parking->name = (char *)malloc(strlen(name) + 1);
    if (newParking->parking->name == NULL) {
        free(newParking->parking);
        free(newParking);
        return;
    }
    strcpy(newParking->parking->name, name);

    newParking->next = NULL;
    newParking->prev = NULL;

    // Assign other fields
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

void addVehicle(ParkingSystem *system, VehicleNode *vehicle) {
    // Initialize the vehicle node pointers
    vehicle->prev = NULL;
    vehicle->next = NULL;

    if (system->vHead == NULL) {
        // If the list is empty, set both head and tail to the new node
        system->vHead = vehicle;
        system->vTail = vehicle;
    } else {
        // Otherwise, append the new node to the tail
        system->vTail->next = vehicle;
        vehicle->prev = system->vTail;
        system->vTail = vehicle; // Update the tail pointer
    }
    addToHashTable(system, vehicle->vehicle);
}

Vehicle *createVehicle(ParkingSystem *system, char *reg) {
    VehicleNode *newVehicleNode = (VehicleNode *)malloc(sizeof(VehicleNode));
    if (newVehicleNode == NULL) {
        return NULL;
    }

    newVehicleNode->vehicle = (Vehicle *)malloc(sizeof(Vehicle));
    if (newVehicleNode->vehicle == NULL) {
        free(newVehicleNode);
        return NULL;
    }

    // Allocate memory for the registration field and copy the string
    newVehicleNode->vehicle->registration = (char *)malloc(strlen(reg) + 1);
    if (newVehicleNode->vehicle->registration == NULL) {
        free(newVehicleNode->vehicle);
        free(newVehicleNode);
        return NULL;
    }
    strcpy(newVehicleNode->vehicle->registration, reg);

    // Initialize other fields
    newVehicleNode->vehicle->parkName = NULL;
    newVehicleNode->vehicle->isParked = 0;
    newVehicleNode->vehicle->date = NULL;
    newVehicleNode->vehicle->time = NULL;
    newVehicleNode->vehicle->lHead = (LogNode *)malloc(sizeof(LogNode));
    newVehicleNode->vehicle->lHead = NULL;
    newVehicleNode->vehicle->lTail = (LogNode *)malloc(sizeof(LogNode));
    newVehicleNode->vehicle->lTail = NULL;

    addVehicle(system, newVehicleNode);
    return newVehicleNode->vehicle;
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
        }
    }
}

void printRemainingParks(ParkingSystem* system) {
    ParkingNode *sortedNode = malloc(sizeof(ParkingNode));
    // Sort the list by park names alphabetically
    sortedNode = sortListName(system);
    ParkingNode *cur = sortedNode;
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
        removeLogs(system, p);
        removePark(system, name);
        printRemainingParks(system);
    } else {
        printf("%s: no such parking.\n", name);
    }
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
}

int enterPark(ParkingSystem *sys, Park *p, Vehicle *v, char *date, char *time) {
    // Allocate memory for park name in vehicle
    v->parkName = (char *)malloc(strlen(p->name) + 1);
    if (v->parkName == NULL) {
        return 0;
    }
    strcpy(v->parkName, p->name);
    
    v->date = createDateStruct(date);
    v->time = createTimeStruct(time);

    sys->lastDate = v->date;
    sys->lastTime = v->time;
    v->isParked = 1;
    p->currentLots++;
    v->lastLog = (Log *)malloc(sizeof(Log));
    v->lastLog = changeLog(v, p, 0);
    return 0;
}

int exitPark(ParkingSystem *system, Park *p, Vehicle *v, char *date, char *time) {
    free(v->parkName);
    v->parkName = NULL;
    v->date = createDateStruct(date);
    v->time = createTimeStruct(time);
    v->isParked = 0;

    if (v->date == NULL || v->time == NULL) {
        return 1;
    }

    system->lastDate = v->date;
    system->lastTime = v->time;
    p->currentLots--;
    Log *l = changeLog(v, p, 1);
    if (l == NULL) {
        return 1;
    }
    printf("%s %s %s %s %s %.2f\n", v->registration, dateToString(l->entryDate), timeToString(l->entryTime), dateToString(l->exitDate), timeToString(l->exitTime), l->value);
    return 0;
}

// Logs are added to the list in order of entry
Log *addLogVehicle(Vehicle *v, Log *l) {
    LogNode *newLog = (LogNode *)malloc(sizeof(LogNode));
    if (newLog == NULL) {
        return NULL;
    }
    
    newLog->log = (Log *)malloc(sizeof(Log));
    if (newLog->log == NULL) {
        free(newLog);
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

Log *updateEntryLog(Log *l, Date *date, Time *time, Park *park) {
    // Free the memory for exitDate and exitTime if they are already allocated
    if (l->exitDate != NULL) {
        free(l->exitDate);
        l->exitDate = NULL;  // Set to NULL to indicate it's freed
    }
    if (l->exitTime != NULL) {
        free(l->exitTime);
        l->exitTime = NULL;  // Set to NULL to indicate it's freed
    }

    // Allocate memory for exitDate and exitTime and copy the new values
    l->exitDate = (Date *)malloc(sizeof(Date));
    if (l->exitDate == NULL) {
        // Handle allocation failure
        return NULL;
    }
    l->exitDate = date;  // Copy the new date

    l->exitTime = (Time *)malloc(sizeof(Time));
    if (l->exitTime == NULL) {
        // Handle allocation failure
        free(l->exitDate);  // Free previously allocated memory
        l->exitDate = NULL; // Set to NULL to indicate it's freed
        return NULL;
    }
    l->exitTime = time;  // Copy the new time

    l->value = calculateValue(l, park);
    l->type = 1;
    return l;
}


Log *changeLog(Vehicle *v, Park *p, int type) {
    if (type == 0) {
        Log *newLog = (Log *)malloc(sizeof(Log));
        if (newLog == NULL) {
            return NULL;
        }

        newLog->entryDate = (Date *)malloc(sizeof(Date));
        if (newLog->entryDate == NULL) {
            free(newLog);
            return NULL;
        }

        newLog->entryTime = (Time *)malloc(sizeof(Time));
        if (newLog->entryTime == NULL) {
            free(newLog->entryDate);
            free(newLog);
            return NULL;
        }

        // Entry
        newLog->entryDate = v->date;
        newLog->entryTime = v->time;
        newLog->type = 0;

        // Allocate memory for reg and copy the string
        newLog->reg = (char *)malloc(strlen(v->registration) + 1);
        if (newLog->reg == NULL) {
            free(newLog->entryDate);  // Free entryDate if allocation fails
            free(newLog->entryTime);  // Free entryTime if allocation fails
            free(newLog);  // Free Log struct if allocation fails
            return NULL;
        }
        strcpy(newLog->reg, v->registration);

        // Allocate memory for parkName and copy the string
        newLog->parkName = (char *)malloc(strlen(p->name) + 1);
        if (newLog->parkName == NULL) {
            free(newLog->entryDate);  // Free entryDate if allocation fails
            free(newLog->entryTime);  // Free entryTime if allocation fails
            free(newLog->reg);  // Free reg if allocation fails
            free(newLog);       // Free Log struct if allocation fails
            return NULL;
        }
        strcpy(newLog->parkName, p->name);

        newLog = addLogVehicle(v, newLog);
        addLogPark(p, newLog);
        return newLog;

    } else if (type == 1) {

        Log *l = updateEntryLog(v->lastLog, v->date, v->time, p);
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
            if (cur->log->type == 0) {
                printf("%s %s %s\n", cur->log->parkName, dateToString(cur->log->entryDate), timeToString(cur->log->entryTime));
            } else {
                printf("%s %s %s %s %s\n", cur->log->parkName, 
                dateToString(cur->log->entryDate), 
                timeToString(cur->log->entryTime), 
                dateToString(cur->log->exitDate), 
                timeToString(cur->log->exitTime));
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
}

void printLogsByDate(LogNode *head) {
    LogNode *cur = head;
    char *prevDate = NULL;
    double totalRevenue = 0.0;

    while (cur != NULL) {
        if (cur->log->type == 0) {
            cur = cur->next;
            continue;
        }
        char *curDate = dateToString(cur->log->exitDate);

        // If the current date is different from the previous one, print accumulated revenue
        if (prevDate != NULL && strcmp(curDate, prevDate) != 0) {
            printf("%s %.2f\n", prevDate, totalRevenue);
            totalRevenue = 0.0; // Reset accumulated revenue for the new date
        }

        // Accumulate revenue
        totalRevenue += cur->log->value;
        prevDate = curDate; // Update previous date

        cur = cur->next;
    }

    // Print the last date and accumulated revenue
    if (prevDate != NULL) {
        printf("%s %.2f\n", prevDate, totalRevenue);
    }
}


void showParkRevenue(Park* p, Date* date) {
    LogNode *cur = sortListExitDate(p);

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
                        printf("%s %s %.2f\n", cur->log->reg, timeToString(cur->log->exitTime), cur->log->value);
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
                printf("%s %s %s %s %s %.2f\n", cur->log->reg, dateToString(cur->log->entryDate), timeToString(cur->log->entryTime), dateToString(cur->log->exitDate), timeToString(cur->log->exitTime), cur->log->value);
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
        if (isValidDate(createDateStruct(date)) && isLogDateValid((createDateStruct(date)),system->lastDate)) {  // and logDateValid of last entry date
            showParkRevenue(park, createDateStruct(date));
        } else {
            printf("invalid date.\n");
        }
        
    }
}

void terminate(ParkingSystem* system, Buffer* buffer) {
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