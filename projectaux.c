#include "project.h"
#include "constants.h"
#include "projectaux.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidParkRequest(int nParks, int cap, double x, double y, double z) {

    if (cap == 0) {
        fprintf(stderr, "%d invalid capacity.\n", cap);
        return 0;
    }

    // Check for invalid costs
    if (x <= 0 || y <= 0 || z <= 0 || x >= y|| y>= z) {
        fprintf(stderr, "invalid cost.\n");
        return 0;
    }

    if (nParks == MAX_PARKING_LOTS ) {
        fprintf(stderr, "too many parks.\n");
        return 0;
    }

    return 1;
}

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

int isValidRequest(ParkingSystem *system, char *name, char *reg, char *date, char *time, int type) {
    Park *park = parkExists(system, name);

    if (park == NULL) {
        printf("%s: no such parking.\n", name);
        return 0;
    }

    if (type == 0) {
        if (park->currentLots == park->maxCapacity) {
            printf("%s: parking full.\n", park->name);
            return 0;
        }
    }

    if (!isValidRegistration(reg) || (!isValidRegistration(reg) && type == 1)) {
        printf("%s: invalid licence plate.\n", reg);
        return 0;
    }

    if (type == 0) {
        if (isVehicleInPark(system, reg, name)) {
            printf("%s:invalid vehicle entry.\n", reg);
            return 0;
        }

    }

    if (type == 1) {
        if (!isVehicleInPark(system, reg, name)) {
            printf("%s: invalid vehicle exit.\n", reg);
            return 0;
        }
    }

    Time *t = createTimeStruct(time);
    Date *d = createDateStruct(date);

    if (!isValidTime(t)) {
        printf("invalid date.\n");
        return 0;
    }

    if (!isValidDate(d)) {
        printf("invalid date.\n");
        return 0;
    }

    if (!isValidLog(system, t, d)) {
        printf("invalid date.\n");
        return 0;
    }

    free(t);
    free(d);

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
    if (v == NULL) {
        return 0;
    }
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
    LogNode *cur = system->lHead;
    
    // Iterate through the linked list until the last log entry is found
    while (cur != NULL && cur->next != NULL) {
        cur = cur->next;
    }

    if (cur == NULL) {
        // No logs in the system
        return 1;
    }

    // Check if the last log entry's date and time are valid
    if (cur->log->type == 0) { // Last log is an entry
        return isValidLogAux(cur->log->entryDate, date, cur->log->entryTime, time);
    } else if (cur->log->type == 1) { // Last log is an exit
        return isValidLogAux(cur->log->exitDate, date, cur->log->exitTime, time);
    }

    return 1; // Default to valid if something unexpected happens
}

Vehicle *getVehicle(ParkingSystem *system, char *reg) {
    VehicleNode *current = system->vHead;

    if (current == NULL) {
        return NULL;
    }

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

double getBillingValue(Park *park, TimeDiff *diff) {
    double value = 0;
    if (diff->date->day == 0) {
        if (diff->time->hour < 1) {
            value = park->billingValue15;
        } else {
            value = park->billingValue15 + (diff->time->hour - 1) * park->billingValueAfter1Hour;
        }
    } else {
        value = park->maxDailyValue;
    }
    return value;
}

TimeDiff *getTimeDiff(Time *t1, Date *d1, Time *t2, Date *d2) {
    TimeDiff *diff = (TimeDiff *)malloc(sizeof(TimeDiff));
    if (diff == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Allocate memory for diff->time and diff->date
    diff->time = (Time *)malloc(sizeof(Time));
    if (diff->time == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    diff->date = (Date *)malloc(sizeof(Date));
    if (diff->date == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Copy values of t1, d1, t2, d2
    Time t1_copy = *t1;
    Time t2_copy = *t2;
    Date d1_copy = *d1;
    Date d2_copy = *d2;

    // Diff in minutes
    if (t2_copy.minute < t1_copy.minute) {
        t2_copy.hour -= 1;
        t2_copy.minute += 60;
    }
    diff->time->minute = t2_copy.minute - t1_copy.minute;

    // Diff in hours
    if (t2_copy.hour < t1_copy.hour) {
        d2_copy.day -= 1;
        t2_copy.hour += 24;
    }
    diff->time->hour = t2_copy.hour - t1_copy.hour;

    // Diff in days
    if (d2_copy.day < d1_copy.day) {
        d2_copy.month -= 1;
        if (d2_copy.month == 0) {
            d2_copy.month = 12;
            d2_copy.year -= 1;
        }
        d2_copy.day += daysByMonth(d2_copy.month - 1);
    }
    diff->date->day = d2_copy.day - d1_copy.day;

    // Diff in months
    if (d2_copy.month < d1_copy.month) {
        d2_copy.year -= 1;
        d2_copy.month += 12;
    }
    diff->date->month = d2_copy.month - d1_copy.month;

    // Diff in years
    diff->date->year = d2_copy.year - d1_copy.year;

    // Calculate total days based on years and months
    diff->date->day += diff->date->year * 365 + diff->date->month * daysByMonth(d2_copy.month - 1);

    // Reset year and months
    diff->date->year = 0;
    diff->date->month = 0;

    return diff;
}


int daysByMonth(int month) {
    switch (month) {
        case 1: return 31;
        case 2: return 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return 0;
    }

}

double calculateValue(Log *log, ParkingSystem *system) {
    Park *park = parkExists(system, log->parkName);
    TimeDiff *timeDiff = getTimeDiff(log->entryTime, log->entryDate, log->exitTime, log->exitDate);
    if (timeDiff == NULL) {
        return -1;  
    }
    double value = getBillingValue(park, timeDiff);
    return value;
}

Date *createDateStruct(char *date) {
    Date *d = (Date *)malloc(sizeof(Date));
    if (d == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Make a copy of the date string
    char *date_copy = strdup(date);
    if (date_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(d);
        exit(1);
    }

    char *day = strtok(date_copy, "-");
    char *month = strtok(NULL, "-");
    char *year = strtok(NULL, "-");

    if (day == NULL || month == NULL || year == NULL) {
        fprintf(stderr, "Invalid date format\n");
        free(date_copy); // Free the copy of the date string
        free(d);        // Free the allocated memory for the Date struct
        exit(1);
    }

    d->day = atoi(day);
    d->month = atoi(month);
    d->year = atoi(year);

    free(date_copy); // Free the copy of the date string after tokenization

    return d;
}

Time *createTimeStruct(char *time) {
    Time *t = (Time *)malloc(sizeof(Time));
    if (t == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Make a copy of the time string
    char *time_copy = strdup(time);
    if (time_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(t);
        exit(1);
    }

    char *hour = strtok(time_copy, ":");
    char *minute = strtok(NULL, ":");

    if (hour == NULL || minute == NULL) {
        fprintf(stderr, "Invalid time format\n");
        free(time_copy); // Free the copy of the time string
        free(t);        // Free the allocated memory for the Time struct
        exit(1);
    }

    t->hour = atoi(hour);
    t->minute = atoi(minute);

    free(time_copy); // Free the copy of the time string after tokenization

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