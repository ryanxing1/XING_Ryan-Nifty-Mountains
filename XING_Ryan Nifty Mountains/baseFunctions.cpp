#include <fstream>
#include <iostream>
#include <apmatrix.h>
#include <apvector.h>
#include <string.h>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "header.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

using namespace std;

//Finds max value of a given apmatrix
short findMax(apmatrix<short> &grid){
    short maxVal = 0;
    for (int i=0; i<NUMROWS; i++){
        for (int j=0; j<NUMCOLS; j++){
            maxVal = max(maxVal, grid[i][j]);
        }
    }
    return maxVal;
}

//Find min value of given apmatrix
short findMin(apmatrix<short> &grid){
    short minVal = SHRT_MAX;
    for (int i=0; i<NUMROWS; i++){
        for (int j=0; j<NUMCOLS; j++){
            minVal = min(minVal, grid[i][j]);
        }
    }
    return minVal;
}

//Draws elevation map, with each elevation corresponding to the shade of a colour
void drawMap(apmatrix<short> &grid){
    short minVal = findMin(grid);
    short maxVal = findMax(grid);

    int colour1[3] = {0, 25, 0};
    int colour2[3] = {0, 191, 255};
    int colour3[3];

    for (int i=0; i<NUMROWS; i++){
        for (int j=0; j<NUMCOLS; j++){
            float shade = float(grid[i][j] - minVal) / float(maxVal);

            //Interpolating colours
            for (int k=0; k<3; k++){
                colour3[k] = (colour2[k] - colour1[k]) * shade + colour1[k];
            }

            al_draw_filled_rectangle(j, i, j+1, i+1, al_map_rgb(colour3[0], colour3[1], colour3[2]));
        }
    }
}

//Draws path from a given starting point using greedy method
int drawLowestElevPath(apmatrix<short> &grid, int startRow, ALLEGRO_COLOR colour){

    srand(time(0));

    int totalElevationChange = 0;
    int curRow = startRow;

    //Iterates through each column, chooses smallest elevation change between three possible moves
    for (int curCol = 0; curCol < NUMCOLS-1; curCol++){

        al_draw_filled_rectangle(curCol, curRow, curCol+1, curRow+1, colour);

        int right, upRight, downRight;
        right = upRight = downRight = INT_MAX;

        //Calculates the changes in elevation
        right = abs(grid[curRow][curCol] - grid[curRow][curCol+1]);
        if (curRow + 1 < NUMROWS){
            downRight = abs(grid[curRow][curCol] - grid[curRow+1][curCol+1]);
        }
        if (curRow - 1 >= 0){
            upRight = abs(grid[curRow][curCol] - grid[curRow-1][curCol+1]);
        }

        //Checks for optimal greedy move and applies it,
        //Adds value to total
        if (right < downRight && right < upRight){
            totalElevationChange += right;
        }
        else if (downRight < right && downRight < upRight){
            curRow++;
            totalElevationChange += downRight;
        }
        else if (upRight < right && upRight < downRight){
            curRow--;
            totalElevationChange += upRight;
        }

        //Checks for duplicate values and randomly selects a path
        else if (right == upRight){
            totalElevationChange += right;
            if (rand() % 2 == 0){
                curRow--;
            }
        }
        else if (right == downRight){
            totalElevationChange += right;
            if (rand() % 2 == 0){
                curRow++;
            }
        }
        else if (upRight == downRight){
            totalElevationChange += upRight;
            if (rand() % 2 == 0){
                curRow--;
            }
            else{
                curRow++;
            }
        }
        else{
            totalElevationChange += right;
            int random = rand() % 3;
            if (random == 1){
                curRow--;
            }
            else if (random == 2){
                curRow++;
            }
        }
    }

    return totalElevationChange;
}

//Returns the index of the lowest change in elevation path for the greedy method
//Note: function draws all paths from starting rows
int indexOfLowestElevPath(apmatrix<short> grid, ALLEGRO_COLOR colour){
    int index;
    int lowestElev = INT_MAX;

    for (int i=0; i<NUMROWS; i++){
        int curElev = drawLowestElevPath(grid, i, colour);
        if (curElev < lowestElev){
            lowestElev = curElev;
            index = i;
        }
    }
    return index;
}

