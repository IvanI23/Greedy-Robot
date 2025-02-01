#include "background.h"

enum DIRECTION{
    UP,
    DOWN,
    LEFT,
    RIGHT
};


typedef struct robot{
    bool isHome;

    int markerCollected,
        markerDropped,
        markerDepositted,
        slowness;

     enum DIRECTION direction;
    
}ROBOT;

ROBOT* robotMemory();

void update(ROBOT* robot, int* x, int* y);
void setRobot(ROBOT* robot);
void left(ROBOT* robot, TILE* tile);
void right(ROBOT* robot, TILE* tile);
void forward(ROBOT* robot, TILE** tiles, TILE* tile);
void moveRobot(ROBOT* robot, TILE** tiles);
void freeRobot(ROBOT* robot);
void pickUpMarker(ROBOT* robot, TILE* tile);
void dropMarker(ROBOT* robot, TILE* tile);
void depositMarker(ROBOT* robot, TILE* tile);

int canMoveForward(ROBOT* robot, TILE** tiles, TILE* tile);
int atMarker(TILE* tile);
int atCorner(TILE* tile);
int atCorner(TILE* tile);
int markerCount(ROBOT* robot);

TILE* updateTile(ROBOT* robot, TILE** tiles, TILE* tile);