#include "constants.h"

typedef struct ParkingLot {
    char name[256];
    int maxCapacity;
    double billingValue15;
    double billingValueAfter1Hour;
    double maxDailyValue;
} ParkingLot;

typedef struct ParkingNode {
    ParkingLot *parking;
    struct ParkingNode *next;
    struct ParkingNode *prev;
} ParkingNode;


typedef struct Buffer {
    char *buffer;
    int index;
} Buffer;

typedef struct ParkingSystem {
    ParkingNode *head;
    int numParkingLots;
} ParkingSystem;
