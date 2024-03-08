#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "project.h"

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


/*
 * Reads the input from the user and inserts it into the buffer.
 */
Buffer *getBuffer(Buffer *buffer) {
    char c;
    int count;

    memset(buffer->buffer, 0, BUFSIZE);
    buffer->index = 0;

    for (count = 0; (c = getchar()) != '\n'; count++) {
        buffer->buffer[count] = c;
    }

    return buffer;
}


char *nextWord(Buffer *buffer) {
    int i = 0;
    char *next_word = (char *)malloc(strlen(buffer->buffer) * sizeof(char));

    if (next_word == NULL) {
        exit(1);
    }

    while (buffer->buffer[buffer->index] == ' ' || buffer->buffer[buffer->index] == '\n') {
        buffer->index++;
    }

    if (buffer->buffer[buffer->index] == '"') {
        buffer->index++;
        while (buffer->buffer[buffer->index] != '"') {
            next_word[i] = buffer->buffer[buffer->index];
            i++, buffer->index++;
        }
        buffer->index++;
    } else {
        while (buffer->buffer[buffer->index] != ' ' && buffer->buffer[buffer->index] != '\n' &&
               buffer->buffer[buffer->index] != '\0') {
            next_word[i] = buffer->buffer[buffer->index];
            i++, buffer->index++;
        }
    }
    next_word[i] = '\0';

    if (i == 0) {
        free(next_word);
        return NULL;
    } else {
        return next_word;
    }
}

void printParkingLots(ParkingSystem* system) {
    ParkingNode *current = system->head;
    while (current != NULL) {
        printf("Parking lot: %s\n", current->parking->name);
        printf("Max capacity: %d\n", current->parking->maxCapacity);
        printf("Billing value for 15 minutes: %.2f\n", current->parking->billingValue15);
        printf("Billing value after 1 hour: %.2f\n", current->parking->billingValueAfter1Hour);
        printf("Max daily value: %.2f\n", current->parking->maxDailyValue);
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
    if (system->numParkingLots == 0) {
        system->head = parking;
        parking->prev = NULL;
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
    printf("n: %s\n", name);

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