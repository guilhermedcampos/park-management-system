#ifndef PROJECTAUX_H
#define PROJECTAUX_H

#include "project.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidDate(Date *date);
int isValidTime(Time *time);
int isLeapYear(int year);
int isValidEntry(ParkingSystem *system, char * name, char *reg, char *date, char *time);
Vehicle *getVehicle(ParkingSystem *system, char *reg);
Buffer *getBuffer(Buffer *buffer);
int isValidRegistration(char *reg);
Time *createTimeStruct(char *time);
Date *createDateStruct(char *date);
char *nextWord(Buffer *buffer);

#endif // PROJECTAUX_H

