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
    system->numParks = 0;

    system->lastDate = malloc(sizeof(Date));
    system->lastTime = malloc(sizeof(Time));
    
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
void removeLogs(ParkingSystem *system, char *name) {
    Park *p = parkExists(system, name);
    LogNode *cur = p->lHead;
    while (cur != NULL) {
        removeLogsVehicle(getVehicle(system, cur->log->reg), name);
        LogNode *temp = cur;
        cur = cur->next;
        free(temp->log->reg);
        free(temp->log->parkName);
        free(temp->log->entryDate);
        free(temp->log->entryTime);
        free(temp->log->exitDate);
        free(temp->log->exitTime);
        free(temp->log);
        free(temp);
    }
    
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

void addVehicle(ParkingSystem *system, VehicleNode *vehicle) {
    // Iterate through the list to find an empty spot
    if (system->vHead == NULL) {
        system->vHead = vehicle;
        vehicle->next = NULL;
    } else {
        VehicleNode *cur = system->vHead;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = vehicle;
        vehicle->prev = cur;
        vehicle->next = NULL;
    }
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
    
    if (parkExists(system, name) != NULL) {
        removePark(system, name);
        removeLogs(system, name);
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
    changeLog(sys, v->date, v->time, v->registration, p->name, 0);
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
    Log *l = changeLog(system, v->date, v->time, v->registration, p->name, 1);
    if (l == NULL) {
        return 1;
    }
    printf("%s %s %s %s %s %.2f\n", v->registration, dateToString(l->entryDate), timeToString(l->entryTime), dateToString(l->exitDate), timeToString(l->exitTime), l->value);
    return 0;
}

// Logs are added to the list in order of entry
void addLogVehicle(Vehicle *v, Log *l) {
    LogNode *newLog = (LogNode *)malloc(sizeof(LogNode));
    if (newLog == NULL) {
        return;
    }
    newLog->log = l;
    newLog->next = NULL;
    newLog->prev = NULL;

    if (v->lHead == NULL) {
        v->lHead = (LogNode *)malloc(sizeof(LogNode));
        v->lHead = newLog;
    } else {
        LogNode *cur = v->lHead;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = newLog;
    }
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
    } else {
        LogNode *cur = p->lHead;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = newLog;
    }
}

void updateEntryLog(Log *l, Date *date, Time *time, Park *park) {
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
        return;
    }
    *l->exitDate = *date;  // Copy the new date

    l->exitTime = (Time *)malloc(sizeof(Time));
    if (l->exitTime == NULL) {
        // Handle allocation failure
        free(l->exitDate);  // Free previously allocated memory
        l->exitDate = NULL; // Set to NULL to indicate it's freed
        return;
    }
    *l->exitTime = *time;  // Copy the new time

    l->value = calculateValue(l, park);
    l->type = 1;
}


Log *changeLog(ParkingSystem *system, Date *date, Time *time, char *reg, char *name, int type) {
    if (type == 0) {
        Log *newLog = (Log *)malloc(sizeof(Log));
        if (newLog == NULL) {
            return NULL;
        }
        // Entry
        newLog->entryDate = date;
        newLog->entryTime = time;
        newLog->type = 0;

        // Allocate memory for reg and copy the string
        newLog->reg = (char *)malloc(strlen(reg) + 1);
        if (newLog->reg == NULL) {
            free(newLog);  // Free Log struct if allocation fails
            return NULL;
        }
        strcpy(newLog->reg, reg);

        // Allocate memory for parkName and copy the string
        newLog->parkName = (char *)malloc(strlen(name) + 1);
        if (newLog->parkName == NULL) {
            
            free(newLog->reg);  // Free reg if allocation fails
            free(newLog);       // Free Log struct if allocation fails
            return NULL;
        }
        strcpy(newLog->parkName, name);

        addLogVehicle(getVehicle(system, reg), newLog);
        addLogPark(parkExists(system, name), newLog);
        return newLog;

    } else if (type == 1) {
        // Find the entry log
        Log *l1 = (Log *)malloc(sizeof(Log));
        l1 = findEntryLogVehicle(system, reg, name);

        if (l1 == NULL) {
            return NULL;
        }

        // Find the exit log
        Log *l2 = (Log *)malloc(sizeof(Log));
        l2 = findEntryLogPark(system, reg, name);
        if (l2 == NULL) {
            return NULL;
        }
        updateEntryLog(l1, date, time, parkExists(system, name));
        updateEntryLog(l2, date, time, parkExists(system, name));
        return l1;
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
    LogNode *cur = v->lHead;
    // Sort the list by park names, since the logs are in order of entry
    cur = sortLogListName(v);
    if (cur == NULL) {
        printf("%s: no entries found in any parking.\n", reg);
        return 0;
    }
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

void showParkRevenue(Park* p, Date* date) {

    // Sort the list by exit date
    sortListExitDate(p);

        LogNode *cur = p->lHead;
    if (cur == NULL) {
        return;
    }
    
    while (cur != NULL) {
        if (strcmp(cur->log->parkName, p->name) == 0) {
            if (date == NULL) {
                if (cur->log->type == 1 && isSameDate(cur->log->exitDate, date)) {
                    printf("%s %.2f\n", dateToString(cur->log->exitDate), cur->log->value);
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
        if (isValidDate(createDateStruct(date))) {  // and logDateValid of last entry date
            showParkRevenue(park, createDateStruct(date));
        } else {
            printf("invalid date.\n");
        }
        
    }
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
                
                free(system);
                free(buffer->buffer);
                free(buffer);

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
                printf("Invalid command\n");
        }
    }
    return 0;
}