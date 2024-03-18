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
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    system->pHead = NULL;
    system->vHead = NULL;
    system->numParks = 0;
    
    return system;
}

void printParks(ParkingSystem* system) {
    ParkingNode *current = system->pHead;
    while (current != NULL) {
        printf("p %s %d %.2f %.2f %.2f\n", 
            current->parking->name, 
            current->parking->maxCapacity, 
            current->parking->billingValue15, 
            current->parking->billingValueAfter1Hour, 
            current->parking->maxDailyValue);
        current = current->next;
    }
}

/* Returns park, or Null if it doesn't exist */
Park* parkExists(ParkingSystem* sys, char* name) {
    ParkingNode *current = sys->pHead;
    while (current != NULL) {
        if (strcmp(current->parking->name, name) == 0) {
            return current->parking;
        }
        current = current->next;
    }
    return NULL;
}

/* Returns 1 if full, 0 if not full */
int isParkFull(ParkingSystem* sys, char* name) {
    ParkingNode *current = sys->pHead;
    while (current != NULL) {
        if (strcmp(current->parking->name, name) == 0) {
            if (current->parking->currentLots == current->parking->maxCapacity) {
                return 1;
            }
        }
        current = current->next;
    }
    return 0;
}

void addPark(ParkingSystem *system, ParkingNode *parking) {
    // Iterate through the list to find an empty spot
    if (system->pHead == NULL) {
        system->pHead = parking;
        parking->next = NULL;
    } else {
        ParkingNode *current = system->pHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = parking;
        parking->prev = current;
        parking->next = NULL;
    }
    system->numParks++;
}

void removePark(ParkingSystem *system, char *name) {
    if (system->pHead == NULL) {
        return;
    }

    ParkingNode *current = system->pHead;
    ParkingNode *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->parking->name, name) == 0) {
            // If the node to be removed is the head of the list
            if (prev == NULL) {
                system->pHead = current->next;
            } else {
                prev->next = current->next;
            }

            // If the node to be removed is not the last node in the list
            if (current->next != NULL) {
                current->next->prev = prev;
            }

            free(current->parking);
            free(current);
            system->numParks--;
            return;
        }
        prev = current;
        current = current->next;
    }
}


void createPark(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue) {
    ParkingNode *newParking = (ParkingNode *)malloc(sizeof(Park));

    if (newParking == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    newParking->parking = (Park *)malloc(sizeof(Park));
    if (newParking->parking == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(newParking);
        exit(1);
    }

    // Allocate memory for the name field and copy the string
    newParking->parking->name = (char *)malloc(strlen(name) + 1);
    if (newParking->parking->name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
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

    // Check for invalid costs
    if (newParking->parking->billingValue15 <= 0 || newParking->parking->billingValueAfter1Hour <= 0 ||
        newParking->parking->maxDailyValue <= 0 || newParking->parking->billingValue15 >= newParking->parking->billingValueAfter1Hour ||
        newParking->parking->billingValueAfter1Hour >= newParking->parking->maxDailyValue) {
        fprintf(stderr, "Invalid cost.\n");
        free(newParking->parking->name);
        free(newParking->parking);
        free(newParking);
        return;
    }

    if (newParking->parking->maxCapacity <= 0) {
        fprintf(stderr, "Invalid capacity.\n");
        free(newParking->parking->name);
        free(newParking->parking);
        free(newParking);
        return;
    }

    printf("t: Parking lot %s created\n", name);

    addPark(system, newParking);
}


int createVehicle(ParkingSystem *system, char *reg) {
    VehicleNode *newVehicle = (VehicleNode *)malloc(sizeof(VehicleNode));
    if (newVehicle == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    newVehicle->vehicle = (Vehicle *)malloc(sizeof(Vehicle));
    if (newVehicle->vehicle == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(newVehicle);
        exit(1);
    }
    system->vHead = newVehicle;

    newVehicle->next = NULL;
    newVehicle->prev = NULL;

    // Allocate memory for the registration field and copy the string
    newVehicle->vehicle->registration = (char *)malloc(strlen(reg) + 1);
    if (newVehicle->vehicle->registration == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(newVehicle->vehicle);
        free(newVehicle);
        exit(1);
    }
    strcpy(newVehicle->vehicle->registration, reg);

    newVehicle->vehicle->parkName = NULL;
    newVehicle->vehicle->isParked = 1;
    newVehicle->vehicle->date = NULL;
    newVehicle->vehicle->time = NULL;

    return 0;
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
            fprintf(stderr, "parking already exists.\n");
        } else {
            if (system->numParks < MAX_PARKING_LOTS) {
                // If the parking lot does not exist, create it
                maxCapacity = nextWord(buffer);
                billingValue15 = nextWord(buffer);
                billingValueAfter1Hour = nextWord(buffer);
                maxDailyValue = nextWord(buffer);
                createPark(system, name, maxCapacity, billingValue15, billingValueAfter1Hour, maxDailyValue);
            } else {
                fprintf(stderr, "too many parks.\n");
            }    
        }
    }
}

void commandR(ParkingSystem* system, Buffer* buffer) {
    char *name;

    name = nextWord(buffer);
    
    if (parkExists(system, name) != NULL) {
        removePark(system, name);
    }

}

void commandE(ParkingSystem* system, Buffer* buffer) {
    char *name, *reg, *date, *time;

    name = nextWord(buffer);
    reg = nextWord(buffer);
    date = nextWord(buffer);
    time = nextWord(buffer);

    // Check if the entry is valid (park exists, park is not full, registration is valid, time is valid)
    if (isValidEntry(system, name, reg, date, time)) {
        printf("t: Entry valida\n");
        Park *park = parkExists(system, name);
        Vehicle *vehicle = getVehicle(system, reg);
        if (vehicle == NULL) {
            createVehicle(system, reg);
        } else {
            if (vehicle->isParked == 1) {
                fprintf(stderr, "invalid vehicle entry.\n");
                return;
            }
            enterPark(system, park, vehicle, date, time);
        }
    }
}

int enterPark(ParkingSystem *sys, Park *p, Vehicle *v, char *date, char *time) {
    v->parkName = p->name;
    v->date = createDateStruct(date);
    v->time = createTimeStruct(time);

    p->currentLots++;
    createLog(sys, v->date, v->time, v->registration, p->name, 0);
    return 0;
}

int exitPark(ParkingSystem *system, Park *p, Vehicle *v, char *date, char *time) {
    v->parkName = NULL;
    v->date = createDateStruct(date);
    v->time = createTimeStruct(time);
    v->isParked = 0;

    p->currentLots--;
    createLog(system, v->date, v->time, v->registration, p->name, 1);
    return 0;
}

Log *createLog(ParkingSystem *system, Date *date, Time *time, char *reg, char *name, int type) {
    if (type == 0) {
        Log *newLog = (Log *)malloc(sizeof(Log));
        if (newLog == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        // Entry
        newLog ->entryDate = date;
        newLog ->entryTime = time;
        newLog ->type = 0;
        strcpy(newLog->reg, reg);
        strcpy(newLog->parkName, name);
        addLog(system, newLog);
        return newLog;
    } else {
        // Findd the entry log
        Log *l = findEntryLog(system, reg, name);
        if (l == NULL) {
            fprintf(stderr, "Invalid exit.\n");
            return NULL;
        }
        // Exit
        l->exitDate = date;
        l->exitTime = time;
        l->type = 1;
        return l;
    }
    return NULL;
}

void addLog(ParkingSystem *system, Log *l) {
    LogNode *newLog = (LogNode *)malloc(sizeof(LogNode));
    if (newLog == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newLog->log = l;
    newLog->next = NULL;
    newLog->prev = NULL;

    if (system->lHead == NULL) {
        system->lHead = newLog;
    } else {
        LogNode *current = system->lHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newLog;
        newLog->prev = current;
    }
}

void commandS(ParkingSystem* system, Buffer* buffer) {
    char *name, *reg, *date, *time;

    name = nextWord(buffer);
    reg = nextWord(buffer);
    date = nextWord(buffer);
    time = nextWord(buffer);

    // Check if the exit is valid (registration is valid, time is valid)
    if (isValidExit(system, name, reg, date, time)) {
        Park *park = parkExists(system, name);
        Vehicle *vehicle = getVehicle(system, reg);
        if (vehicle == NULL) {
            fprintf(stderr, "invalid license plate.\n");
            return;
        } else {
            if (vehicle->isParked == 0) {
                fprintf(stderr, "invalid vehicle exit.\n");
                return;
            }
            exitPark(system, park, vehicle, date, time);
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
                
                break;
            
            // Shows revenue of a parking lot
            case 'f':
                buffer->index = 2;

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