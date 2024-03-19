#include "project.h"
#include "constants.h"
#include "projectaux.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidTime(Time *time) {
    if (time->hour < 0 || time->hour > 23) {
        return 0;
    }
    if (time->minute < 0 || time->minute > 59) {
        return 0;
    }
    return 1;
}

int isValidDate(Date *date) {
    if (date->day < 1 || date->day > 31) {
        return 0;
    }
    if (date->month < 1 || date->month > 12) {
        return 0;
    }
    if (date->year < 0) {
        return 0;
    }

    // Check if it's February 29th and it's not a leap year
    if (date->month == 2 && date->day == 29 && !isLeapYear(date->year)) {
        return 0;
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

char *dateToString(Date *date) {
    char *dateString = (char *)malloc(11 * sizeof(char));
    if (dateString == NULL) {
        exit(1);
    }

    sprintf(dateString, "%02d-%02d-%04d", date->day, date->month, date->year); 
    return dateString;
}

char *timeToString(Time *time){
    char *timeString = (char *)malloc(6 * sizeof(char));
    if (timeString == NULL) {
        exit(1);
    }

    sprintf(timeString, "%02d:%02d", time->hour, time->minute);
    return timeString;
}

int isValidRequest(ParkingSystem *system, char *name, char *reg, char *date, char *time) {
    Park *park = parkExists(system, name);

    if (park == NULL) {
        fprintf(stderr, "no such parking.\n");
        return 0;
    }

    if (!isValidRegistration(reg)) {
        fprintf(stderr, "invalid licence plate.");
        return 0;
    }

    // Check if the vehicle is in the park if exit
    if (!isVehicleInPark(system, reg, name)) {
        fprintf(stderr, "invalid vehicle exit.");
    }

    Time *t = createTimeStruct(time);
    Date *d = createDateStruct(date);

    if (!isValidTime(t)) {
        fprintf(stderr, "invalid date.\n");
        return 0;
    }

    if (!isValidDate(d)) {
        fprintf(stderr, "invalid date.\n");
        return 0;
    }

    if (!isValidLog(system, t, d)) {
        fprintf(stderr, "invalid date.\n");
        return 0;
    }

    return 1;
}

int isValidRegistration(char *reg) {

    // Check if the length of the registration is valid
    if (strlen(reg) != REGISTRATION_LENGTH) {
        return 0;
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
            return 0;
        }
    }

    // Check if the registration contains at least one pair of letters and one pair of digits
    if (letterCount < 2 || digitCount < 2) {
        return 0;
    }

    // If all checks pass, the registration is valid
    return 1;
}

int isVehicleInPark(ParkingSystem *system, char *reg, char *name) {
    Vehicle *v = getVehicle(system, reg);
    if ((strcmp(v->parkName, name) == 0) && v->isParked) {
        return 1;
    }
    return 0;
}

// If d1 is sooner than d2, it's valid (1)
int isLogDateValid(Date *d1, Date *d2) {
    if (d1->year < d2->year) {
        return 1;
    } else if (d1->month < d2->month) {
        return 1;
    } else if (d1-> month == d2->month) {
        if (d1->day < d2->day) {
            return 1;

        } else if (d1->day == d2->day) {
            return 2;
        }
        return 0;
    }
    return 0;
}

// If t1 is sooner than t2, it's valid (1)
int isLogTimeValid(Time *t1, Time *t2) {
    if (t1->hour < t2->hour) {
        return 1;
    }   else if (t1->hour == t2->hour) {
        if (t1->minute < t2->minute) {
            return 1;
        }
        return 0;
    }
    return 0;
}

int isValidLogAux(Date *d1, Date *d2, Time *t1, Time *t2) {
    int val = isLogDateValid(d1, d2);
    if (val == 1) {
        return 1;
    } else if (val == 2) {
        if (isLogTimeValid(t1, t2) == 1) {
            return 1;
        }
        return 0;
    }
    return 0;

}

// Checks if the date and time of the last log in the system is sooner
int isValidLog(ParkingSystem *system, Time *time, Date *date) {
    // Go through logs linked list in system and find if the date and time are valid
    LogNode *cur = system->lHead;
    if (cur == NULL) {
        return 1;
    }

    while (cur != NULL) {
        // Last log 
        if (cur->prev != NULL && cur->next == NULL) {
            if (cur->log->type == 0) { // Last log is an entry
                return isValidLogAux(cur->log->entryDate, date, cur->log->entryTime, time);

            } else if (cur->log->type == 1) { // Last log is an exit
                return isValidLogAux(cur->log->exitDate, date, cur->log->exitTime, time);
            }
        } 
    }
    return 1;
}

Vehicle *getVehicle(ParkingSystem *system, char *reg) {
    VehicleNode *current = system->vHead;
    while (current != NULL) {
        if (strcmp(current->vehicle->registration, reg) == 0) {
            return current->vehicle;
        }
        current = current->next;
    }
    return NULL;
}

Log *findEntryLog(ParkingSystem *system, char *reg, char *name) {
    LogNode *current = system->lHead;
    while (current != NULL) {
        // type needs to be 0 (entry), so it doesn't return an exit log (old log)
        if (strcmp(current->log->reg, reg) == 0 && strcmp(current->log->parkName, name) == 0 && current->log->type == 0) {
            return current->log;
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