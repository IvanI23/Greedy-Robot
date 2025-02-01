#include "background.h"
#include "animation.h"
#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include "DrawApp/graphics.h"

int main(){

    srand(time(NULL));

    TILE** tiles = tileMemory();
    createTiles(tiles);
    setBackground(tiles);
    
    ROBOT* robot = robotMemory();
    moveRobot(robot, tiles);


    freeTiles(tiles);
    freeRobot(robot);


    return 0;
}