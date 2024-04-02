/*
 * iaed-24 - ist1106909 - project
 * Header file with all constants
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_PARKING_LOTS 20   // Maximum number of parking lots
#define BUFSIZE 8192          // Maximum buffer size
#define REGISTRATION_LENGTH 8 // Maximum registration length
#define HASH_TABLE_SIZE 1000  // Hash table size

#define ERR_INVALID_DATE "invalid date.\n"
#define ERR_NO_PARK "%s: no such parking.\n"
#define ERR_INVALID_REG "%s: invalid licence plate.\n"
#define ERR_PARK_EXISTS "%s: parking already exists.\n"
#define ERR_PARK_FULL "%s: parking is full.\n"
#define ERR_INVALID_CAP "%d: invalid capacity.\n"
#define ERR_INVALID_COST "invalid cost.\n"
#define ERR_TOO_MANY_PARKS "too many parks.\n"
#define ERR_INVALID_ENTRY "%s: invalid vehicle entry.\n"
#define ERR_INVALID_EXIT "%s: invalid vehicle exit.\n"


#endif // CONSTANTS_H