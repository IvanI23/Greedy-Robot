#include "animation.h"
#include "background.h"
#include "DrawApp/graphics.h"
#include <stdlib.h>
#include <stdio.h>


ROBOT* robotMemory(){
    ROBOT* robot = (ROBOT*)malloc(sizeof(ROBOT));
    if (robot == NULL) {
        return NULL; 
    }
    return robot;
}
TILE* updateTile(ROBOT* robot, TILE** tiles, TILE* tile){   //Updates the tile which the robot is currently on
    int i = tile->i;
    int j = tile->j;

    if (robot->direction == LEFT) {
        return &tiles[i-1][j];
    } else if (robot->direction == DOWN) {
        return &tiles[i][j+1];
    } else if (robot->direction == RIGHT) {
        return &tiles[i+1][j];
    } else if (robot->direction == UP) {
        return &tiles[i][j-1];
    }
}

void update(ROBOT* robot, int* x, int* y) {
    sleep(robot->slowness);
    clear();
    setColour(blue);
    fillPolygon(3, x, y);
    
}

void setRobot(ROBOT* robot){
    robot->isHome = true;
    robot->markerCollected = 0;
    robot->slowness = 10;      //Essentially the waitTime for sleep(waitTime) function
    robot->direction = RIGHT;
    robot->markerDepositted = 0;
}

void left(ROBOT* robot, TILE* tile){
    int x[3];
    int y[3];

    switch(robot-> direction){    //Changes the direction of the robot based on the current direction to simulate a 90 degrees anticlockwise turn
        case LEFT:
            x[0] = tile->xTop; x[1] = tile->xRight; x[2] = tile->xLeft;
            y[0] = tile->yTop; y[1] = tile->yBottom; y[2] = tile->yBottom;
            robot->direction = UP;
            break;
        case DOWN:
            x[0] = tile->xLeft; x[1] = tile->xRight; x[2] = tile->xRight;
            y[0] = tile->yLeft; y[1] = tile->yTop; y[2] = tile->yBottom;
            robot->direction = LEFT;
            break;
        case RIGHT:
            x[0] = tile->xBottom; x[1] = tile->xRight; x[2] = tile->xLeft;
            y[0] = tile->yBottom; y[1] = tile->yTop; y[2] = tile->yTop;
            robot->direction = DOWN;
            break;
        case UP:
            x[0] = tile->xLeft; x[1] = tile->xRight; x[2] = tile->xRight;
            y[0] = tile->yLeft; y[1] = tile->yTop; y[2] = tile->yBottom;
            robot->direction = RIGHT;
            break;
    }

    update(robot, x, y);
}

void right(ROBOT* robot, TILE* tile){
    int x[3];
    int y[3];

    switch(robot-> direction){        //Changes the direction of the robot based on the current direction to simulate a 90 degrees clockwise turn
        case LEFT:
            x[0] = tile->xBottom; x[1] = tile->xRight; x[2] = tile->xLeft;
            y[0] = tile->yBottom; y[1] = tile->yTop; y[2] = tile->yTop;
            robot->direction = DOWN;
            break;
        case DOWN:
            x[0] = tile->xRight; x[1] = tile->xLeft; x[2] = tile->xLeft;
            y[0] = tile->yRight; y[1] = tile->yBottom; y[2] = tile->yTop;
            robot->direction = RIGHT;
            break;
        case RIGHT:
            x[0] = tile->xTop; x[1] = tile->xRight; x[2] = tile->xLeft;
            y[0] = tile->yTop; y[1] = tile->yBottom; y[2] = tile->yBottom;
            robot->direction = UP;
            break;
        case UP:
            x[0] = tile->xRight; x[1] = tile->xLeft; x[2] = tile->xLeft;
            y[0] = tile->yRight; y[1] = tile->yBottom; y[2] = tile->yTop;
            robot->direction = LEFT;
            break;
    }

    update(robot, x, y);
}

int canMoveForward(ROBOT* robot, TILE** tiles, TILE* tile){
    int i = tile->i;
    int j = tile->j;
    int check;

    switch(robot-> direction){    //Check if robot is facing a wall or obstacle
        case LEFT:
            check= !tiles[i-1][j].wall && !tiles[i-1][j].obstacle;
            if (!check){
                updateHealth(&tiles[i-1][j]);
            }
            break;
        case DOWN:
            check= !tiles[i][j+1].wall && !tiles[i][j+1].obstacle;
            if (!check){
                updateHealth(&tiles[i][j+1]);
            }
            break;
        case RIGHT:
            check= !tiles[i+1][j].wall && !tiles[i+1][j].obstacle;
            if (!check){
                updateHealth(&tiles[i+1][j]);
            }
            break;
        case UP:
            check= !tiles[i][j-1].wall && !tiles[i][j-1].obstacle;
            if (!check){
                updateHealth(&tiles[i][j-1]);
            }
            break;
    }

    return check;
}


int markerCount(ROBOT* robot){
    return robot-> markerCollected;
}

int atMarker(TILE* tile){
    return tile->marker;
}

int atCorner(TILE* tile){
    return tile->corner;
}

int atHome(TILE* tile){
    return tile->home;
}

void depositMarker(ROBOT* robot, TILE* tile){
    robot->markerDepositted+= robot->markerCollected;

    for ((robot-> markerCollected); robot->markerCollected; robot->markerCollected--) {   //Display Money picture for each marker deposited

        foreground();

        sleep(250);
        displayImage("Money.jpg", tile->xLeft, tile->yTop - TILE_SIZE);  
        sleep(250);
        clear();
    }
    robot->markerCollected = 0;
}

void pickUpMarker(ROBOT* robot, TILE* tile){
    robot->markerCollected++;
    tile->marker = false;

    background();

    setColour(white);
    fillRect(tile->xLeft, tile->yTop, TILE_SIZE, TILE_SIZE);
    setColour(black);
    drawRect(tile->xLeft, tile->yTop, TILE_SIZE, TILE_SIZE);   //Updates the grid when a marker is collected, so appears as if Robot picked up the marker

    foreground(); 
}

void dropMarker(ROBOT* robot, TILE* tile){
    if ( !(markerCount(robot)) || tile-> home) {
        return;
    }
    robot->markerDropped++;
    robot->markerCollected--;
    tile->marker = true;

    background();

    setColour(yellow);
    fillOval(tile->xLeft, tile->yTop, TILE_SIZE, TILE_SIZE);
    setColour(black);
    drawOval(tile->xLeft, tile->yTop, TILE_SIZE, TILE_SIZE);  //Updates the grid when a marker is dropped, so appears as if Robot dropped the marker

    foreground();
}

void forward(ROBOT* robot, TILE** tiles, TILE* tileCurrent){
    int i = tileCurrent->i;      //Gets location of current tile in context of the grid
    int j = tileCurrent->j;

    int x[3];
    int y[3];

    TILE* tile;

    switch (robot-> direction){       //Checks which way robot faces, and updates the x and y array with corresponding values
        case LEFT:
            tile = &tiles[i-1][j];
            x[0] = (tile)->xLeft; x[1] = (tile)->xRight; x[2] = (tile)->xRight;
            y[0] = (tile)->yLeft; y[1] = (tile)->yTop; y[2] = (tile)->yBottom;
            break;
        case DOWN:
            tile = &tiles[i][j+1];
            x[0] = (tile)->xBottom; x[1] = (tile)->xRight; x[2] = (tile)->xLeft;
            y[0] = (tile)->yBottom; y[1] = (tile)->yTop; y[2] = (tile)->yTop;
            break;
        case RIGHT:
            tile = &tiles[i+1][j];
            x[0] = (tile)->xRight; x[1] = (tile)->xLeft; x[2] = (tile)->xLeft;
            y[0] = (tile)->yRight; y[1] = (tile)->yBottom; y[2] = (tile)->yTop;
            break;
        case UP:
            tile = &tiles[i][j-1];
            x[0] = (tile)->xTop; x[1] = (tile)->xRight; x[2] = (tile)->xLeft;
            y[0] = (tile)->yTop; y[1] = (tile)->yBottom; y[2] = (tile)->yBottom;
            break;
    }
        

    update(robot, x,y);
}

void moveRobot(ROBOT* robot, TILE** tiles) {
    foreground();

    setRobot(robot);    //Initialises the robot
    int count = 0;

    TILE* tile = returnHomeTile(tiles);    //Gets location of home tile
    int i = tile->i;
    int j = tile->j;

    int count_markers = tile->markerCount;     //Gets number of markers in the Grid.

    int x[3] = {tile->xRight, tile->xLeft, tile->xLeft};
    int y[3] = {tile->yRight, tile->yTop, tile->yBottom};

    update(robot, x, y);       //Sets the robot at the home tile

    while((robot->markerDepositted < count_markers) || !(atHome(tile))) {
        
        if (atCorner(tile)) {
            depositMarker(robot, tile);
        }


        if (atMarker(tile)) {
            pickUpMarker(robot, tile);
        }
        
        
        if (!(rand()%20)){          //For each step there is 10% chance that the robot changes direction
            left(robot, tile);
        }
        else if (!(rand()%20)){
            right(robot, tile);
            dropMarker(robot, tile);   //For each step the robot has 5% chance to drop a marker
        }
        

        if (canMoveForward(robot, tiles, tile)) {
            forward(robot, tiles, tile);
            tile = updateTile(robot, tiles, tile);
        } else {
            
            if (rand() % 2 || count) {      //Randomly chooses to turn left or right when it hits a blocker
                right(robot, tile);
                count++;     // Ensures the robot doesnt turn left and then right and vice versa
            
            } else {
                left(robot, tile);
                count=0;
            }
        }}


}
void freeRobot(ROBOT* robot){
    free(robot);  //clears memory allocated for robot in the heap
}

