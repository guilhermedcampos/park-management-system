#ifndef PROJECTAUX_H
#define PROJECTAUX_H

#include "project.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidDate(Date *date);
int isValidTime(Time *time);
int isLeapYear(int year);
int isValidRequest(ParkingSystem *system, char * name, char *reg, char *date, char *time);
int isVehicleInPark(ParkingSystem *system, char *reg, char *name);
int isLogDateValid(Date *d1, Date *d2);
int isLogTimeValid(Time *t1, Time *t2);
int isValidLogAux(Date *d1, Date *d2, Time *t1, Time *t2);
int isValidLog(ParkingSystem *system, Time *time, Date *date);
Vehicle *getVehicle(ParkingSystem *system, char *reg);
Log *findEntryLog(ParkingSystem *system, char *reg, char *name);
Buffer *getBuffer(Buffer *buffer);
int isValidRegistration(char *reg);
Time *createTimeStruct(char *time);
Date *createDateStruct(char *date);
char *nextWord(Buffer *buffer);

#endif // PROJECTAUX_H

