#include "background.h"
#include "DrawApp\graphics.h"
#include <stdlib.h>
#include <time.h>

int ROW;
int COLUMN;
int count_markers;
int count_obstacles;
int tile_start = 50;
int gridSize;
int sixthGrid;

TILE** tileMemory() {     //Creates a 2d array to store the tile properties
    
    ROW = (rand() % 6) + 12;
    COLUMN = (rand() % 15) + 14;
    gridSize = ROW * COLUMN;
    sixthGrid = gridSize / 6;


    TILE** tiles = (TILE**)malloc(COLUMN * sizeof(TILE*));
    if (tiles == NULL) {
        return NULL; 
    }

    for (int i = 0; i < COLUMN; i++) {
        tiles[i] = (TILE*)malloc(ROW * sizeof(TILE));
        if (tiles[i] == NULL) {
            for (int k = 0; k < i; k++) {
                free(tiles[k]);
            }
            free(tiles);
            return NULL;
        }
    }
    return tiles;
}

void setWall(TILE** tiles) {          //Defaults all outer tiles to be walls
    for (int i = 0; i < COLUMN; i++) {
        for (int j = 0; j < ROW; j++) {
            if (i == 0 || j == 0 || i == (COLUMN - 1) || j == (ROW - 1)) {
                tiles[i][j].wall = true;
                tiles[i][j].normal = false;
                tiles[i][j].blocker = true;
            }else{
                tiles[i][j].wall = false;
                tiles[i][j].normal = true;
            }
        }
    }
}

void setExtraWall(TILE** tiles) {
    
    int option = rand() % 5;
    int quaterGrid = gridSize / 4;
    int potential_extraWall = (rand() % quaterGrid) + (sixthGrid * 2); //Sets number of extra walls based on Grid Size
    int i = 0;
    
    for (i; i < potential_extraWall; i++) {
        int x = rand() % COLUMN;
        int y = rand() % ROW;
        
        bool aroundWall = (x > 0 && y > 0 && x < COLUMN-1 && y < ROW-1) &&       //Ensures added wall is connected to another wall
                          (tiles[x-1][y].wall || tiles[x+1][y].wall || 
                           tiles[x][y-1].wall || tiles[x][y+1].wall);

        if (tiles[x][y].normal) {
            if (aroundWall) {
                tiles[x][y].wall = true;
                tiles[x][y].normal = false;
                tiles[x][y].blocker = true;

            }
        }
    }
}

bool checkSurrounded(TILE** tiles, TILE* tile) {
    int x = tile->i;
    int y = tile->j;

    // Avoids checking tiles on grid boundaries to prevent out of bounds errors
    if (x == 0 || y == 0 || x == COLUMN - 1 || y == ROW - 1) {
        return 0;  
    }

    // Checks if the tile is surrounded by blockers on all four sides
    bool isSurrounded = (tiles[x - 1][y].blocker && tiles[x + 1][y].blocker && tiles[x][y - 1].blocker && tiles[x][y + 1].blocker);
    
    return isSurrounded;
}

void setObstacles(TILE** tiles) {
    int count = 0;
    int twelfthGrid = gridSize / 12;

    count_obstacles = (rand() % twelfthGrid) + twelfthGrid; //Sets number of obstacles based on Grid Size
    while (count < count_obstacles) {
        int x = rand() % COLUMN;
        int y = rand() % ROW;
        if (tiles[x][y].normal) {
            tiles[x][y].obstacle = true;
            tiles[x][y].normal = false;
            tiles[x][y].health = HEALTH;
            tiles[x][y].blocker = true;
            count++;
        }
    }
}

void setMarkers(TILE** tiles) {
    int count = 0;
    int twelfthGrid = gridSize / 12;
    int twentyforthGrid = gridSize / 24;
    

    count_markers = (rand() % twelfthGrid) + twentyforthGrid; //Sets number of markers based on Grid Size
    while (count < count_markers) {
        int x = rand() % COLUMN;
        int y = rand() % ROW;

        bool surrounded = checkSurrounded(tiles, &tiles[x][y]);

        if (tiles[x][y].normal && !surrounded) {    //Only places marker if the tile is not surrounded by blockers
            tiles[x][y].marker = true;
            tiles[x][y].normal = false;
            count++;
        }
    }
}

void setTileLocation(TILE** tiles) {
    for (int i = 0; i < COLUMN; i++) {
        for (int j = 0; j < ROW; j++) {
            tiles[i][j].i = i;
            tiles[i][j].j = j;
        }
    }
}

void updateHealth(TILE* tile) {            //Updates health and properties of obstacle tiles
    int quaterHealth = HEALTH / 4;

    background();
    setColour(white);
    
    if (tile-> health == HEALTH){
        drawLine(tile->xLeft, tile->yTop, tile->xRight, tile->yBottom);
    } 
    else if (tile-> health == HEALTH - quaterHealth){
        drawLine(tile->xRight, tile->yTop, tile->xLeft, tile->yBottom);
    } 
    else if (tile-> health == HEALTH - (quaterHealth * 2)){
        drawLine(tile->xTop, tile->yTop, tile->xBottom, tile->yBottom);
    } 
    else if (tile-> health == HEALTH - (quaterHealth * 3)){
        drawLine(tile-> xLeft, tile-> yLeft, tile-> xRight, tile-> yRight);
    } 
    else if (tile-> health == 0){
        fillRect(tile->xLeft, tile->yTop, TILE_SIZE, TILE_SIZE);
        setColour(black);
        drawRect(tile->xLeft, tile->yTop, TILE_SIZE, TILE_SIZE);
        tile -> normal = true;
        tile -> obstacle = false;
    }
    (tile-> health)--;
    foreground();
}

void setCorners(TILE** tiles) {
    int cornerCount= 4;

    if (tiles[1][1].normal && !checkSurrounded(tiles, &tiles[1][1])) {            //Tries to set it at initial corners
        tiles[1][1].corner = true;
        tiles[1][1].normal = false;
        cornerCount--;
    }
    if (tiles[1][ROW-2].normal && !checkSurrounded(tiles, &tiles[1][ROW-2])) {
        tiles[1][ROW-2].corner = true;
        tiles[1][ROW-2].normal = false;
        cornerCount--;
    }
    if (tiles[COLUMN-2][1].normal && !checkSurrounded(tiles, &tiles[COLUMN-2][1])) {
        tiles[COLUMN-2][1].corner = true;
        tiles[COLUMN-2][1].normal = false;
        cornerCount--;
    }
    if (tiles[COLUMN-2][ROW-2].normal && !checkSurrounded(tiles, &tiles[COLUMN-2][ROW-2])) {
        tiles[COLUMN-2][ROW-2].corner = true;
        tiles[COLUMN-2][ROW-2].normal = false;
        cornerCount--;
    }

    setExtraTile(tiles, cornerCount); //Creates a new corners if initial corners are blocked
}

void setExtraTile(TILE** tiles , int count) {
    while (count){              //Creates a new corners if initial corners are blocked
        int x = rand() % COLUMN;
        int y = rand() % ROW;

        bool isCorner= (x == 0 || y == 0 || x == COLUMN - 1 || y == ROW - 1) ||  //Checks if the tile is corner
                        (tiles[x-1][y].wall && tiles[x][y+1].wall) ||
                        (tiles[x+1][y].wall && tiles[x][y-1].wall) ||
                        (tiles[x-1][y].wall && tiles[x][y-1].wall) ||
                        (tiles[x+1][y].wall && tiles[x][y+1].wall);
                       
        if (tiles[x][y].normal && isCorner && !checkSurrounded(tiles, &tiles[x][y])) {
            tiles[x][y].corner = true;
            tiles[x][y].normal = false;
            count--;
        }
    }
}

void setHome(TILE** tiles) {
    while(1){
        int x = rand() % COLUMN;
        int y = rand() % ROW;
        if (tiles[x][y].normal) {
            tiles[x][y].home = true;
            tiles[x][y].normal = false;
            tiles[x][y].markerCount = count_markers;
            break;
        }
    }
}

void createTiles(TILE** tiles) {

    setTileLocation(tiles);
    setWall(tiles);
    setExtraWall(tiles);                
    setObstacles(tiles);
    setCorners(tiles);
    setMarkers(tiles);
    setHome(tiles);

}


void decideColour(TILE* tile) {
    if (tile->wall) {
        setRGBColour(0, 100, 0); //Set colour to dark green 
    } else if (tile->corner) {
        setColour(green);
    } else if (tile->marker) {
        setColour(yellow);
    } else if (tile->obstacle) {
        setRGBColour(139, 69, 19); //Set colour to brown
    } else if (tile->home) {  
        setColour(magenta);
    } else {
        setColour(white);
    }
}
    

void drawGrid(TILE** tiles) {
    background();
    for (int i = 0; i < COLUMN; i++) {
        for (int j = 0; j < ROW; j++) {
            int x = i * TILE_SIZE + tile_start; 
            int y = j * TILE_SIZE + tile_start;  

            decideColour(&tiles[i][j]);

            if (tiles[i][j].normal || tiles[i][j].marker || tiles[i][j].home || tiles[i][j].corner || tiles[i][j].obstacle) { 
                tiles[i][j].xTop = (x + HALF_TILE);
                tiles[i][j].yTop = (y);

                tiles[i][j].xBottom = (x + HALF_TILE);
                tiles[i][j].yBottom = (y + TILE_SIZE);

                tiles[i][j].xLeft = (x);
                tiles[i][j].yLeft = (y + HALF_TILE);

                tiles[i][j].xRight = (x + TILE_SIZE);
                tiles[i][j].yRight = (y + HALF_TILE);      //Only records values of tiles that can be stepped on at some point

            }

            if (tiles[i][j].marker) {
                fillOval(x, y, TILE_SIZE, TILE_SIZE);
                setColour(black);                     
                drawOval(x, y, TILE_SIZE, TILE_SIZE);
            } else {
                fillRect(x, y, TILE_SIZE, TILE_SIZE);
            }if (!tiles[i][j].wall){    //Design purpouses the wall is illusioned to be connected and not seperated by tils
                    setColour(black);
                    drawRect(x, y, TILE_SIZE, TILE_SIZE);    
                }
            
        }
    }
}

TILE* returnHomeTile(TILE** tiles) {  
    for (int i = 0; i < COLUMN; i++) {
        for (int j = 0; j < ROW; j++) {
            if (tiles[i][j].home) {
                return &tiles[i][j];
            }     //searches and returns a pointer to the home tile
        }
    }
    return NULL;
}

void setBackground(TILE** tiles) {
    setWindowSize(WIDTH, HEIGHT);
    drawGrid(tiles);
} 


void freeTiles(TILE** tiles) {
    if (tiles == NULL) return;

    for (int i = 0; i < COLUMN; i++) {
        free(tiles[i]);
    }
    free(tiles);
}