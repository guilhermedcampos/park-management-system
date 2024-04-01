#include "project.h"
#include "constants.h"
#include "projectaux.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidParkRequest(ParkingSystem *system, char* name, int cap, double x, double y, double z) {
    // Checks if the parking lot already exists
    if (getPark(system, name) != NULL) {
        printf("%s: parking already exists.\n", name);
        return 0;
    }

    if (cap == 0) {
        printf("%d: invalid capacity.\n", cap);
        return 0;
    }

    // Check for invalid costs
    if (x <= 0 || y <= 0 || z <= 0 || x >= y|| y>= z) {
        printf("invalid cost.\n");
        return 0;
    }

    if (system->numParks == MAX_PARKING_LOTS ) {
        printf("too many parks.\n");
        return 0;
    }

    return 1;
}

int isValidTime(Time *time) {
    if (time == NULL) {
        return 0;
    }
    if (time->hour < 0 || time->hour > 23) {
        return 0;
    }
    if (time->minute < 0 || time->minute > 59) {
        return 0;
    }
    return 1;
}

int isValidDate(Date *date) {

    if (date == NULL) {
        return 0;
    }   

    if (date->day < 1 || date->day > daysByMonth(date->month)) {
        return 0;
    }
    if (date->month < 1 || date->month > 12) {
        return 0;
    }
    if (date->year < 0) {
        return 0;
    }

    return 1;
}

char *dateToString(Date *date) {
    char *dateString = (char *)malloc(11 * sizeof(char));
    if (dateString == NULL) {
        return NULL;
    }

    sprintf(dateString, "%02d-%02d-%04d", date->day, date->month, date->year); 
    return dateString;
}

char *timeToString(Time *time){
    char *timeString = (char *)malloc(6 * sizeof(char));
    if (timeString == NULL) {
        return NULL;
    }

    sprintf(timeString, "%02d:%02d", time->hour, time->minute);
    return timeString;
}

int isValidRequest(ParkingSystem *system, char *name, char *reg, char *date, char *time, int type) {
    Park *park = getPark(system, name);

    if (park == NULL) {
        printf("%s: no such parking.\n", name);
        return 0;
    }

    if (type == 0) {
        if (park->currentLots == park->maxCapacity) {
            printf("%s: parking is full.\n", park->name);
            return 0;
        }
    }

    if (!isValidRegistration(reg) || (!isValidRegistration(reg) && type == 1)) {
        printf("%s: invalid licence plate.\n", reg);
        return 0;
    }

    if (type == 0) {
        if (isVehicleParked(system, reg)) {
            printf("%s: invalid vehicle entry.\n", reg);
            return 0;
        }
    }

    if (type == 1) {
        if (!isVehicleInParkExit(system, reg, name)) {
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

int isValidPair(char cur, char next) {
    if ((cur >= 'A' && cur <= 'Z') && (next >= 'A' && next <= 'Z')) {
        return 0;
    } else if ((cur >= '0' && cur <= '9') && (next >= '0' && next <= '9')) {
        return 1;
    } else {
        return 2;
    }
}

int isValidRegistration(char *reg) {
    // Check if the length of the registration is valid
    if (strlen(reg) != REGISTRATION_LENGTH) {
        return 0;
    }

    // Flags to track groups of letters and digits
    int letterGroup = 0;
    int digitGroup = 0;

    if (reg[2] != '-' || reg[5] != '-') {
        return 0;
    }

    // Iterate through each character in the registration
    for (int i = 0; i < REGISTRATION_LENGTH; i++) {
        char cur = reg[i];
        char next = reg[i + 1];

        int res;
        if ((res = isValidPair(cur, next)) == 2) { 
            return 0;
        } else if (res == 0) {
            letterGroup++;
            i +=2;
        } else {
            digitGroup++;
            i += 2;
        }
    }

    // Check if the registration contains at least one pair of letters and one pair of digits
    if ((letterGroup == 2 && digitGroup == 1) || (letterGroup == 1 && digitGroup == 2) ) {
        return 1;
    }

    // If all checks pass, the registration is valid
    return 0;
}

/* Returns 1 if full, 0 if not full */
int isParkFull(ParkingSystem* sys, char* name) {
    ParkNode *cur = sys->pHead;
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


int isVehicleParked(ParkingSystem *system, char *reg) {
    Vehicle *v = getVehicle(system, reg);
    // First entry, vehicle not created yet
    if (v == NULL) {
        return 0;
    } 
    
    if (v->parkName == NULL && v->isParked == 0) {
        return 0;
    } 

    return 1;
}

int isVehicleInParkExit(ParkingSystem *system, char *reg, char *name) {
    Vehicle *v = getVehicle(system, reg);
    if (v == NULL) {
        return 0;
    } if (v->parkName != NULL) {
        if ((strcmp(v->parkName, name) == 0) && v->isParked == 1) {
            return 1;
        }
        return 0;
    }

    return 1;
}
// If d1 is sooner than d2, it's valid (1)
int isLogDateValid(Date *d1, Date *d2) {

    if (d1->year < d2->year) {
        return 1; 
    } else if (d1->year == d2->year && d1->month < d2->month) {
        return 1; 
    } else if (d1->year == d2->year && d1->month == d2->month && d1->day < d2->day) {
        return 1; 
    } else if (d1->year == d2->year && d1->month == d2->month && d1->day == d2->day) {
        return 2;
    }

    return 0;
}


// If t1 is sooner than t2, it's valid (1)
int isLogTimeValid(Time *t1, Time *t2) {

    if (t1->hour < t2->hour) {
        return 1;
    }   else if (t1->hour == t2->hour) {
            if (t1->minute <= t2->minute) {
                return 1;
            }
        return 0;
    }
    return 0;
}
// If d1 and t1 are sooner than d2 and t2, it's valid (1)
int isDateBefore(Date *d1, Date *d2, Time *t1, Time *t2) {
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
    if (system->lastDate == NULL || system->lastTime == NULL) {
        return 1;
    }
    return isDateBefore(system->lastDate, date, system->lastTime, time);
}

void freeLogNode(LogNode *node) {
    if (node == NULL) {
        return;
    }
    free(node->log->exitDate);
    node->log->exitDate = NULL;
    free(node->log->exitTime);
    node->log->exitTime = NULL;
    free(node->log->entryDate);
    node->log->entryDate = NULL;
    free(node->log->entryTime);
    node->log->entryTime = NULL;
    free(node->log);
    node->log = NULL;
}


void removeVehicleLog(ParkingSystem *sys, Park *p, char *reg) {
    Vehicle *v = getVehicle(sys, reg);
    if (v == NULL) {
        return;
    }

    LogNode *current = v->lHead;
    while (current != NULL) {
        // Check if the log's park name matches the provided park name
        if (strcmp(current->log->parkName, p->name) == 0) {
            // If matched, remove this node from the linked list
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                // If current node is the head node
                v->lHead = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            // Free the memory for the removed node
            LogNode *temp = current;
            current = current->next;
            freeLogNode(temp);
            free(temp);
            temp = NULL;     
        } else {
            // Move to the next node
            current = current->next;
        }
    }
}

void freeParkLogs(ParkingSystem *sys, Park *p) {
    LogNode *cur = p->lHead;
    p->isSorted = 0;

    while (cur != NULL) {
        LogNode *temp = cur;
        cur = cur->next;
        if (temp->log->type == 0) {
            Vehicle *v = getVehicle(sys, temp->log->reg);
            if (v != NULL) {
                v->isParked = 0;
                v->parkName = NULL;
            }
        }
        removeVehicleLog(sys, p, temp->log->reg);
    }
}

// Function to get a vehicle by registration using the hash table
Vehicle *getVehicle(ParkingSystem *system, char *reg) {
    unsigned int index = hash(reg);
    VehicleHashNode *current = system->hashTable[index];

    while (current != NULL) {
        if (strcmp(current->vehicle->registration, reg) == 0) {
            return current->vehicle;
        }
        current = current->next;
    }
    return NULL; // Vehicle not found
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

// Merge function for mergesort
void merge(LogNode **arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    LogNode *L[n1];
    LogNode *R[n2];

    // Copy data to temporary arrays L[] and R[] using memcpy
    memcpy(L, &arr[l], n1 * sizeof(LogNode *));
    memcpy(R, &arr[m + 1], n2 * sizeof(LogNode *));

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (strcmp(L[i]->log->parkName, R[j]->log->parkName) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[] and R[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Mergesort function
void mergeSort(LogNode **arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Sort by name
LogNode *sortLogListName(Vehicle *v) {
    if (v == NULL || v->lHead == NULL) {
        return NULL;
    }

    // Count the number of nodes
    int count = 0;
    LogNode *temp = v->lHead;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    // Convert linked list to array
    LogNode **arr = (LogNode **)malloc(count * sizeof(LogNode *));
    temp = v->lHead;
    int i = 0;
    while (temp != NULL) {
        arr[i] = temp;
        temp = temp->next;
        i++;
    }

    // Sort the array using mergesort
    mergeSort(arr, 0, count - 1);

    // Update the linked list with sorted array
    for (i = 0; i < count - 1; i++) {
        arr[i]->next = arr[i + 1];
        arr[i + 1]->prev = arr[i];
    }
    arr[0]->prev = NULL;
    arr[count - 1]->next = NULL;

    // Update the head of the linked list
    v->lHead = arr[0];

    free(arr);
    arr = NULL;

    return v->lHead;
}

ParkNode *sortListName(ParkingSystem *sys) {
    Park *temp;
    ParkNode *current = sys->pHead;
    ParkNode *index = NULL;

    if (current == NULL) {
        return NULL; 
    }

    // ordenar por outras coisas
    while (current != NULL) {
        index = current->next;

        while (index != NULL) {
            if (strcmp(current->parking->name, index->parking->name) > 0) {
                temp = current->parking;
                current->parking = index->parking;
                index->parking = temp;
            }
            index = index->next;
        }
        current = current->next;
    }

    // find head of the new list
    while (sys->pHead->prev != NULL) {
        sys->pHead = sys->pHead->prev;
    }

    return sys->pHead; 
}

 // Partition function for quicksort
int partition(LogNode **arr, int low, int high) {
    LogNode *pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j]->log->type == 1 && pivot->log->type == 1) {
            if (isDateBefore(arr[j]->log->exitDate, pivot->log->exitDate, arr[j]->log->exitTime, pivot->log->exitTime)) {
                i++;
                LogNode *temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    LogNode *temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return i + 1;
}

// Quicksort function
void quicksort(LogNode **arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}


void sortListExitDate(Park *p) {
    // Find number of elements in linked list
    int n = 0;
    LogNode *current = p->lHead;
    while (current != NULL) {
        n++;
        current = current->next;
    }

    // Transform linked list into array
    LogNode **arr = (LogNode **)malloc(n * sizeof(LogNode *));
    if (arr == NULL) {
        return; // Allocation failed
    }
    current = p->lHead;
    for (int i = 0; i < n; i++) {
        arr[i] = current;
        current = current->next;
    }

    // Quick sort the array 
    quicksort(arr, 0, n - 1);

    // return the new head
    for (int i = 0; i < n - 1; i++) {
        arr[i]->next = arr[i + 1];
        arr[i + 1]->prev = arr[i];
    }
    arr[0]->prev = NULL;
    arr[n - 1]->next = NULL;

    p->lHead = arr[0];
    free(arr);

    return;
}

size_t dateInMinutes(Date *d, Time *t) {
    size_t totalMins = 0;
    totalMins += d->year * 365 * 24 * 60;
    for (int i = 1; i< d->month; i++) {
        totalMins += daysByMonth(i) * 24 * 60;
    }
    totalMins += d->day * 24 * 60;
    totalMins += t->hour * 60;
    totalMins += t->minute;
    return totalMins;
}

int isSameDate(Date *d1, Date *d2) {
    if (d1->day == d2->day && d1->month == d2->month && d1->year == d2->year) { return 1; }
    return 0;
}


size_t getTimeDiff(Time *t1, Date *d1, Time *t2, Date *d2) {
    size_t firstDateMins = dateInMinutes(d1, t1);
    size_t secondDateMins = dateInMinutes(d2, t2);
    return secondDateMins - firstDateMins;
}

int getTotalMonthDays(int month) {
    int days = 0;
    for( int i = 1; i <= month; i++) {
        days += daysByMonth(i);
    }
    return days;
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

double min(double a, double b) {
    return a < b ? a : b;
}

double calculateValue(Log *log, Park *park) {
    size_t minsInDay = 24 * 60;
    size_t diff = getTimeDiff(log->entryTime, log->entryDate, log->exitTime, log->exitDate);
    size_t days = diff / minsInDay;
    double completeDayRev = park->dailyCost * days;
    size_t remainingQuarters = ((diff % minsInDay) + 14) / 15;
    double remainingRev = 0;
    if (remainingQuarters <= 4) {
        remainingRev = park->quarterCost * remainingQuarters;
    } else {
        remainingRev = 4 * park->quarterCost + (remainingQuarters - 4) * park->afterHourCost;
    }
    completeDayRev += min(remainingRev, park->dailyCost);
    return completeDayRev;
}

Date *createDateStruct(char *date) {
    if (strlen(date) != 10) {
        return NULL;
    }

    Date *d = (Date *)malloc(sizeof(Date));
    if (d == NULL) {
        return NULL;
    }

    // Make a copy of the date string
    char *date_copy = strdup(date);
    if (date_copy == NULL) {
        free(d);
        d = NULL;
        return NULL;
    }

    char *day = strtok(date_copy, "-");
    char *month = strtok(NULL, "-");
    char *year = strtok(NULL, "-");

    if (day == NULL || month == NULL || year == NULL) {
        free(date_copy); // Free the copy of the date string
        date_copy = NULL;
        free(d);        // Free the allocated memory for the Date struct
        d = NULL;
        return NULL;
    }

    d->day = atoi(day);
    d->month = atoi(month);
    d->year = atoi(year);

    free(date_copy); // Free the copy of the date string after tokenization
    date_copy = NULL;
    return d;
}

Time *createTimeStruct(char *time) {
    Time *t = (Time *)malloc(sizeof(Time));
    if (t == NULL) {
        return NULL;
    }

    // Make a copy of the time string
    char *time_copy = strdup(time);
    if (time_copy == NULL) {
        free(t);
        t = NULL;
        return NULL;
    }

    char *hour = strtok(time_copy, ":");
    char *minute = strtok(NULL, ":");

    if (hour == NULL || minute == NULL) {
        free(time_copy); // Free the copy of the time string
        time_copy = NULL;
        free(t);        // Free the allocated memory for the Time struct
        t = NULL;
        return NULL;    
    }

    t->hour = atoi(hour);
    t->minute = atoi(minute);

    free(time_copy); // Free the copy of the time string after tokenization
    time_copy = NULL;
    return t;
}

char *nextWord(Buffer *buffer) {
    int i = 0;
    char *next_word = (char *)malloc(strlen(buffer->buffer) * sizeof(char));

    if (next_word == NULL) {
        return NULL;
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