#include <iostream>
#include <climits>
#include <queue>
#include <functional>
#include <vector>
#include <utility>

#include <fstream>
#include <apmatrix.h>
#include <apvector.h>
#include <string.h>
#include <cstdlib>
#include "header.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

using namespace std;

//Algorithm that uses memoization to store previous smallest changes in elevation from nodes
//Uses stored data to complete future computations

int dpApproach(apmatrix<short> &grid){

    //Creates array that will store smallest changes in elevation, initializes values to 0
    int vals[NUMROWS][NUMCOLS];
    for (int i=0; i<NUMROWS; i++){
        for (int j=0; j<NUMCOLS; j++){
            vals[i][j] = 0;
        }
    }

    // Iteration through cells by column
    for (int i=1; i<NUMCOLS; i++){
        for (int j=0; j<NUMROWS; j++){

            //Checks previous possible parents and gathers their changes in elevation
            int a, b, c;
            a = b = c = INT_MAX;

            a = abs(grid[j][i-1] - grid[j][i]);
            if (j > 0){
                b = abs(grid[j-1][i-1] - grid[j][i]);
            }
            if (j < NUMROWS - 1){
                c = abs(grid[j+1][i-1] - grid[j][i]);
            }

            //Stores the smallest change in elevation between three parents in value array
            if (j == 0){
                vals[j][i] = min(a + vals[j][i-1], c + vals[j+1][i-1]);
            }
            else if (j == NUMROWS - 1){
                vals[j][i] = min(a + vals[j][i-1], b + vals[j-1][i-1]);
            }
            else{
                vals[j][i] = min(min(a + vals[j][i-1], c + vals[j+1][i-1]), b + vals[j-1][i-1]);
            }
        }
    }

    //Uses drawBestPath function to backtrack the value array, drawing the best path;
    int shortestElevationChange = drawBestPath(vals);
    return shortestElevationChange;
}

int drawBestPath(int vals[NUMROWS][NUMCOLS]){

    //Calculates the index of the smallest change in elevation path from value array
    int shortest = INT_MAX;
    int index;
    for (int i=0; i<NUMROWS; i++){
        if (vals[i][NUMCOLS-1] < shortest){
            shortest = vals[i][NUMCOLS-1];
            index = i;
        }
    }

    //Backtracks the value array, always choosing the smallest value of the three possible parents
    int curX = index;
    for (int curY=NUMCOLS; curY>0; curY--){

        int a, b, c;
        a = b = c = INT_MAX;

        a = vals[curX][curY-1];
        if (curX > 0){
            b = vals[curX-1][curY-1];
        }
        if (curX < NUMROWS - 1){
            c = vals[curX+1][curY-1];
        }

        int previous = min(a, min(b,c));

        if (b == previous){
            curX -= 1;
        }
        else if (c == previous){
            curX += 1;
        }

        al_draw_filled_rectangle(curY, curX, curY + 1, curX + 1, GREEN);
    }
    return shortest;
}
