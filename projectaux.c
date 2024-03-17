#include "project.h"
#include "constants.h"
#include "projectaux.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidTime(Time *time) {
    if (time->hour < 0 || time->hour > 23) {
        return -1;
    }
    if (time->minute < 0 || time->minute > 59) {
        return -1;
    }
    return 1;
}

int isValidDate(Date *date) {
    if (date->day < 1 || date->day > 31) {
        return -1;
    }
    if (date->month < 1 || date->month > 12) {
        return -1;
    }
    if (date->year < 0) {
        return -1;
    }

    // Check if it's February 29th and it's not a leap year
    if (date->month == 2 && date->day == 29 && !isLeapYear(date->year)) {
        return -1;
    }

    return 1;
}

int isLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        return 1;
    } else {
        return 0;
    }
}

int isValidEntry(ParkingSystem *system, char *name, char *reg, char *date, char *time) {
    Park *park = parkExists(system, name);

    if (park == NULL) {
        fprintf(stderr, "no such parking.\n");
        return -1;
    }

    if (isParkFull(system, name) == 1){
        fprintf(stderr, "parking is full.\n");
        return -1;
    }

    if (isValidRegistration(reg) == -1) {
        fprintf(stderr, "invalid licence plate.\n");
        return -1; 
    } 

    Time *t = createTimeStruct(time);
    Date *d = createDateStruct(date);

    if (isValidTime(t) == -1) {
        fprintf(stderr, "invalid time.\n");
        return -1;
    }

    if (isValidDate(d) == -1) {
        fprintf(stderr, "invalid date.\n");
        return -1;
    }

    return 1;
}

int isValidRegistration(char *reg) {

    // Check if the length of the registration is valid
    if (strlen(reg) != REGISTRATION_LENGTH) {
        return -1;
    }

    // Initialize counts for letters and digits
    int letterCount = 0;
    int digitCount = 0;
    int letterGroup = 0;
    int digitGroup = 0;

    // Iterate through each character in the registration
    for (int i = 0; i < REGISTRATION_LENGTH; i++) {
        char cur = reg[i];

        // Check if the current character is a letter (uppercase)
        if (cur >= 'A' && cur <= 'Z') {
            letterCount++;
            if (!letterGroup) {
                letterGroup = 1;
            }
        }
        // Check if the current character is a digit
        else if (cur >= '0' && cur <= '9') {
            digitCount++;
            if (!digitGroup) {
                digitGroup = 1;
            }
        }
        // Check if the current character is a hyphen
        else if (cur != '-') {
            // If the character is not a hyphen, it must be either a letter or a digit
            return -1;
        }
    }

    // Check if the registration contains at least one pair of letters and one pair of digits
    if (letterCount < 2 || digitCount < 2) {
        return -1;
    }

    // If all checks pass, the registration is valid
    return 1;
}

Vehicle *getVehicle(ParkingSystem *system, char *reg) {
    VehicleNode *current = system->vehicles;
    while (current != NULL) {
        if (strcmp(current->vehicle->registration, reg) == 0) {
            return current->vehicle;
        }
        current = current->next;
    }
    return NULL;
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

Date *createDateStruct(char *date) {
    Date *d = (Date *)malloc(sizeof(Date));
    if (d == NULL) {
        exit(1);
    }

    char *day = strtok(date, "-");
    char *month = strtok(NULL, "-");
    char *year = strtok(NULL, "-");

    d->day = atoi(day);
    d->month = atoi(month);
    d->year = atoi(year);

    return d;
}

Time *createTimeStruct(char *time) {
    Time *t = (Time *)malloc(sizeof(Time));
    if (t == NULL) {
        exit(1);
    }

    char *hour = strtok(time, ":");
    char *minute = strtok(NULL, ":");

    t->hour = atoi(hour);
    t->minute = atoi(minute);

    return t;
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