/*
 * iaed-24 - ist1106909 - project
 * Auxiliar source file with the auxiliar functions.
 */
#include "project.h"
#include "constants.h"
#include "projectaux.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Converts a Date structure to a string representation.
 *
 * @param date Pointer to the Date structure to be converted.
 * @return String containing the date in the format "DD-MM-YYYY".
 */
char *dateToString(Date *date) {
    char *dateString = (char *)malloc(11 * sizeof(char));
    if (dateString == NULL) {
        return NULL;
    }

    sprintf(dateString, "%02d-%02d-%04d", date->day, date->month, date->year); 
    return dateString;
}

/**
 * @brief Converts a Time structure to a string representation.
 *
 * @param time Pointer to the Time structure to be converted.
 * @return String containing the time in the format "HH:MM".
 */
char *timeToString(Time *time){
    char *timeString = (char *)malloc(6 * sizeof(char));
    if (timeString == NULL) {
        return NULL;
    }

    sprintf(timeString, "%02d:%02d", time->hour, time->minute);
    return timeString;
}

/**
 * @brief Checks if the given Time structure is valid.
 *
 * @param time Pointer to the Time structure to be validated.
 * @return Returns 1 if the Time structure is valid, 0 otherwise.
 */
int isValidTime(Time *time) {
    if (time == NULL) {
        return FALSE;
    }
    if (time->hour < 0 || time->hour > 23) {
        return FALSE;
    }
    if (time->minute < 0 || time->minute > 59) {
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Checks if the given Date structure is valid.
 *
 * @param date Pointer to the Date structure to be validated.
 * @return Returns 1 if the Date structure is valid, 0 otherwise.
 */
int isValidDate(Date *date) {
    if (date == NULL) {
        return FALSE;
    }   

    if (date->day < 1 || date->day > daysByMonth(date->month)) {
        return FALSE;
    }
    if (date->month < 1 || date->month > 12) {
        return FALSE;
    }
    if (date->year < 0) {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief Checks if two Date structures represent the same date.
 *
 * @param d1 Pointer to the first Date structure.
 * @param d2 Pointer to the second Date structure.
 * @return Returns 1 if both Date structures represent the same date, 0 otherwise.
 */
int isSameDate(Date *d1, Date *d2) {
    if (d1->day == d2->day && d1->month == d2->month && d1->year == d2->year) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @brief Checks if the first date is before the second date.
 *
 * @param d1 Pointer to the first Date structure.
 * @param d2 Pointer to the second Date structure.
 * @return Returns 1 if the first date is before the second date, 0 otherwise.
 */
int isLogDateBefore(Date *d1, Date *d2) {

    if (d1->year < d2->year) {
        return TRUE; 
    } else if (d1->year == d2->year && d1->month < d2->month) {
        return TRUE; 
    } else if (d1->year == d2->year && d1->month == d2->month && d1->day < d2->day) {
        return TRUE; 
    } else if (d1->year == d2->year && d1->month == d2->month && d1->day == d2->day) {
        return 2;
    }

    return FALSE;
}

/**
 * @brief Checks if the first time is before the second time.
 *
 * @param t1 Pointer to the first Time structure.
 * @param t2 Pointer to the second Time structure.
 * @return Returns 1 if the first time is before the second time, 0 otherwise.
 */
int isLogTimeBefore(Time *t1, Time *t2) {

    if (t1->hour < t2->hour) {
        return TRUE;
    }   else if (t1->hour == t2->hour) {
            if (t1->minute <= t2->minute) {
                return TRUE;
            }
        return FALSE;
    }
    return FALSE;
}

/**
 * @brief Checks if the first date is before the second date.
 *
 * @param d1 Pointer to the first Date structure.
 * @param d2 Pointer to the second Date structure.
 * @param t1 Pointer to the first Time structure.
 * @param t2 Pointer to the second Time structure.
 * @return Returns 1 if the first date is before the second date, 0 otherwise.
 */
int isDateBefore(Date *d1, Date *d2, Time *t1, Time *t2) {
    int val = isLogDateBefore(d1, d2);
    if (val == TRUE) {
        return TRUE;
    } else if (val == 2) {
        if (isLogTimeBefore(t1, t2) == TRUE) {
            return TRUE;
        }
        return TRUE;
    }
    return TRUE;
}

/**
 * @brief Calculates the total number of minutes represented by a date and time.
 *
 * @param d Pointer to the Date struct representing the date.
 * @param t Pointer to the Time struct representing the time.
 * @return Returns the total number of minutes.
 */
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

/**
 * @brief Calculates the time difference in minutes between two date-time pairs.
 *
 * @param t1 Pointer to the Time struct representing the first time.
 * @param d1 Pointer to the Date struct representing the first date.
 * @param t2 Pointer to the Time struct representing the second time.
 * @param d2 Pointer to the Date struct representing the second date.
 * @return Returns the time difference in minutes.
 */
size_t getTimeDiff(Time *t1, Date *d1, Time *t2, Date *d2) {
    size_t firstDateMins = dateInMinutes(d1, t1);
    size_t secondDateMins = dateInMinutes(d2, t2);
    return secondDateMins - firstDateMins;
}

/**
 * @brief Returns total number of days in the months up to the given month.
 *
 * @param month The month for which to calculate the total number of days.
 * @return The total number of days in the months up to the given month.
 */
int getTotalMonthDays(int month) {
    int days = 0;
    for( int i = 1; i <= month; i++) {
        days += daysByMonth(i);
    }
    return days;
}

/**
 * @brief Returns the number of days in the specified month.
 *
 * @param month The month for which to determine the number of days.
 * @return The number of days in the specified month.
 */
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

/**
 * @brief Returns the minimum of two double values.
 *
 * @param a The first double value.
 * @param b The second double value.
 * @return The minimum of the two double values.
 */
double min(double a, double b) {
    return a < b ? a : b;
}

/**
 * @brief Creates a Date structure from a string representation of date.
 *
 * @param date A string representing the date in the format "DD-MM-YYYY".
 * @return A pointer to the created Date structure.
 */
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


/**
 * @brief Creates a Time structure from a string representation of time.
 *
 * @param time A string representing the time in the format "HH:MM".
 * @return A pointer to the created Time structure.
 *         The caller is responsible for freeing the allocated memory.
 *         Returns NULL if the input time string is invalid or memory allocation fails.
 */
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

/**
 * @brief Checks if the date of last log in system is sooner.
 *
 * @param sys Pointer to the ParkingSystem structure.
 * @param time Pointer to the Time structure representing the time to compare.
 * @param date Pointer to the Date structure representing the date to compare.
 * @return Returns 1 if the date of the last log in the system is sooner, 0 otherwise.
 */
int isValidLog(System *sys, Time *time, Date *date) {
    if (sys->lastDate == NULL || sys->lastTime == NULL) {
        return TRUE;
    }
    return isDateBefore(sys->lastDate, date, sys->lastTime, time);
}


/**
 * @brief Checks if a park request is valid. Used for command "p".
 *
 * @param sys Pointer to the ParkingSystem structure.
 * @param name Pointer to a character array representing the name of the park.
 * @param cap Integer representing the capacity of the parking lot.
 * @param x Double representing the cost per quarter-hour.
 * @param y Double representing the cost per hour after the first hour.
 * @param z Double representing the cost per 24 hours.
 * @return Returns 1 if the park request is valid, 0 otherwise.
 */
int isValidParkRequest(System *sys, char* name, int cap, double x, double y, double z) {
    if (getPark(sys, name) != NULL) { // Checks if the parking lot already exists
        printf(ERR_PARK_EXISTS, name);
        return FALSE;
    }
    if (cap == 0) { // Check for invalid capacity
        printf(ERR_INVALID_CAP, cap);
        return FALSE;
    }
    if (x <= 0 || y <= 0 || z <= 0 || x >= y|| y>= z) { // Check for invalid costs
        printf(ERR_INVALID_COST);
        return FALSE;
    }
    if (sys->numParks == MAX_PARKING_LOTS ) { // Check if reached the maximum number
        printf(ERR_TOO_MANY_PARKS);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Validates a date request.
 *
 * This function validates a date request by checking if the provided date and time are valid,
 * and if the date and time are log-valid within the system.
 *
 * @param sys Pointer to the parking system.
 * @param date The date string to validate.
 * @param time The time string to validate.
 * @return 1 if the date request is valid, 0 otherwise.
 */
int isValidDateRequest(System *sys, char *date, char *time) {
    // Create time and date structures
    Time *t = createTimeStruct(time);
    Date *d = createDateStruct(date);

    // Check if time, date, and log are valid
    if (!isValidTime(t) || !isValidDate(d) || !isValidLog(sys, t, d)) {
        printf(ERR_INVALID_DATE);
        return FALSE;
    }

    // Free allocated memory and return result
    free(t);
    free(d);
    return TRUE;
}

/**
 * @brief Checks if a request (entry or exit) is valid.
 *
 * @param sys Pointer to the ParkingSystem structure.
 * @param name Pointer to a character array representing the name of the parking lot.
 * @param reg Pointer to a character array representing the vehicle registration.
 * @param date Pointer to a character array representing the date of the request.
 * @param time Pointer to a character array representing the time of the request.
 * @param type Integer representing the type of request (0 for entry, 1 for exit).
 * @return Returns 1 if the request is valid, 0 otherwise.
 */
int isValidRequest(System *sys, char *name, char *reg, char *date, char *time, int type) {
    Park *park = getPark(sys, name);
    if (park == NULL) {
        printf(ERR_NO_PARK, name);
        return FALSE;
    }
    if (type == 0) {
        if (park->currentLots == park->maxCapacity) {
            printf(ERR_PARK_FULL, park->name);
            return FALSE;
        }
    }
    if (!isValidRegistration(reg) || (!isValidRegistration(reg) && type == 1)) {
        printf(ERR_INVALID_REG, reg);
        return FALSE;
    }
    if (type == 0) {
        if (isVehicleParked(sys, reg)) {
            printf(ERR_INVALID_ENTRY, reg);
            return FALSE;
        }
    }
    if (type == 1) {
        if (!isVehicleInParkExit(sys, reg, name)) {
            printf(ERR_INVALID_EXIT, reg);
            return FALSE;
        }
    }
    return isValidDateRequest(sys, date, time);
}

/**
 * @brief Checks if a vehicle has valid logs for printing.
 *
 * @param v Pointer to the Vehicle structure.
 * @param reg Pointer to a character array representing the vehicle registration.
 * @return Returns 1 if the vehicle has valid logs for printing, 0 otherwise.
 */
int isValidPrintLogsRequest(Vehicle *v, char *reg) {
    if (v == NULL) {
        printf("%s: no entries found in any parking.\n", reg);
        return FALSE;
    }

    if (v->lHead == NULL) {
        printf("%s: no entries found in any parking.\n", v->registration);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Checks if a revenue information request is valid.
 *
 * @param park Pointer to the Park structure.
 * @param name Pointer to a character array representing the park name.
 * @return Returns 1 if the revenue check is valid, 0 otherwise.
 */
int isValidRevenueCheck(Park *park, char *name) {
    if (park == NULL) { // Check if park exists
        printf("%s: no such parking.\n", name);
        return FALSE;
    }

    if (park->lHead == NULL) { // If park log list is empty, return
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief Checks if a pair of characters is valid.
 *
 * @param cur The current character.
 * @param next The next character.
 * @return Returns 0 if both characters are uppercase letters, 1 if both are digits, and 2 otherwise.
 */
int isValidPair(char cur, char next) {
    if ((cur >= 'A' && cur <= 'Z') && (next >= 'A' && next <= 'Z')) {
        return 0;
    } else if ((cur >= '0' && cur <= '9') && (next >= '0' && next <= '9')) {
        return 1;
    } else {
        return 2;
    }
}

/**
 * @brief Checks if a vehicle registration string is valid.
 *
 * @param reg The vehicle registration string to be validated.
 * @return Returns 1 if the registration string is valid, and 0 otherwise.
 */
int isValidRegistration(char *reg) {
    // Check if the length of the registration is valid
    if (strlen(reg) != REGISTRATION_LENGTH) {
        return FALSE;
    }

    // Flags to track groups of letters and digits
    int letterGroup = 0;
    int digitGroup = 0;

    if (reg[2] != '-' || reg[5] != '-') {
        return FALSE;
    }

    // Iterate through each character in the registration
    for (int i = 0; i < REGISTRATION_LENGTH; i++) {
        char cur = reg[i];
        char next = reg[i + 1];

        int res;
        if ((res = isValidPair(cur, next)) == 2) { 
            return FALSE;
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
        return FALSE;
    }

    // If all checks pass, the registration is valid
    return TRUE;
}

/**
 * @brief Checks if a parking lot is full.
 *
 * @param sys The parking system containing the parking lot.
 * @param name The name of the parking lot to check.
 * @return Returns 1 if the parking lot is full, and 0 if it is not full.
 */
int isParkFull(System* sys, char* name) {
    ParkNode *cur = sys->pHead;
    while (cur != NULL) {
        if (strcmp(cur->parking->name, name) == 0) {
            if (cur->parking->currentLots == cur->parking->maxCapacity) {
                return TRUE;
            }
        }
        cur = cur->next;
    }
    return FALSE;
}

/**
 * @brief Checks if a vehicle is currently parked in any parking lot.
 *
 * @param sys The parking system containing the parking lots.
 * @param reg The registration number of the vehicle to check.
 * @return Returns 1 if the vehicle is parked, and 0 if it is not parked.
 */
int isVehicleParked(System *sys, char *reg) {
    Vehicle *v = getVehicle(sys, reg);
    // First entry, vehicle not created yet
    if (v == NULL) {
        return FALSE;
    } 
    
    if (v->parkName == NULL && v->isParked == 0) {
        return FALSE;
    } 

    return TRUE;
}

/**
 * @brief Checks if a vehicle is eligible for exit from a specific parking lot.
 *
 * @param sys The parking system containing the parking lots.
 * @param reg The registration number of the vehicle to check.
 * @param name The name of the park from which the vehicle is expected to exit.
 * @return Returns 1 if the vehicle is eligible for exit, and 0 otherwise.
 */
int isVehicleInParkExit(System *sys, char *reg, char *name) {
    Vehicle *v = getVehicle(sys, reg);
    if (v == NULL) {
        return FALSE;
    } if (v->parkName != NULL) {
        if ((strcmp(v->parkName, name) == 0) && v->isParked == TRUE) {
            return TRUE;
        }
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief Removes vehicle logs associated with a specific park from the system.
 *
 * @param sys Pointer to the ParkingSystem structure.
 * @param p Pointer to the Park structure representing the park.
 * @param reg Pointer to the character array representing the registration of the vehicle.
 */
void removeVehicleLog(System *sys, Park *p, char *reg) {
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

/**
 * @brief Frees the logs associated with a specific park.
 *
 * @param sys Pointer to the ParkingSystem structure.
 * @param p Pointer to the Park structure representing the park.
 */
void freeParkLogs(System *sys, Park *p) {
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

/**
 * @brief Merge function for mergesort.
 *
 * @param arr The array of LogNode pointers to be merged.
 * @param l The left index of the first subarray.
 * @param m The middle index.
 * @param r The right index of the second subarray.
 */
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

/**
 * @brief Mergesort function.
 *
 * @param arr The array of LogNode pointers to be sorted.
 * @param l The left index of the array to be sorted.
 * @param r The right index of the array to be sorted.
 */
void mergeSort(LogNode **arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

/**
 * @brief Sorts a linked list of log nodes by park name.
 *
 * @param v Pointer to the vehicle whose log list is to be sorted.
 * @return Pointer to the head of the sorted log list.
 */
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

/**
 * @brief Sorts the linked list of parks by name.
 *
 * @param sys Pointer to the parking system.
 * @return Pointer to the head of the sorted park list.
 */
ParkNode *sortListName(System *sys) {
    Park *temp;
    ParkNode *current = sys->pHead;
    ParkNode *index = NULL;

    if (current == NULL) {
        return NULL; 
    }

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

/**
 * @brief Partitions the array for the quicksort algorithm.
 *
 * @param arr Array of log nodes to be partitioned.
 * @param low The starting index of the partition.
 * @param high The ending index of the partition.
 * @return The partition index.
 */
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

/**
 * @brief Sorts an array of log nodes using the quicksort algorithm.
 *
 * @param arr Array of log nodes to be sorted.
 * @param low The starting index of the array.
 * @param high The ending index of the array.
 */
void quicksort(LogNode **arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

/**
 * @brief Sorts the linked list of logs in a park by exit date.
 *
 * @param p Pointer to the park whose log list is to be sorted.
 */
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

    // Return the new head
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

/**
 * @brief Retrieves the next word from the buffer.
 *
 * @param buffer Pointer to the buffer.
 * @return Pointer to the next word, or NULL if there are no more words in the buffer.
 */
char *nextWord(Buffer *buffer) {
    int i = 0;
    char *next_word = (char *)malloc(strlen(buffer->buffer) * sizeof(char));
    if (next_word == NULL) {
        return NULL;
    }
    while (buffer->buffer[buffer->index] == ' ' 
        || buffer->buffer[buffer->index] == '\n') {
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
        while (buffer->buffer[buffer->index] != ' ' && 
               buffer->buffer[buffer->index] != '\n' &&
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