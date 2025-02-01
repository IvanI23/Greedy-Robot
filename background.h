#ifndef TILE_H     //Guards prevent multiple inclusions of background.h
#define TILE_H

#include <time.h>
#include <stdbool.h>

#define TILE_SIZE 50
#define HALF_TILE (TILE_SIZE / 2)
#define WIDTH 1960
#define HEIGHT 1080
#define HEALTH 8

typedef struct tile{
    
    bool wall,
         corner,
         marker,
         obstacle,
         normal,
         blocker,
         home;

    int xTop, yTop,
        xBottom, yBottom,
        xLeft, yLeft,
        xRight, yRight,
        i, j,
        markerCount,
        health;

} TILE;

TILE** tileMemory();
TILE* returnHomeTile(TILE** tiles);

void setTileLocation(TILE** tiles);   //Initialises the tile properties
void setWall(TILE** tiles);
void setMarkers(TILE** tiles);
void setObstacles(TILE** tiles);
void setCorners(TILE** tiles);
void setHome(TILE** tiles);
void createTiles(TILE** tiles);   
bool checkSurrounded(TILE** tiles, TILE* tile);   
void setExtraWall(TILE** tiles);
void setExtraTile(TILE** tiles , int count);

void decideColour(TILE* tile);    //Draws Grid
void drawGrid(TILE** tiles);
void setBackground(TILE** tiles);
void updateHealth(TILE* tiles);

void freeTiles(TILE** tiles);

#endif 