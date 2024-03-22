#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "project.h"
#include "constants.h"
#include "projectaux.h"

int bufferIndex = 0;


/*
TO-DO
correct print statements ( and check their order )
define constants for error messages


*/

ParkingSystem* init() {
    ParkingSystem *system = (ParkingSystem *)malloc(sizeof(ParkingSystem));
    if (system == NULL) {
        exit(1);
    }
    system->pHead = NULL;
    system->vHead = NULL;
    system->numParks = 0;
    
    return system;
}

void printParks(ParkingSystem* system) {
    ParkingNode *cur = system->pHead;
    while (cur != NULL) {
        printf("p %s %d %.2f %.2f %.2f\n", 
            cur->parking->name, 
            cur->parking->maxCapacity, 
            cur->parking->billingValue15, 
            cur->parking->billingValueAfter1Hour, 
            cur->parking->maxDailyValue);
        cur = cur->next;
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

void addPark(ParkingSystem *system, ParkingNode *parking) {
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

void removeLogs(ParkingSystem *system, char *name) {
    LogNode *cur = system->lHead;
    LogNode *prev = NULL;
    while (cur != NULL) {
        if (strcmp(cur->log->parkName, name) == 0) {
            // If the node to be removed is the head of the list
            if (prev == NULL) {
                system->lHead = cur->next;
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

void removePark(ParkingSystem *system, char *name) {
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

            free(cur->parking);
            free(cur);
            system->numParks--;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}


void createPark(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue) {
    ParkingNode *newParking = (ParkingNode *)malloc(sizeof(Park));

    if (newParking == NULL) {
        exit(1);
    }

    newParking->parking = (Park *)malloc(sizeof(Park));
    if (newParking->parking == NULL) {
        free(newParking);
        exit(1);
    }

    // Allocate memory for the name field and copy the string
    newParking->parking->name = (char *)malloc(strlen(name) + 1);
    if (newParking->parking->name == NULL) {
        free(newParking->parking);
        free(newParking);
        exit(1);
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

    printf("t: Parking lot %s created\n", name);

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
    VehicleNode *newVehicle = (VehicleNode *)malloc(sizeof(VehicleNode));
    if (newVehicle == NULL) {
        exit(1);
    }

    newVehicle->vehicle = (Vehicle *)malloc(sizeof(Vehicle));
    if (newVehicle->vehicle == NULL) {
        
        free(newVehicle);
        exit(1);
    }
    system->vHead = newVehicle;

    newVehicle->next = NULL;
    newVehicle->prev = NULL;

    // Allocate memory for the registration field and copy the string
    newVehicle->vehicle->registration = (char *)malloc(strlen(reg) + 1);
    if (newVehicle->vehicle->registration == NULL) {
        
        free(newVehicle->vehicle);
        free(newVehicle);
        exit(1);
    }
    strcpy(newVehicle->vehicle->registration, reg);

    newVehicle->vehicle->parkName = NULL;
    newVehicle->vehicle->isParked = 1;
    newVehicle->vehicle->date = NULL;
    newVehicle->vehicle->time = NULL;

    return newVehicle->vehicle;
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

void commandR(ParkingSystem* system, Buffer* buffer) {
    char *name;

    name = nextWord(buffer);
    
    if (parkExists(system, name) != NULL) {
        removePark(system, name);
        removeLogs(system, name);
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
            printf("%s %d.\n", name, park->maxCapacity - park->currentLots);
        } 
    }
}

int enterPark(ParkingSystem *sys, Park *p, Vehicle *v, char *date, char *time) {
    // Allocate memory for park name in vehicle
    v->parkName = (char *)malloc(strlen(p->name) + 1);
    if (v->parkName == NULL) {
        
        exit(1);
    }
    strcpy(v->parkName, p->name);

    
    // Allocate memory for date and time
    if (v->date == NULL) {
        v->date = createDateStruct(date);
    }

    if (v->time == NULL) {
        v->time = createTimeStruct(time);
    }

    p->currentLots++;
    printf("t: Vehicle %s entered parking lot %s\n", v->registration, p->name);
    changeLog(sys, v->date, v->time, v->registration, p->name, 0);

    return 0;
}

int exitPark(ParkingSystem *system, Park *p, Vehicle *v, char *date, char *time) {
    v->parkName = NULL;
    v->date = createDateStruct(date);
    v->time = createTimeStruct(time);
    v->isParked = 0;

    p->currentLots--;
    Log *l = changeLog(system, v->date, v->time, v->registration, p->name, 1);
    if (l == NULL) {
        return 1;
    }
    printf("%s %s %s %s %s %.2f.\n", v->registration, dateToString(l->entryDate), timeToString(l->entryTime), dateToString(l->exitDate), timeToString(l->exitTime), l->value);
    return 0;
}

Log *changeLog(ParkingSystem *system, Date *date, Time *time, char *reg, char *name, int type) {
    if (type == 0) {
        Log *newLog = (Log *)malloc(sizeof(Log));
        if (newLog == NULL) {
            
            exit(1);
        }
        // Entry
        newLog->entryDate = date;
        newLog->entryTime = time;
        newLog->type = 0;

        // Allocate memory for reg and copy the string
        newLog->reg = (char *)malloc(strlen(reg) + 1);
        if (newLog->reg == NULL) {
            
            free(newLog);  // Free Log struct if allocation fails
            exit(1);
        }
        strcpy(newLog->reg, reg);

        // Allocate memory for parkName and copy the string
        newLog->parkName = (char *)malloc(strlen(name) + 1);
        if (newLog->parkName == NULL) {
            
            free(newLog->reg);  // Free reg if allocation fails
            free(newLog);       // Free Log struct if allocation fails
            exit(1);
        }
        strcpy(newLog->parkName, name);

        addLog(system, newLog);
        return newLog;
    } else if (type == 1) {
        // Find the entry log
        Log *l = findEntryLog(system, reg, name);
        if (l == NULL) {
            fprintf(stderr, "invalid exit.\n");
            return NULL;
        }
        // Exit
        l->exitDate = date;
        l->exitTime = time;
        l->value = calculateValue(l, system);
        l->type = 1;
        return l;
    }
    return NULL;
}


void addLog(ParkingSystem *system, Log *l) {
    LogNode *newLog = (LogNode *)malloc(sizeof(LogNode));
    if (newLog == NULL) {
        
        exit(1);
    }
    newLog->log = l;
    newLog->next = NULL;
    newLog->prev = NULL;

    if (system->lHead == NULL) {
        system->lHead = newLog;
    } else {
        LogNode *cur = system->lHead;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = newLog;
        newLog->prev = cur;
    }
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
            if (vehicle->isParked == 0) {
                fprintf(stderr, "invalid vehicle exit.\n");
                return;
            }
            exitPark(system, park, vehicle, date, time);
    } 
}

// TO-DO: SORT LOGS BY PARK NAME, DATE, TIME
int printVehicleLogs(ParkingSystem* system, char* reg) {
    LogNode *cur = system->lHead;
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
        }
        cur = cur->next;
    }
    return numLogs;   
}

void commandV(ParkingSystem* system, Buffer* buffer) {
    char *reg;

    reg = nextWord(buffer);

    if (reg == NULL || !isValidRegistration(reg)) {
        printf("%s: invalid license plate.\n", reg);
        return;
    }

    printVehicleLogs(system, reg);
}

void showParkRevenue(ParkingSystem* system, Park* p, Date* date) {

    LogNode *cur = system->lHead;
    if (cur == NULL) {
        return;
    }
    
    while (cur != NULL) {
        if (strcmp(cur->log->parkName, p->name) == 0) {
            if (date == NULL) {
                if (cur->log->type == 1) {
                    printf("%s %.2f\n", dateToString(cur->log->exitDate), cur->log->value);
                }
            } else {
                // TO-DO: CHECK IF DATE IS VALID (invalid ou posterior ao dia do ultimo reg saida ou entrada)
                if (isValidDate(date) && isLogDateValid(date, cur->log->exitDate)) {
                    if (cur->log->type == 1) {
                        printf("%s %s %.2f\n", cur->log->reg, timeToString(cur->log->exitTime), cur->log->value);
                    }
                } else {
                    fprintf(stderr, "invalid date.\n");
                }
            }
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
        showParkRevenue(system, park, NULL);
    } else {
        if (isValidDate(createDateStruct(date))) {  // and logDateValid of last entry date
            showParkRevenue(system, park, createDateStruct(date));
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