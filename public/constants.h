/*
 * iaed-24 - ist1106909 - project
 * Header file with all constants
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
 * Numeric constants.
 */
#define MAX_PARKING_LOTS 20   // Maximum number of parking lots
#define BUFSIZE 8192          // Maximum buffer size
#define REGISTRATION_LENGTH 8 // Maximum registration length
#define HASH_TABLE_SIZE 1000  // Hash table size

/*
 * Error messages.
 */
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

/*
 * Booleans.
*/
#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILURE 0
#define SORTED 1
#define UNSORTED 0

/*
 * Status of vehicle.
 */
#define ENTRY 0
#define EXIT 1

#endif // CONSTANTS_H