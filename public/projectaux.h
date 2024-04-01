#ifndef PROJECTAUX_H
#define PROJECTAUX_H

#include "project.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

LogNode *sortLogListName(Vehicle *vehicle);
void sortListExitDate(Park *park);

int isValidParkRequest(ParkingSystem *system, char *name, int cap, double x, double y, double z);
int isValidPrintLogsRequest(Vehicle *v, char *reg);
int isValidDate(Date *date);
int isValidTime(Time *time);
int isSameDate(Date *d1, Date *d2);
void freeParkLogs(ParkingSystem *system, Park *p);
char *dateToString(Date *date);
char *timeToString(Time *time);
int isParkFull(ParkingSystem* sys, char* name);
int isValidRequest(ParkingSystem *system, char * name, char *reg, char *date, char *time, int type);
int isVehicleInParkExit(ParkingSystem *system, char *reg, char *name);
int isVehicleParked(ParkingSystem *system, char *reg);
int isLogDateValid(Date *d1, Date *d2);
int isLogTimeValid(Time *t1, Time *t2);
int isDateBefore(Date *d1, Date *d2, Time *t1, Time *t2);
int isValidLog(ParkingSystem *system, Time *time, Date *date);
Vehicle *getVehicle(ParkingSystem *system, char *reg);
ParkNode *sortListName(ParkingSystem *sys);
size_t getTimeDiff(Time *t1, Date *d1, Time *t2, Date *d2);
size_t dateInMinutes(Date *d, Time *t);
int getTotalMonthDays(int month);
double calculateValue(Log *log, Park *park);
int daysByMonth(int month);
Buffer *getBuffer(Buffer *buffer);
int isValidRegistration(char *reg);
Time *createTimeStruct(char *time);
Date *createDateStruct(char *date);
char *nextWord(Buffer *buffer);

#endif // PROJECTAUX_H

