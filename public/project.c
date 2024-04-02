/*
 * iaed-24 - ist1106909 - project
 * Main source file with the main functions.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "project.h"
#include "constants.h"
#include "projectaux.h"

/**
 * @brief Reads the input from the user and inserts it into the buffer.
 *
 * @param buffer Pointer to the Buffer struct where the input will be stored.
 * @return Returns a pointer to the updated Buffer struct containing the user input.
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

/**
 * @brief Initializes the parking system.
 *
 * Initializes the parking system by allocating memory for the system structure,
 * initializing its fields, and setting up arrays for parks and hash tables.
 *
 * @return A pointer to the initialized parking system if successful, otherwise NULL.
 */
System* init() {
    // Allocate memory for the parking system
    System *sys = (System *)malloc(sizeof(System));
    if (sys == NULL) {
        return NULL; // Memory allocation failed
    }

    // Initialize fields of the parking system structure
    sys->pHead = NULL;
    sys->vHead = NULL;
    sys->vTail = NULL;
    sys->lastDate = NULL;
    sys->lastTime = NULL;
    sys->numParks = 0;

    // Initialize the parks array and the hash table
    initParksArray(sys);
    initHashTable(sys);

    return sys;
}

/**
 * @brief Initializes the parks array in the parking system.
 *
 * @param sys A pointer to the parking system.
 */
void initParksArray(System *sys) {
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        sys->parks[i] = NULL;
    }
}

/**
 * @brief Initializes the hash table in the parking system.
 *
 * @param sys A pointer to the parking system.
 */
void initHashTable(System *sys) {
    // Set all buckets of the hash table to NULL
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        sys->hashTable[i] = NULL;
    }
}

/**
 * @brief Adds a park to the array of parks in the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param park A pointer to the park to be added.
 */
void addParkToArray(System *sys, Park *park) {
    // Iterate through the array of parks to find the first available slot
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        if (sys->parks[i] == NULL) {
            // Insert the park into the first available slot and return
            sys->parks[i] = park;
            return;
        }
    }
}

/**
 * @brief Adds a parking node to the linked list of parks in the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param parking A pointer to the parking node to be added.
 */
void addParkToList(System *sys, ParkNode *parking) {
    // Iterate through the list to find an empty spot
    if (sys->pHead == NULL) {
        // If the list is empty, set the parking node as the head
        sys->pHead = parking;
        parking->next = NULL;
    } else {
        // Otherwise, iterate through the list to find the last node
        ParkNode *cur = sys->pHead;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        // Insert the parking node at the end of the list
        cur->next = parking;
        parking->prev = cur;
        parking->next = NULL;
    }
}

/**
 * @brief Adds a park to the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param parking A pointer to the parking node containing the park to be added.
 */
void addPark(System *sys, ParkNode *parking) {
    // Add the park to the array of parks
    addParkToArray(sys, parking->parking);

    // Add the park to the linked list of parks
    addParkToList(sys, parking);

    // Increment the count of parks in the system
    sys->numParks++;
}

/**
 * @brief Updates the array of parks in the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param index The index from which elements are to be shifted.
 */
void updateParksArray(System *sys, int index) {
    // Shift elements to the left starting from the index
    for (int i = index; i < MAX_PARKING_LOTS - 1; i++) {
        sys->parks[i] = sys->parks[i + 1];
    }

    // Set the last element to NULL (as it's shifted left)
    sys->parks[MAX_PARKING_LOTS - 1] = NULL;
}

/**
 * @brief Removes a park from the array of parks in the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param name The name of the park to be removed.
 */
void removeParkFromArray(System *sys, char *name) {
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        if (sys->parks[i] != NULL && strcmp(sys->parks[i]->name, name) == 0) {
            // First element of the list, shift left the array after removing first element
            updateParksArray(sys, i);
        }
    }
}

/**
 * @brief Removes a park node from the list in the parking system.
 *
 * This function removes the park node with the specified name from the list.
 *
 * @param sys A pointer to the parking system.
 * @param name The name of the park to be removed.
 */
void removeParkFromList(System *sys, char *name) {
    ParkNode *cur = sys->pHead; /**< Current node pointer */
    ParkNode *prev = NULL; /**< Previous node pointer */
    
    // Iterate through the list to find the node with the specified name
    while (cur != NULL) {
        if (strcmp(cur->parking->name, name) == 0) {
            // If the node to be removed is the head of the list
            if (prev == NULL) {
                sys->pHead = cur->next;
            } else {
                prev->next = cur->next;
            }

            // If the node to be removed is not the last node in the list
            if (cur->next != NULL) {
                cur->next->prev = prev;
            }
            
            // Free memory for the park node and its name
            free(cur->parking->name);
            free(cur->parking);
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

/**
 * @brief Removes a park from the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param name The name of the park to be removed.
 */
void removePark(System *sys, char *name) {
    // If the list of parks is empty, return
    if (sys->pHead == NULL) {
        return;
    }

    // Remove the park from the array and the linked list
    removeParkFromArray(sys, name);
    removeParkFromList(sys, name);
    
    // Decrement the number of parks in the system
    sys->numParks--;
}

/**
 * @brief Prints information about all parks in the parking system.
 *
 * @param sys A pointer to the parking system.
 */
void printParks(System* sys) {
    // Iterate through the parks array
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        // Check if the park at index i is not NULL
        if (sys->parks[i] != NULL) {
            // Print park information: name, max capacity, and available parking lots
            printf("%s %d %d\n", sys->parks[i]->name, sys->parks[i]->maxCapacity, sys->parks[i]->maxCapacity - sys->parks[i]->currentLots);
        }
    }
}

/**
 * @brief Sorts the linked list of parks by name in the parking system.
 *
 * @param sys A pointer to the parking system.
 * @return A pointer to the head of the sorted list.
 */
void printRemainingParks(System* sys) {
    // Sort the list by park names alphabetically
    ParkNode *cur = sortListName(sys);
    while (cur != NULL) {
        printf("%s\n", cur->parking->name);
        cur = cur->next;
    }
}

/**
 * @brief Retrieves a park by its name from the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param name The name of the park to retrieve.
 * @return A pointer to the park if found, otherwise NULL.
 */
Park* getPark(System* sys, char* name) {
    ParkNode *cur = sys->pHead;

    // Iterate through the list of parks
    while (cur != NULL) {
        // Check if the name matches
        if (strcmp(cur->parking->name, name) == 0) {
            return cur->parking; // Return the park if found
        }
        cur = cur->next;
    }
    return NULL; // Return NULL if the park is not found
}

/**
 * @brief Creates a new park node.
 *
 * @param name The name of the park.
 * @param maxCapacity The maximum capacity of the park.
 * @param quarterCost The billing value for the first 15 minutes.
 * @param afterHourCost The billing value after the first hour.
 * @param dailyCost The maximum daily billing value.
 * @return A pointer to the newly created park node if successful, otherwise NULL.
 */
ParkNode *createPark(char *name, char *maxCapacity, char *quarterCost, char *afterHourCost, char *dailyCost) {
    Park *newPark = createParkData(name, maxCapacity, quarterCost, afterHourCost, dailyCost);
    if (newPark == NULL) {
        return NULL;
    }

    ParkNode *newParking = (ParkNode *)malloc(sizeof(ParkNode));
    if (newParking == NULL) {
        free(newPark->name);
        free(newPark);
        return NULL;
    }

    newParking->parking = newPark;
    newParking->next = NULL;
    newParking->prev = NULL;

    return newParking;
}

/**
 * @brief Creates a new park structure.
 *
 * @param name The name of the park.
 * @param maxCapacity The maximum capacity of the park.
 * @param quarterCost The billing value for the first 15 minutes.
 * @param afterHourCost The billing value after the first hour.
 * @param dailyCost The maximum daily billing value.
 * @return A pointer to the newly created park structure if successful, otherwise NULL.
 */
Park *createParkData(char *name, char *maxCapacity, char *quarterCost, char *afterHourCost, char *dailyCost) {
    Park *newPark = (Park *)malloc(sizeof(Park));   // Allocate memory for the park structure
    if (newPark == NULL) {
        return NULL;
    }

    newPark->name = strdup(name); // Allocate memory for the name field and copy the string
    if (newPark->name == NULL) {
        free(newPark);
        return NULL;
    }

    newPark->lHead = NULL;
    newPark->maxCapacity = atoi(maxCapacity);
    newPark->currentLots = 0;
    newPark->quarterCost = atof(quarterCost);
    newPark->afterHourCost = atof(afterHourCost);
    newPark->dailyCost = atof(dailyCost);

    return newPark;
}

/**
 * @brief Adds a vehicle to the linked list of vehicles in the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param vehicle A pointer to the vehicle to be added.
 */
void addVehicleToList(System *sys, Vehicle *vehicle) {
    // Initialize the vehicle node pointers
    VehicleNode *vehicleNode = (VehicleNode *)malloc(sizeof(VehicleNode));
    if (vehicleNode == NULL) {
        // Handle memory allocation failure
        return;
    }
    vehicleNode->prev = NULL;
    vehicleNode->next = NULL;
    vehicleNode->vehicle = vehicle;

    if (sys->vHead == NULL) {
        // If the list is empty, set both head and tail to the new node
        sys->vHead = vehicleNode;
        sys->vTail = vehicleNode;
    } else {
        // Otherwise, append the new node to the tail
        sys->vTail->next = vehicleNode;
        vehicleNode->prev = sys->vTail;
        sys->vTail = vehicleNode; // Update the tail pointer
    }
}

/**
 * @brief Hash function to map registration strings to indices.
 *
 * @param reg The registration string to be hashed.
 * @return The hashed value representing the index in the hash table.
 */
unsigned int hash(const char *reg) {
    unsigned int hashValue = 0;
    while (*reg) {
        hashValue = (hashValue << 5) + *reg++;
    }
    return hashValue % HASH_TABLE_SIZE;
}

/**
 * @brief Adds a vehicle to the hash table in the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param vehicle A pointer to the vehicle to be added.
 */
void addToHashTable(System *sys, Vehicle *vehicle) {
    // Calculate the index for the vehicle based on its registration number
    unsigned int index = hash(vehicle->registration);
    
    // Allocate memory for a new node
    VehicleHashNode *newNode = (VehicleHashNode *)malloc(sizeof(VehicleHashNode));
    if (newNode == NULL) {
        // Handle memory allocation error
        return;
    }
    
    // Initialize the new node with the vehicle and insert it into the appropriate bucket
    newNode->vehicle = vehicle;
    newNode->next = sys->hashTable[index];
    sys->hashTable[index] = newNode;
}

/**
 * @brief Adds a vehicle to the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param vehicle A pointer to the vehicle to be added.
 */
void addVehicle(System *sys, Vehicle *vehicle) {
    // Add the vehicle to the linked list of vehicles
    addVehicleToList(sys, vehicle);

    // Add the vehicle to the hash table for efficient lookup
    addToHashTable(sys, vehicle);
}

/**
 * @brief Retrieves the vehicle associated with the given registration
 *
 * @param sys Pointer to the ParkingSystem struct representing the system.
 * @param reg Pointer to the character array representing the registration.
 * @return Returns a pointer to the Vehicle struct if found, or NULL if the vehicle is not found.
 */
Vehicle *getVehicle(System *sys, char *reg) {
    unsigned int index = hash(reg);
    VehicleHashNode *current = sys->hashTable[index];

    while (current != NULL) {
        if (strcmp(current->vehicle->registration, reg) == 0) {
            return current->vehicle;
        }
        current = current->next;
    }
    return NULL; // Vehicle not found
}

/**
 * @brief Creates vehicle data with the given registration number.
 *
 * @param reg The registration number of the vehicle.
 * @return A pointer to the newly created vehicle data if successful, or NULL on failure.
 */
Vehicle *createVehicleData(char *reg) {
    Vehicle *vehicle = (Vehicle *)malloc(sizeof(Vehicle));
    if (vehicle == NULL) {
        return NULL; // Memory allocation failed
    }

    // Allocate memory for the registration field and copy the string
    vehicle->registration = strdup(reg);
    if (vehicle->registration == NULL) {
        free(vehicle);
        return NULL; // Memory allocation failed
    }

    // Initialize other fields
    vehicle->parkName = NULL;
    vehicle->isParked = 0;
    vehicle->lHead = NULL;
    vehicle->lTail = NULL;
    vehicle->lastLog = NULL;

    return vehicle;
}

/**
 * @brief Creates a new vehicle and adds it to the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param reg The registration number of the vehicle.
 * @return A pointer to the newly created vehicle if successful, or NULL on failure.
 */
Vehicle *createVehicle(System *sys, char *reg) {
    Vehicle *vehicle = createVehicleData(reg);
    if (vehicle == NULL) {
        return NULL; // Creation of vehicle data failed
    }
    // Add the vehicle to the parking sys
    addVehicle(sys, vehicle);
    return vehicle;
}

/**
 * @brief Enters a vehicle into a park in the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param p A pointer to the park where the vehicle is entering.
 * @param v A pointer to the vehicle entering the park.
 * @param date The date of entry in "YYYY-MM-DD" format.
 * @param time The time of entry in "HH:MM" format.
 * @return SUCCESS if the operation is successful.
 */
int enterPark(System *sys, Park *p, Vehicle *v, char *date, char *time) {

    updateSystem(sys, date, time);
    v->parkName = p->name;
    v->isParked = 1;
    p->currentLots++;
    v->lastLog = createLog(v, p, date, time);
    return SUCCESS;
}

/**
 * @brief Prints the exit details of a vehicle.
 *
 * @param v A pointer to the vehicle whose exit details are to be printed.
 */
void printExit(Vehicle *v) {
    // Convert entry and exit date/time to string representations
    char *d1 = dateToString(v->lastLog->entryDate);
    char *t1 = timeToString(v->lastLog->entryTime);
    char *d2 = dateToString(v->lastLog->exitDate);
    char *t2 = timeToString(v->lastLog->exitTime);

    // Print vehicle exit details
    printf("%s %s %s %s %s %.2f\n", v->registration, d1, t1, d2, t2, v->lastLog->value);

    // Free dynamically allocated memory for date/time strings
    free(d1);
    free(t1);
    free(d2);
    free(t2);
}

/**
 * @brief Handles the exit of a vehicle from the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param p A pointer to the park from which the vehicle is exiting.
 * @param v A pointer to the vehicle exiting the park.
 * @param date A string representing the exit date of the vehicle.
 * @param time A string representing the exit time of the vehicle.
 * @return Returns 0 upon successful exit, or 1 if there is a failure in the process.
 */
int exitPark(System *sys, Park *p, Vehicle *v, char *date, char *time) {
    updateSystem(sys, date, time);  
    v->parkName = NULL;
    v->isParked = 0;    
    p->currentLots--;
    Log *l = updateEntryLog(v->lastLog, date, time, p);
    if (l == NULL) {
        return FAILURE;
    }
    printExit(v);
    return SUCCESS;
}

/**
 * @brief Updates the system's last date and time.
 *
 * This function updates the last date and time of the system with the provided date and time values.
 *
 * @param sys A pointer to the parking system to be updated.
 * @param date A string representing the new date.
 * @param time A string representing the new time.
 */
void updateSystem(System *sys, char *date, char *time) {
    Date *d = createDateStruct(date);
    Time *t = createTimeStruct(time);

    Date *newDate = (Date *)realloc(sys->lastDate, sizeof(Date));
    if (newDate == NULL) {
        free(d); 
        free(t);
        return; 
    }
    sys->lastDate = newDate;
    memcpy(sys->lastDate, d, sizeof(Date)); 

    Time *newTime = (Time *)realloc(sys->lastTime, sizeof(Time));
    if (newTime == NULL) {
        free(d); 
        free(t);
        return; 
    }
    sys->lastTime = newTime;
    memcpy(sys->lastTime, t, sizeof(Time));

    free(d); 
    free(t);
}


/**
 * @brief Adds a log to the log list of a vehicle.
 *
 * @param v A pointer to the vehicle to which the log is being added.
 * @param l A pointer to the log to be added.
 * @return Returns a pointer to the added log if successful, or NULL if memory allocation fails.
 */
Log *addLogToVehicle(Vehicle *v, Log *l) {
    LogNode *newLog = (LogNode *)malloc(sizeof(LogNode));
    if (newLog == NULL) {
        return NULL;
    }
    
    newLog->log = l;
    newLog->next = NULL;
    newLog->prev = NULL;

    if (v->lHead == NULL) {
        // If the list is empty, set both head and tail to the new node
        v->lHead = newLog;
        v->lTail = newLog;
    } else {
        // Otherwise, append the new node to the tail
        v->lTail->next = newLog;
        newLog->prev = v->lTail;
        v->lTail = newLog; // Update the tail pointer
    }
    return l;
}

/**
 * @brief Adds a log to the log list of a park.
 *
 * @param p A pointer to the park to which the log is being added.
 * @param l A pointer to the log to be added.
 */
void addLogToPark(Park *p, Log *l) {
    LogNode *newLog = (LogNode *)malloc(sizeof(LogNode));
    if (newLog == NULL) {
        return;
    }
    newLog->log = l;
    newLog->next = NULL;
    newLog->prev = NULL;
    p->isSorted = UNSORTED;

    if (p->lHead == NULL) {
        p->lHead = newLog;
        p->lTail = newLog;
    } else {
        // Otherwise, append the new node to the tail
        p->lTail->next = newLog;
        newLog->prev = p->lTail;
        p->lTail = newLog; // Update the tail pointer
    }
}

/**
 * @brief Adds a log to both the vehicle's log list and the park's log list.
 *
 * @param newLog A pointer to the log to be added.
 * @param v A pointer to the vehicle to which the log is being added.
 * @param p A pointer to the park to which the log is being added.
 */
void addLog(Log *newLog, Vehicle *v, Park *p) {
     // Add the new log to vehicle's log list
    newLog = addLogToVehicle(v, newLog);

    // Add the new log to park's log list
    addLogToPark(p, newLog);
}

/**
 * @brief Updates an entry log with exit date, exit time, and calculates the value.
 *
 * @param l A pointer to the entry log to be updated.
 * @param date A string representing the exit date.
 * @param time A string representing the exit time.
 * @param park A pointer to the park where the vehicle is exiting from.
 * @return A pointer to the updated log if successful, or NULL if memory allocation fails.
 */
Log *updateEntryLog(Log *l, char *date, char *time, Park *park) {

    // Allocate memory for exitDate and exitTime and copy the new values
    l->exitDate = createDateStruct(date);
    if (l->exitDate == NULL) {
        // Handle allocation failure
        return NULL;
    }

    l->exitTime = createTimeStruct(time);
    if (l->exitTime == NULL) {
        free(l->exitDate);  // Free previously allocated memory
        return NULL;
    }

    l->value = calculateValue(l, park);
    l->type = EXIT; // Update the type to indicate it's an exit log
    return l;
}

/**
 * @brief Creates a new log for a vehicle entering a park.
 *
 * @param v A pointer to the vehicle associated with the log.
 * @param p A pointer to the park where the vehicle is entering.
 * @param d A string representing the entry date.
 * @param t A string representing the entry time.
 * @return A pointer to the newly created log if successful, or NULL if memory allocation fails.
 */
Log *createLog(Vehicle *v, Park *p, char *d, char *t) {
    Log *newLog = (Log *)malloc(sizeof(Log));
    if (newLog == NULL) {
        return NULL;
    }

    newLog->entryDate = createDateStruct(d);
    newLog->entryTime = createTimeStruct(t);
    newLog->exitDate = NULL;
    newLog->exitTime = NULL;
    newLog->type = ENTRY;

    newLog->reg = v->registration;
    newLog->parkName = p->name;
    addLog(newLog, v, p);

    return newLog;
} 

/**
 * @brief Prints the entry and exit information for a log node.
 *
 * @param cur The current log node whose entry and exit information to print.
 * @param dEntry A string representing the entry date.
 * @param tEntry A string representing the entry time.
 */
void printExits(LogNode *cur, char *dEntry, char *tEntry) {
    char *dExit = dateToString(cur->log->exitDate);
    char *tExit = timeToString(cur->log->exitTime);
    printf("%s %s %s %s %s\n", cur->log->parkName, 
    dEntry, 
    tEntry, 
    dExit, 
    tExit);
    free(dEntry);
    free(tEntry);
    free(dExit);
    free(tExit);
}

/**
 * @brief Prints the logs associated with a vehicle.
 *
 * @param v A pointer to the vehicle whose logs to print.
 */
void printVehicleLogs(Vehicle *v) {
    LogNode *cur = v->lHead;
    cur = sortLogListName(v);
    while (cur != NULL) {
        // If its an entry log, print only entry info
        char *dEntry = dateToString(cur->log->entryDate);
        char *tEntry = timeToString(cur->log->entryTime);
        if (cur->log->type == 0) {
            printf("%s %s %s\n", cur->log->parkName, dEntry, tEntry);
            free(dEntry);
            free(tEntry);
        } else {
            printExits(cur, dEntry, tEntry);
        }
        cur = cur->next;
    }
}

/**
 * @brief Prints logs sorted by date and calculates revenue.
 *
 * @param head A pointer to the head of the log list.
 */
void printLogsByDate(LogNode *head) {
    LogNode *cur = head;
    int prevDay = INVALID;
    int prevMonth = INVALID;
    int prevYear = INVALID;
    double totalRevenue = 0.0;

    while (cur != NULL) {
        if (cur->log->type == ENTRY) {
            cur = cur->next;
            continue;
        }
        int curDay = cur->log->exitDate->day;
        int curMonth = cur->log->exitDate->month;
        int curYear = cur->log->exitDate->year;

        // If the current date is different from the previous one, print accumulated revenue
        if ((curDay != prevDay || curMonth != prevMonth || curYear != prevYear) && prevDay != -1 && prevMonth != -1 && prevYear != -1) {
            printf("%02d-%02d-%04d %.2f\n", prevDay, prevMonth, prevYear, totalRevenue);
            totalRevenue = 0.0; // Reset accumulated revenue for the new date
        }

        // Accumulate revenue
        totalRevenue += cur->log->value;
        prevDay = curDay; // Update previous date
        prevMonth = curMonth;
        prevYear = curYear;
        cur = cur->next;
    }

    // Print the last date and accumulated revenue
    if (prevDay != -1 && prevMonth != -1 && prevYear != -1) {
        printf("%02d-%02d-%04d %.2f\n", prevDay, prevMonth, prevYear, totalRevenue);
    }
}

/**
 * @brief Prints revenue for logs corresponding to a specific date.
 *
 * @param cur A pointer to the current log node.
 * @param date A pointer to the date for which revenue is to be printed.
 */
void printRevenue(LogNode *cur, Date *date) {
    // Check if the provided date is valid and if the log represents an exit
    if (isValidDate(date) && cur->log->type == 1) {
        // Check if the log's exit date matches the provided date
        if (isLogDateBefore(date, cur->log->exitDate) && isSameDate(cur->log->exitDate, date)) {
            char *t = timeToString(cur->log->exitTime); // Convert exit time to string
            // Print registration number, exit time, and value
            printf("%s %s %.2f\n", cur->log->reg, t, cur->log->value); 
            free(t); // Free dynamically allocated memory
        }
    } 
}

/**
 * @brief Shows revenue information for a park.
 *
 * @param p A pointer to the park for which revenue information is to be shown.
 * @param date A pointer to the date for which revenue is to be shown. Pass NULL
 *             to show revenue for all logs associated with the park.
 */
void showParkRevenue(Park* p, Date* date) {
    LogNode *cur = p->lHead; // Initialize current log node pointer
    
    while (cur != NULL) {
        // Check if the log is associated with the current park
        if (strcmp(cur->log->parkName, p->name) == 0) {
            // If no date is provided, print revenue information for all logs
            if (date == NULL) {
                if (cur->log->type == EXIT) {
                    printLogsByDate(cur); // Print revenue information for all logs by date
                    break;
                } 
            } else {
                printRevenue(cur, date); // Print revenue for logs with the provided date
            }
        }
        cur = cur->next; // Move to the next log node
    }
}

/**
 * @brief Processes a new park request, creating a new Park.
 *
 * @param sys A pointer to the parking system.
 * @param buffer A pointer to the buffer containing park information.
 * @param name The name of the park.
 */
void newParkRequest(System* sys, Buffer* buffer, char* name) {
    char *maxCapacity, *quarterCost, *afterHourCost, *dailyCost;

    // Extract park information from the buffer
    maxCapacity = nextWord(buffer);
    quarterCost = nextWord(buffer);
    afterHourCost = nextWord(buffer);
    dailyCost = nextWord(buffer);
    // Check if the park request is valid
    if (isValidParkRequest(sys, name, atoi(maxCapacity), atof(quarterCost), atof(afterHourCost), atof(dailyCost))) {
        ParkNode *park = createPark(name, maxCapacity, quarterCost, afterHourCost, dailyCost);
        if (park != NULL) {
            addPark(sys, park);  // Add the new park to the parking system.
        }
    } 
    free(maxCapacity);
    free(quarterCost);
    free(afterHourCost);
    free(dailyCost);
}

/**
 * @brief Processes a revenue check for a specific park on a given date.
 * 
 * @param sys The parking system.
 * @param park The park for which revenue is to be checked.
 * @param date The date for which revenue is to be checked.
 */
void processRevenueCheck(System *sys, Park *park, char *date) {
        Date *d = createDateStruct(date);
    if (isValidDate(d) && isLogDateBefore(d, sys->lastDate)) { // Validate date 
        showParkRevenue(park, d);
    } else {
        printf(ERR_INVALID_DATE);
    }
    free(d);
}

/**
 * @brief Calculates the parking fee for a given log.
 *
 * @param log Pointer to the log for which the fee is calculated.
 * @param park Pointer to the park where the vehicle was parked.
 * @return The calculated parking fee.
 */
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

/**
 * @brief Creates a park or prints all parks in the system.
 *
 * @param sys A pointer to the parking system.
 * @param buffer A pointer to the buffer containing the command and its arguments.
 */
void commandP(System* sys, Buffer* buffer) {   
    char *name; // Declare a variable to store the park name
    name = nextWord(buffer);

    if (name == NULL) {
        printParks(sys); // Print all parks in the system
    } else {
        newParkRequest(sys, buffer, name); // Process a new park request
    }
    free(name);
}

/**
 * @brief Processes a request to remove a park from the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param buffer A pointer to the buffer containing the park name to be removed.
 */
void commandR(System* sys, Buffer* buffer) {
    char *name; // Declare a variable to store the park name

    name = nextWord(buffer);
    Park *p = getPark(sys, name); // Get the park by name
    if (p != NULL) {
        freeParkLogs(sys, p); // Free memory for logs associated with the park
        removePark(sys, name); // Remove the park from the system.
        printRemainingParks(sys); // Print the remaining parks in the system.

    } else {
        printf(ERR_NO_PARK, name);
    }
    free(name);
    name = NULL;
}

/**
 * @brief Executes the command for vehicle entry in a park.
 *
 * @param sys A pointer to the parking system.
 * @param buffer A pointer to the buffer containing command arguments.
 */
void commandE(System* sys, Buffer* buffer) {
    char *name, *reg, *date, *time;
    name = nextWord(buffer);
    reg = nextWord(buffer);
    date = nextWord(buffer);
    time = nextWord(buffer);
    // Check if the entry is valid 
    if (isValidRequest(sys, name, reg, date, time, 0)) {
        Park *park = getPark(sys, name);
        Vehicle *vehicle = getVehicle(sys, reg);
        if (vehicle == NULL) {
            Vehicle *v = createVehicle(sys, reg);
            enterPark(sys, park, v, date, time);
        } else {
            enterPark(sys, park, vehicle, date, time);
        }
        printf("%s %d\n", name, park->maxCapacity - park->currentLots);
    }
    freeArgs(name, reg, time, date);
}

/**
 * @brief Executes the command for vehicle exit from the parking system.
 *
 * @param sys A pointer to the parking system.
 * @param buffer A pointer to the buffer containing command arguments.
 */
void commandS(System* sys, Buffer* buffer) {
    char *name, *reg, *date, *time;

    name = nextWord(buffer);
    reg = nextWord(buffer);
    date = nextWord(buffer);
    time = nextWord(buffer);

    // Check if the exit is valid (registration is valid, time is valid)
    if (isValidRequest(sys, name, reg, date, time, 1)) {
        Park *park = getPark(sys, name);
        Vehicle *vehicle = getVehicle(sys, reg);
        exitPark(sys, park, vehicle, date, time);
    } 
    freeArgs(name, reg, time, date);
}

/**
 * @brief Executes the command for printing vehicle logs.
 *
 * @param sys A pointer to the parking system.
 * @param buffer A pointer to the buffer containing command arguments.
 */
void commandV(System* sys, Buffer* buffer) {
    char *reg;
    reg = nextWord(buffer);

    // Check if registration is valid
    if (reg == NULL || !isValidRegistration(reg)) {
        printf(ERR_INVALID_REG, reg);
        return;
    }
    
    Vehicle *v = getVehicle(sys, reg); // Retrieve vehicle information

    // Check if printing logs request is valid
    if (isValidPrintLogsRequest(v, reg) == 1) {
        printVehicleLogs(v);
    }

    free(reg);
    reg = NULL;
}

/**
 * @brief Executes the command for displaying park revenue.
 *
 * @param sys A pointer to the parking system.
 * @param buffer A pointer to the buffer containing command arguments.
 */
void commandF(System* sys, Buffer* buffer) {
    char *name, *date;
    name = nextWord(buffer);
    date = nextWord(buffer);
    Park *park = getPark(sys, name); // Retrieve park information

    if (isValidRevenueCheck(park, name)) {
        if (park->isSorted == UNSORTED) { // If list is not sorted, sort it
            sortListExitDate(park);
            park->isSorted = SORTED;
        } 
        
        if (date == NULL) { // Display park revenue daily
            showParkRevenue(park, NULL);
        } else {
            processRevenueCheck(sys, park, date); // Display park revenue based on date
        }
    }
    free(name); // Free allocated memory for name and date
    free(date);
}

/**
 * @brief Frees the memory allocated for command arguments.
 *
 * @param name A pointer to the name argument.
 * @param reg A pointer to the registration argument.
 * @param time A pointer to the time argument.
 * @param date A pointer to the date argument.
 */
void freeArgs(char *name, char *reg, char *time, char *date) {
    free(name);
    name = NULL;
    free(reg);
    reg = NULL;
    free(time);
    time = NULL;
    free(date);
    date = NULL;
}

/**
 * @brief Frees the memory allocated for the hash table.
 *
 * @param sys A pointer to the parking system.
 */
void freeHashTable(System *sys) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        VehicleHashNode *cur = sys->hashTable[i];
        while (cur != NULL) {
            VehicleHashNode *temp = cur;
            cur = cur->next;
            free(temp);
            temp = NULL;
        }
    }
}

/**
 * @brief Frees the memory allocated for a LogNode.
 *
 * @param node Pointer to the LogNode structure to be freed.
 */
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

/**
 * @brief Frees the memory allocated for all logs in the system.
 *
 * @param sys A pointer to the parking system.
 */
void freeLogs(System *sys) {
    ParkNode *cur = sys->pHead;
    while (cur != NULL) {
        LogNode *logCur = cur->parking->lHead;
        while (logCur != NULL) {
            LogNode *temp = logCur;
            logCur = logCur->next;
            free(temp->log->entryDate);
            temp->log->entryDate = NULL;
            free(temp->log->exitDate);
            temp->log->exitDate = NULL;
            free(temp->log->entryTime);
            temp->log->entryTime = NULL;    
            free(temp->log->exitTime);
            temp->log->exitTime = NULL;
            free(temp->log);
            temp->log = NULL;
            free(temp);
            temp = NULL;
        }
        cur = cur->next;
    }
}

/**
 * @brief Frees the memory allocated for all vehicles in the system.
 *
 * @param sys A pointer to the parking system.
 */
void freeVehicles(System *sys) {
    VehicleNode *cur = sys->vHead;
    while (cur != NULL) {
        free(cur->vehicle->registration);
        cur->vehicle->registration = NULL;

        LogNode *logCur = cur->vehicle->lHead;
        while (logCur != NULL) {
            LogNode *temp = logCur;
            logCur = logCur->next;
            free(temp);
            temp = NULL;
        }

        free(cur->vehicle);
        cur->vehicle = NULL;
        VehicleNode *temp = cur;
        cur = cur->next;
        free(temp);
        temp = NULL;
    }
}

/**
 * @brief Frees the memory allocated for all parks in the system.
 *
 * @param sys A pointer to the parking system.
 */
void freeParks(System *sys) {
    ParkNode *cur = sys->pHead;
    while (cur != NULL) {
        free(cur->parking->name);
        cur->parking->name = NULL;
        free(cur->parking);
        cur->parking = NULL;
        ParkNode *temp = cur;
        cur = cur->next;
        free(temp);
        temp = NULL;
    }
}

/**
 * @brief Terminates the parking system and frees all allocated memory.
 *
 * @param sys A pointer to the parking system.
 * @param buffer A pointer to the buffer used for command inputs.
 */
void terminate(System* sys, Buffer* buffer) {
    free(sys->lastDate);
    sys->lastDate = NULL;
    free(sys->lastTime);
    sys->lastTime = NULL;

    // Free all the logs in the system
    freeLogs(sys);
    freeVehicles(sys);
    freeParks(sys);
    freeHashTable(sys);

    free(sys);
    free(buffer->buffer);
    free(buffer);
}

/**
 * @brief Processes a request based on the command provided in the buffer.
 * 
 * @param sys The parking system.
 * @param buffer The buffer containing the command.
 */
void processRequest(System *sys, Buffer *buffer) {
    switch (buffer->buffer[0]) {
        case 'p':
            buffer->index = 2;
            commandP(sys, buffer);
            break;
        case 'e':
            buffer->index = 2;
            commandE(sys, buffer);
            break;
        case 's':
            buffer->index = 2;
            commandS(sys, buffer);
            break;
        case 'v':
            buffer->index = 2;
            commandV(sys, buffer);
            break;
        case 'f':
            buffer->index = 2;
            commandF(sys, buffer);
            break;
        case 'r':
            buffer->index = 2;
            commandR(sys, buffer);
            break;
        case 'q':
            terminate(sys, buffer);
            exit(SUCCESS);
        default:
            break;
    }
}

/**
 * @brief The main function of the program.
 *
 * @return SUCCESS upon successful execution.
 */
int main() {
    // Initializes the buffer
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    buffer->buffer = (char *)malloc(BUFSIZE);

    // Initializes the parking system
    System *sys = init();
    if (sys == NULL) {
        exit(1);
    }
    
    while (TRUE) {
        // Reads the input and stores it in the buffer
        buffer = getBuffer(buffer);
        processRequest(sys, buffer);
    }
    return SUCCESS;
}