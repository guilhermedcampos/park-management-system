/*
 * iaed-24 - ist1106909 - project
 * Header file with all the auxiliar functions prototypes.
 */
#ifndef PROJECTAUX_H
#define PROJECTAUX_H

#include "project.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Date and time related functions
 */
char *dateToString(Date *date);
char *timeToString(Time *time);
int isValidDate(Date *date);
int isValidTime(Time *time);
int isSameDate(Date *d1, Date *d2);
int isLogDateBefore(Date *d1, Date *d2);
int isLogTimeBefore(Time *t1, Time *t2);
int isDateBefore(Date *d1, Date *d2, Time *t1, Time *t2);
size_t dateInMinutes(Date *d, Time *t);
size_t getTimeDiff(Time *t1, Date *d1, Time *t2, Date *d2);
int getTotalMonthDays(int month);
int daysByMonth(int month);
double min(double a, double b); 
Time *createTimeStruct(char *time);
Date *createDateStruct(char *date);


/*
 * Validaty related functions
 */
int isValidLog(System *sys, Time *time, Date *date);
int isValidParkRequest(System *sys, char* name, int cap, double x, double y, double z);
int isValidDateRequest(System *sys, char *date, char *time);
int isValidRequest(System *sys, char *name, char *reg, char *date, char *time, int type);
int isValidPrintLogsRequest(Vehicle *v, char *reg);
int isValidRevenueCheck(Park *park, char *name);
int isValidPair(char cur, char next);
int isValidRegistration(char *reg); 
int isParkFull(System* sys, char* name);
int isVehicleParked(System *sys, char *reg);    
int isVehicleInParkExit(System *sys, char *reg, char *name);

/*
 * Log removal related functions
 */
void removeVehicleLog(System *sys, Park *p, char *reg);
void freeParkLogs(System *sys, Park *p);

/*
 * Sorting related functions
 */
void merge(LogNode **arr, int l, int m, int r);
void mergeSort(LogNode **arr, int l, int r);
LogNode *sortLogListName(Vehicle *v);
ParkNode *sortListName(System *sys);
int partition(LogNode **arr, int low, int high);
void quicksort(LogNode **arr, int low, int high);
void sortListExitDate(Park *park);

char *nextWord(Buffer *buffer);

#endif // PROJECTAUX_H

