#ifndef PROJECTAUX_H
#define PROJECTAUX_H

#include "project.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char *dateToString(Date *date);
char *timeToString(Time *time);
int isValidDate(Date *date);
int isValidTime(Time *time);
int isSameDate(Date *d1, Date *d2);
int isLogDateBefore(Date *d1, Date *d2);
int isLogTimeBefore(Time *t1, Time *t2);
int isDateBefore(Date *d1, Date *d2, Time *t1, Time *t2);
size_t getTimeDiff(Time *t1, Date *d1, Time *t2, Date *d2);
size_t dateInMinutes(Date *d, Time *t);
Time *createTimeStruct(char *time);
Date *createDateStruct(char *date);


int isValidLog(System *sys, Time *time, Date *date);
int isValidParkRequest(System *sys, char* name, int cap, double x, double y, double z);
int isValidRequest(System *sys, char *name, char *reg, char *date, char *time, int type);
int isValidPrintLogsRequest(Vehicle *v, char *reg);
int isValidRevenueCheck(Park *park, char *name);

int isValidPair(char cur, char next);
int isValidRegistration(char *reg); 
int isParkFull(System* sys, char* name);
int isVehicleParked(System *sys, char *reg);    
int isVehicleInParkExit(System *sys, char *reg, char *name);


LogNode *sortLogListName(Vehicle *vehicle);
void sortListExitDate(Park *park);


void freeParkLogs(System *sys, Park *p);

Vehicle *getVehicle(System *sys, char *reg);
ParkNode *sortListName(System *sys);
int getTotalMonthDays(int month);
double calculateValue(Log *log, Park *park);
int daysByMonth(int month);
Buffer *getBuffer(Buffer *buffer);
char *nextWord(Buffer *buffer);

#endif // PROJECTAUX_H

