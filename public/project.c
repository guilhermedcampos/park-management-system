#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "project.h"
#include "constants.h"
#include "projectaux.h"

//

/**
 * @brief Initializes the parking system.
 *
 * Initializes the parking system by allocating memory for the system structure,
 * initializing its fields, and setting up arrays for parks and hash tables.
 *
 * @return A pointer to the initialized parking system if successful, otherwise NULL.
 */
ParkingSystem* init() {
    // Allocate memory for the parking system
    ParkingSystem *system = (ParkingSystem *)malloc(sizeof(ParkingSystem));
    if (system == NULL) {
        return NULL; // Memory allocation failed
    }

    // Initialize fields of the parking system structure
    system->pHead = NULL;
    system->vHead = NULL;
    system->vTail = NULL;
    system->lastDate = NULL;
    system->lastTime = NULL;
    system->numParks = 0;

    // Initialize the parks array and the hash table
    initParksArray(system);
    initHashTable(system);

    return system;
}

/**
 * @brief Initializes the parks array in the parking system.
 *
 * @param system A pointer to the parking system.
 */
void initParksArray(ParkingSystem *system) {
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        system->parks[i] = NULL;
    }
}

/**
 * @brief Initializes the hash table in the parking system.
 *
 * @param system A pointer to the parking system.
 */
void initHashTable(ParkingSystem *system) {
    // Set all buckets of the hash table to NULL
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        system->hashTable[i] = NULL;
    }
}

/**
 * @brief Adds a park to the array of parks in the parking system.
 *
 * @param system A pointer to the parking system.
 * @param park A pointer to the park to be added.
 */
void addParkToArray(ParkingSystem *system, Park *park) {
    // Iterate through the array of parks to find the first available slot
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        if (system->parks[i] == NULL) {
            // Insert the park into the first available slot and return
            system->parks[i] = park;
            return;
        }
    }
}

/**
 * @brief Adds a parking node to the linked list of parks in the parking system.
 *
 * @param system A pointer to the parking system.
 * @param parking A pointer to the parking node to be added.
 */
void addParkToList(ParkingSystem *system, ParkNode *parking) {
    // Iterate through the list to find an empty spot
    if (system->pHead == NULL) {
        // If the list is empty, set the parking node as the head
        system->pHead = parking;
        parking->next = NULL;
    } else {
        // Otherwise, iterate through the list to find the last node
        ParkNode *cur = system->pHead;
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
 * @param system A pointer to the parking system.
 * @param parking A pointer to the parking node containing the park to be added.
 */
void addPark(ParkingSystem *system, ParkNode *parking) {
    // Add the park to the array of parks
    addParkToArray(system, parking->parking);

    // Add the park to the linked list of parks
    addParkToList(system, parking);

    // Increment the count of parks in the system
    system->numParks++;
}

/**
 * @brief Updates the array of parks in the parking system.
 *
 * @param system A pointer to the parking system.
 * @param index The index from which elements are to be shifted.
 */
void updateParksArray(ParkingSystem *system, int index) {
    // Shift elements to the left starting from the index
    for (int i = index; i < MAX_PARKING_LOTS - 1; i++) {
        system->parks[i] = system->parks[i + 1];
    }

    // Set the last element to NULL (as it's shifted left)
    system->parks[MAX_PARKING_LOTS - 1] = NULL;
}

/**
 * @brief Removes a park from the array of parks in the parking system.
 *
 * @param system A pointer to the parking system.
 * @param name The name of the park to be removed.
 */
void removeParkFromArray(ParkingSystem *system, char *name) {
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        if (system->parks[i] != NULL && strcmp(system->parks[i]->name, name) == 0) {
            // First element of the list, shift left the array after removing first element
            updateParksArray(system, i);
        }
    }
}

/**
 * @brief Removes a park node from the list in the parking system.
 *
 * This function removes the park node with the specified name from the list in the parking system.
 *
 * @param system A pointer to the parking system.
 * @param name The name of the park to be removed.
 */
void removeParkFromList(ParkingSystem *system, char *name) {
    ParkNode *cur = system->pHead; /**< Current node pointer */
    ParkNode *prev = NULL; /**< Previous node pointer */
    
    // Iterate through the list to find the node with the specified name
    while (cur != NULL) {
        if (strcmp(cur->parking->name, name) == 0) {
            // If the node to be removed is the head of the list
            if (prev == NULL) {
                system->pHead = cur->next;
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
 * @param system A pointer to the parking system.
 * @param name The name of the park to be removed.
 */
void removePark(ParkingSystem *system, char *name) {
    // If the list of parks is empty, return
    if (system->pHead == NULL) {
        return;
    }

    // Remove the park from the array and the linked list
    removeParkFromArray(system, name);
    removeParkFromList(system, name);
    
    // Decrement the number of parks in the system
    system->numParks--;
}

/**
 * @brief Prints information about all parks in the parking system.
 *
 * @param system A pointer to the parking system.
 */
void printParks(ParkingSystem* system) {
    // Iterate through the parks array
    for (int i = 0; i < MAX_PARKING_LOTS; i++) {
        // Check if the park at index i is not NULL
        if (system->parks[i] != NULL) {
            // Print park information: name, max capacity, and available parking lots
            printf("%s %d %d\n", system->parks[i]->name, system->parks[i]->maxCapacity, system->parks[i]->maxCapacity - system->parks[i]->currentLots);
        }
    }
}

/**
 * @brief Sorts the linked list of parks by name in the parking system.
 *
 * @param sys A pointer to the parking system.
 * @return A pointer to the head of the sorted list.
 */
void printRemainingParks(ParkingSystem* system) {
    // Sort the list by park names alphabetically
    ParkNode *cur = sortListName(system);
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
Park* getPark(ParkingSystem* sys, char* name) {
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
 * This function creates a new park node with the provided parameters.
 *
 * @param name The name of the park.
 * @param maxCapacity The maximum capacity of the park.
 * @param billingValue15 The billing value for the first 15 minutes.
 * @param billingValueAfter1Hour The billing value after the first hour.
 * @param maxDailyValue The maximum daily billing value.
 * @return A pointer to the newly created park node if successful, otherwise NULL.
 */
ParkNode *createPark(char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue) {
    Park *newPark = createParkData(name, maxCapacity, billingValue15, billingValueAfter1Hour, maxDailyValue);
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
 * This function creates a new park structure and initializes its fields.
 *
 * @param name The name of the park.
 * @param maxCapacity The maximum capacity of the park.
 * @param billingValue15 The billing value for the first 15 minutes.
 * @param billingValueAfter1Hour The billing value after the first hour.
 * @param maxDailyValue The maximum daily billing value.
 * @return A pointer to the newly created park structure if successful, otherwise NULL.
 */
Park *createParkData(char *name, char *maxCapacity, char *billingValue15, char *billingValueAfter1Hour, char *maxDailyValue) {
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
    newPark->billingValue15 = atof(billingValue15);
    newPark->billingValueAfter1Hour = atof(billingValueAfter1Hour);
    newPark->maxDailyValue = atof(maxDailyValue);

    return newPark;
}

/**
 * @brief Adds a vehicle to the linked list of vehicles in the parking system.
 *
 * This function creates a new node for the given vehicle and adds it to the linked list
 * of vehicles in the parking system.
 *
 * @param system A pointer to the parking system.
 * @param vehicle A pointer to the vehicle to be added.
 */
void addVehicleToList(ParkingSystem *system, Vehicle *vehicle) {
    // Initialize the vehicle node pointers
    VehicleNode *vehicleNode = (VehicleNode *)malloc(sizeof(VehicleNode));
    if (vehicleNode == NULL) {
        // Handle memory allocation failure
        return;
    }
    vehicleNode->prev = NULL;
    vehicleNode->next = NULL;
    vehicleNode->vehicle = vehicle;

    if (system->vHead == NULL) {
        // If the list is empty, set both head and tail to the new node
        system->vHead = vehicleNode;
        system->vTail = vehicleNode;
    } else {
        // Otherwise, append the new node to the tail
        system->vTail->next = vehicleNode;
        vehicleNode->prev = system->vTail;
        system->vTail = vehicleNode; // Update the tail pointer
    }
}

// Hash function to map registration strings to indices
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
 * @param system A pointer to the parking system.
 * @param vehicle A pointer to the vehicle to be added.
 */
void addToHashTable(ParkingSystem *system, Vehicle *vehicle) {
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
    newNode->next = system->hashTable[index];
    system->hashTable[index] = newNode;
}

/**
 * @brief Adds a vehicle to the parking system.
 *
 * @param system A pointer to the parking system.
 * @param vehicle A pointer to the vehicle to be added.
 */
void addVehicle(ParkingSystem *system, Vehicle *vehicle) {
    // Add the vehicle to the linked list of vehicles
    addVehicleToList(system, vehicle);

    // Add the vehicle to the hash table for efficient lookup
    addToHashTable(system, vehicle);
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
 * @param system A pointer to the parking system.
 * @param reg The registration number of the vehicle.
 * @return A pointer to the newly created vehicle if successful, or NULL on failure.
 */
Vehicle *createVehicle(char *reg) {
    Vehicle *vehicle = createVehicleData(reg);
    if (vehicle == NULL) {
        return NULL; // Creation of vehicle data failed
    }

    return vehicle;
}

/**
 * @brief Enters a vehicle into a park in the parking system.
 *
 * Updates the necessary data structures to indicate that the vehicle
 * has entered the specified park at the given date and time.
 *
 * @param sys A pointer to the parking system.
 * @param p A pointer to the park where the vehicle is entering.
 * @param v A pointer to the vehicle entering the park.
 * @param date The date of entry in "YYYY-MM-DD" format.
 * @param time The time of entry in "HH:MM" format.
 * @return 0 if the operation is successful, -1 if memory allocation fails.
 */
int enterPark(ParkingSystem *sys, Park *p, Vehicle *v, char *date, char *time) {

    updateSystem(sys, date, time);
    v->parkName = p->name;
    v->isParked = 1;
    p->currentLots++;
    v->lastLog = createLog(v, p, date, time);
    return 0;
}

/**
 * @brief Prints the exit details of a vehicle.
 *
 * This function prints the registration number of the vehicle along with its entry
 * and exit date and time, as well as the parking fee incurred.
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
 * @param system A pointer to the parking system.
 * @param p A pointer to the park from which the vehicle is exiting.
 * @param v A pointer to the vehicle exiting the park.
 * @param date A string representing the exit date of the vehicle.
 * @param time A string representing the exit time of the vehicle.
 * @return Returns 0 upon successful exit, or 1 if there is a failure in the process.
 */
int exitPark(ParkingSystem *system, Park *p, Vehicle *v, char *date, char *time) {
    updateSystem(system, date, time);  
    v->parkName = NULL;
    v->isParked = 0;    
    p->currentLots--;
    Log *l = updateEntryLog(v->lastLog, date, time, p);
    if (l == NULL) {
        return 1;
    }
    printExit(v);
    return 0;
}

/**
 * @brief Updates the system's last date and time.
 *
 * This function updates the last date and time of the system with the provided date and time values.
 *
 * @param system A pointer to the parking system to be updated.
 * @param date A string representing the new date.
 * @param time A string representing the new time.
 */
void updateSystem(ParkingSystem *system, char *date, char *time) {
    Date *d = createDateStruct(date);
    Time *t = createTimeStruct(time);

    Date *newDate = (Date *)realloc(system->lastDate, sizeof(Date));
    if (newDate == NULL) {
        free(d); 
        free(t);
        return; 
    }
    system->lastDate = newDate;
    memcpy(system->lastDate, d, sizeof(Date)); 

    Time *newTime = (Time *)realloc(system->lastTime, sizeof(Time));
    if (newTime == NULL) {
        free(d); 
        free(t);
        return; 
    }
    system->lastTime = newTime;
    memcpy(system->lastTime, t, sizeof(Time));

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
    p->isSorted = 0;

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
    l->type = 1; // Update the type to indicate it's an exit log
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
    newLog->type = 0;

    newLog->reg = v->registration;
    newLog->parkName = p->name;
    addLog(newLog, v, p);

    return newLog;
} 

int printVehicleLogs(Vehicle *v) {
    LogNode *cur = v->lHead;
    cur = sortLogListName(v);
    int numLogs = 0;
    while (cur != NULL) {
        // If its an entry log, print only entry info
        char *dEntry = dateToString(cur->log->entryDate);
        char *tEntry = timeToString(cur->log->entryTime);
        if (cur->log->type == 0) {
            printf("%s %s %s\n", cur->log->parkName, dEntry, tEntry);
            free(dEntry);
            free(tEntry);
        } else {
            // If its an exit log, print entry and exit info
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
        numLogs++;
        cur = cur->next;
    }
    return numLogs;   
}

void printLogsByDate(LogNode *head) {
    LogNode *cur = head;
    int prevDay = -1;
    int prevMonth = -1;
    int prevYear = -1;
    double totalRevenue = 0.0;

    while (cur != NULL) {
        if (cur->log->type == 0) {
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


void showParkRevenue(Park* p, Date* date) {
    LogNode *cur;
    if (p->isSorted == 0) {
        cur = sortListExitDate(p);
        p->isSorted = 1;
    } else {
        cur = p->lHead;
    }

    if (cur == NULL) {
        return;
    }
    
    while (cur != NULL) {
        if (strcmp(cur->log->parkName, p->name) == 0) {
            if (date == NULL) {
                if (cur->log->type == 1) {
                    printLogsByDate(cur);
                    break;
                } 
            } else {
                if (isValidDate(date) && cur->log->type == 1) {
                    if (isLogDateValid(date, cur->log->exitDate) && isSameDate(cur->log->exitDate, date)) {
                        char *t = timeToString(cur->log->exitTime);
                        printf("%s %s %.2f\n", cur->log->reg, t, cur->log->value);
                        free(t);
                    }
                } 
            }
        }
        cur = cur->next;
    }
}

void printParkLogs(Park* p) {
    LogNode *cur = p->lHead;
    if (cur == NULL) {
        return;
    }
    while (cur != NULL) {
            if (cur->log->type == 1) {
                char *dEntry = dateToString(cur->log->entryDate);
                char *tEntry = timeToString(cur->log->entryTime);
                char *dExit = dateToString(cur->log->exitDate);
                char *tExit = timeToString(cur->log->exitTime);
                printf("%s %s %s %s %s %.2f\n", cur->log->reg, dEntry, tEntry, dExit, tExit, cur->log->value);
                free(dEntry);
                free(tEntry);
                free(dExit);
                free(tExit);
            }
        cur = cur->next;
    }
}

void commandP(ParkingSystem* system, Buffer* buffer) {   
    char *name, *maxCapacity, *billingValue15, *billingValueAfter1Hour, *maxDailyValue;

    name = nextWord(buffer);

    if (name == NULL) {
        // If there are no more arguments, list the parking lots
        printParks(system);
    } else {
        // Checks if the parking lot already exists
        if (getPark(system, name) != NULL) {
            printf("%s: parking already exists.\n", name);
        } else {
            maxCapacity = nextWord(buffer);
            billingValue15 = nextWord(buffer);
            billingValueAfter1Hour = nextWord(buffer);
            maxDailyValue = nextWord(buffer);
            if (isValidParkRequest(system->numParks, atoi(maxCapacity), atof(billingValue15), atof(billingValueAfter1Hour), atof(maxDailyValue))) {
                ParkNode *park = createPark(name, maxCapacity, billingValue15, billingValueAfter1Hour, maxDailyValue);
                if (park != NULL) {
                    addPark(system, park);
                }
                
            } 
            free(maxCapacity);
            free(billingValue15);
            free(billingValueAfter1Hour);
            free(maxDailyValue);
        }
        free(name);
    }
}

void commandR(ParkingSystem* system, Buffer* buffer) {
    char *name;

    name = nextWord(buffer);
    Park *p = getPark(system, name);
    if (p != NULL) {
        freeParkLogs(system, p);
        removePark(system, name);
        printRemainingParks(system);

    } else {
        printf("%s: no such parking.\n", name);
    }
    free(name);
}

void commandE(ParkingSystem* system, Buffer* buffer) {
    char *name, *reg, *date, *time;

    name = nextWord(buffer);
    reg = nextWord(buffer);
    date = nextWord(buffer);
    time = nextWord(buffer);
    // Check if the entry is valid (park exists, park is not full, registration is valid, time is valid)
    if (isValidRequest(system, name, reg, date, time, 0)) {
        Park *park = getPark(system, name);
        Vehicle *vehicle = getVehicle(system, reg);
        if (vehicle == NULL) {
            Vehicle *v = createVehicle(reg);
            addVehicle(system, v);
            enterPark(system, park, v, date, time);
            printf("%s %d\n", name, park->maxCapacity - park->currentLots);
        } else {
            enterPark(system, park, vehicle, date, time);
            printf("%s %d\n", name, park->maxCapacity - park->currentLots);
        }
    }
    free(name);
    name = NULL;
    free(reg);
    reg = NULL;
    free(date);
    date = NULL;
    free(time);
    time = NULL;
}

void commandS(ParkingSystem* system, Buffer* buffer) {
    char *name, *reg, *date, *time;

    name = nextWord(buffer);
    reg = nextWord(buffer);
    date = nextWord(buffer);
    time = nextWord(buffer);

    // Check if the exit is valid (registration is valid, time is valid)
    if (isValidRequest(system, name, reg, date, time, 1)) {
        Park *park = getPark(system, name);
        Vehicle *vehicle = getVehicle(system, reg);
        exitPark(system, park, vehicle, date, time);
    } 
    free(name);
    free(reg);
    free(date);
    free(time);
}

void commandV(ParkingSystem* system, Buffer* buffer) {
    char *reg;

    reg = nextWord(buffer);

    if (reg == NULL || !isValidRegistration(reg)) {
        printf("%s: invalid licence plate.\n", reg);
        return;
    }

    Vehicle *v = getVehicle(system, reg);
    if (v == NULL) {
        printf("%s: no entries found in any parking.\n", reg);
        return;
    }

    if (v->lHead == NULL) {
        printf("%s: no entries found in any parking.\n", reg);
        return;
    }

    printVehicleLogs(v);
    free(reg);
}

// Checks total revenue of a park
void commandF(ParkingSystem* system, Buffer* buffer) {
    char *name, *date;

    name = nextWord(buffer);
    date = nextWord(buffer);

    Park *park = getPark(system, name);
    if (park == NULL) {
        printf("%s: no such parking.\n", name);
        return;
    }
    if (date == NULL) {
        showParkRevenue(park, NULL);
    } else {
        Date *d = createDateStruct(date);
        if (isValidDate(d) && isLogDateValid((d),system->lastDate)) {  // and logDateValid of last entry date
            showParkRevenue(park, d);
        } else {
            printf("invalid date.\n");
        }
        free(d);
    }
    free(name);
    free(date);
}

void freeHashTable(ParkingSystem *system) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        VehicleHashNode *cur = system->hashTable[i];
        while (cur != NULL) {
            VehicleHashNode *temp = cur;
            cur = cur->next;
            free(temp);
            temp = NULL;
        }
    }
}

void freeLogs(ParkingSystem *system) {
    ParkNode *cur = system->pHead;
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

void freeVehicles(ParkingSystem *system) {
    VehicleNode *cur = system->vHead;
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

void freeParks(ParkingSystem *system) {
    ParkNode *cur = system->pHead;
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

void terminate(ParkingSystem* system, Buffer* buffer) {
    free(system->lastDate);
    system->lastDate = NULL;
    free(system->lastTime);
    system->lastTime = NULL;

    // Free all the logs in the system
    freeLogs(system);
    freeVehicles(system);
    freeParks(system);
    freeHashTable(system);

    free(system);
    free(buffer->buffer);
    free(buffer);
}

int main() {
    // Initializes the buffer
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    ParkingSystem *system;
    buffer->buffer = (char *)malloc(BUFSIZE);

    system = init();
    if (system == NULL) {
        exit(1);
    }
    
    while (1) {
        buffer = getBuffer(buffer);

        switch (buffer->buffer[0]) {

            // Closes the program
            case 'q':
                terminate(system, buffer);

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
                commandV(system, buffer);
                break;
            
            // Shows revenue of a parking lot
            case 'f':
                buffer->index = 2;
                commandF(system, buffer);

                break;

             case 'r':
                buffer->index = 2;
                commandR(system, buffer);

                break;
            default:
                break;
        }
    }
    return 0;
}