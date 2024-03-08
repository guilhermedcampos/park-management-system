#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "project.h"
#include "constants.h"
#include "projectaux.h"

int bufferIndex = 0;

ParkingSystem* init() {
    ParkingSystem *system = (ParkingSystem *)malloc(sizeof(ParkingSystem));
    if (system == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    system->head = NULL;
    system->numParkingLots = 0;
    return system;
}

void printParkingLots(ParkingSystem* system) {
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

int parkingExists(ParkingSystem* sys, char* name) {
    ParkingNode *current = sys->head;
    while (current != NULL) {
        if (strcmp(current->parking->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void addParking(ParkingSystem *system, ParkingNode *parking) {
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
    system->numParkingLots++;
}


void createParkingLot(ParkingSystem *system, char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue) {
    ParkingNode *newParking = (ParkingNode *)malloc(sizeof(ParkingLot));

    if (newParking == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    newParking->parking = (ParkingLot *)malloc(sizeof(ParkingLot));
    if (newParking->parking == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(newParking);
        exit(1);
    }

    newParking->next = NULL;
    newParking->prev = NULL;


    strcpy(newParking->parking->name, name);
    newParking->parking->maxCapacity = atoi(maxCapacity);
    newParking->parking->billingValue15 = atof(billingValue15);
    newParking->parking->billingValueAfter1Hour = atof(billingValueAfter1Hour);
    newParking->parking->maxDailyValue = atof(maxDailyValue);

    printf("Parking lot %s created\n", name);

    addParking(system, newParking);

}

void commandP(ParkingSystem* system, Buffer* buffer) {   
    char *name, *maxCapacity, *billingValue15, *billingValueAfter1Hour, *maxDailyValue;
    int exists = 0;

    name = nextWord(buffer);

    if (name == NULL) {
        // If there are no more arguments, list the parking lots
        printParkingLots(system);

    } else {
        
        // Checks if the parking lot already exists
        if ((exists = parkingExists(system, name)) == 1) {
            fprintf(stderr, "Parking lot already exists\n");

        } else {

            // If the parking lot does not exist, create it
            maxCapacity = nextWord(buffer);
            billingValue15 = nextWord(buffer);
            billingValueAfter1Hour = nextWord(buffer);
            maxDailyValue = nextWord(buffer);
            createParkingLot(system, name, maxCapacity, billingValue15, billingValueAfter1Hour, maxDailyValue);
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
            default:
                printf("Invalid command\n");
        }
    }
    return 0;
}