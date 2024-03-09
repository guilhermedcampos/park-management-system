#ifndef PROJECTAUX_H
#define PROJECTAUX_H

#include "project.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidEntry(ParkingSystem *system, char *reg, char *date, char *time);
Vehicle *getVehicle(ParkingSystem *system, char *reg);
Buffer *getBuffer(Buffer *buffer);
int isValidRegistration(char *reg);
char *nextWord(Buffer *buffer);

#endif // PROJECTAUX_H

