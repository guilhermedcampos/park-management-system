#ifndef PROJECTAUX_H
#define PROJECTAUX_H

#include "project.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

LogNode *sortLogListName(Vehicle *vehicle);
void sortListExitDate(Park *park);

int isValidParkRequest(System *sys, char *name, int cap, double x, double y, double z);
int isValidPrintLogsRequest(Vehicle *v, char *reg);
int isValidRevenueCheck(Park *park, char *name);
int isValidDate(Date *date);
int isValidTime(Time *time);
int isSameDate(Date *d1, Date *d2);
void freeParkLogs(System *sys, Park *p);
char *dateToString(Date *date);
char *timeToString(Time *time);
int isParkFull(System* sys, char* name);
int isValidRequest(System *sys, char * name, char *reg, char *date, char *time, int type);
int isVehicleInParkExit(System *sys, char *reg, char *name);
int isVehicleParked(System *sys, char *reg);
int isLogDateValid(Date *d1, Date *d2);
int isLogTimeValid(Time *t1, Time *t2);
int isDateBefore(Date *d1, Date *d2, Time *t1, Time *t2);
int isValidLog(System *sys, Time *time, Date *date);
Vehicle *getVehicle(System *sys, char *reg);
ParkNode *sortListName(System *sys);
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

