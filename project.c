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
    system->head = NULL;
    system->vehicles = NULL;
    system->numParks = 0;
    
    return system;
}

void printParks(ParkingSystem* system) {
    ParkingNode *current = system->head;
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
    ParkingNode *current = sys->head;
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
    ParkingNode *current = sys->head;
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
    if (system->head == NULL) {
        system->head = parking;
        parking->next = NULL;
    } else {
        ParkingNode *current = system->head;
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
    if (system->head == NULL) {
        return;
    }

    ParkingNode *current = system->head;
    ParkingNode *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->parking->name, name) == 0) {
            // If the node to be removed is the head of the list
            if (prev == NULL) {
                system->head = current->next;
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

    newParking->next = NULL;
    newParking->prev = NULL;


    strcpy(newParking->parking->name, name);
    newParking->parking->maxCapacity = atoi(maxCapacity);
    newParking->parking->currentLots = 0;
    double value15 = newParking->parking->billingValue15 = atof(billingValue15);
    double valueAfter1Hour = newParking->parking->billingValueAfter1Hour = atof(billingValueAfter1Hour);
    double maxDaily = newParking->parking->maxDailyValue = atof(maxDailyValue);

        // Check for invalid costs
    if (value15 <= 0 || valueAfter1Hour <= 0 || maxDaily <= 0 || value15 >= valueAfter1Hour || valueAfter1Hour >= maxDaily) {
        fprintf(stderr, "invalid cost.\n");
        return;
    }

    if (newParking->parking->maxCapacity <= 0) {
        fprintf(stderr, "invalid capacity.\n");
        free(newParking);
        return;
    }

    printf("Parking lot %s created\n", name);

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
    system->vehicles = newVehicle;

    newVehicle->next = NULL;
    newVehicle->prev = NULL;
    newVehicle->vehicle->parkName = NULL;
    newVehicle->vehicle->isParked = 1;
    strcpy(newVehicle->vehicle->registration, reg);
    newVehicle->vehicle->entryDate = NULL;
    newVehicle->vehicle->entryTime = NULL;
    newVehicle->vehicle->exitDate = NULL;
    newVehicle->vehicle->exitTime = NULL;

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
    if (isValidEntry(system, reg, date, time)) {
        Park *park = parkExists(system, name);
        Vehicle *vehicle = getVehicle(system, reg);
        if (vehicle == NULL) {
            createVehicle(system, reg);
        } else {
            if (vehicle->isParked == 1) {
                fprintf(stderr, "invalid vehicle entry.\n");
                return;
            }
            enterPark(park, vehicle, date, time);
        }
    }
}

int enterPark(Park *p, Vehicle *v, char *date, char *time) {
    v->parkName = p->name;
    strcpy(v->entryDate, date);
    strcpy(v->entryTime, time);

    p->currentLots++;
    p->regs[p->currentLots] = v->registration;

    return 0;
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