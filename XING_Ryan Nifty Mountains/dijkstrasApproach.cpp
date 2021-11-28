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
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

using namespace std;

typedef pair<int, pair<int, int>> vertex;

//Implentation of Dijkstra's shortest path algorithm on a grid
int dijkstraImplementation(apmatrix<short> &grid){

    //Static arrays declared due to stack overflow issue (too much memory used from stack)
    static int dist[NUMROWS][NUMCOLS];
    static int parentRow[NUMROWS][NUMCOLS];
    static int parentCol[NUMROWS][NUMCOLS];

    //Initializing distance and parent arrays to 0
    for (int i=0; i<NUMROWS; i++){
        for (int j=0; j<NUMCOLS; j++){
            if (j == 0){
                dist[i][j] = 0;
            }
            else {
                dist[i][j] = INT_MAX;
            }
            parentRow[i][j] = 0;
            parentCol[i][j] = 0;
        }
    }

    //Use of priority queue, vertex type stores current weight of vertex, along with its coordinates
    priority_queue<vertex, vector<vertex>, greater<vertex>> pq;

    //Pushes all starting positions of the first column to the queue
    for (int i=0; i<NUMROWS; i++){
        pq.push(make_pair(0, make_pair(i,0)));
    }

    while (pq.size() != 0){

        //Takes vertex with the minimum vertex weight, stores its coordinates as x1, y1
        int x1 = pq.top().second.first; int y1 = pq.top().second.second;
        pq.pop();

        //All 8 different possible moves, checks if moves are within bound, if so, possibly relax the current nodes neighbours

        //Right
        if (y1 < NUMCOLS - 1){
            int x2 = x1; int y2 = y1 + 1;
            updatePath(pq, x1, y1, x2, y2, grid, dist, parentRow, parentCol);
        }

        //Up-Right
        if (x1 > 0 && y1 < NUMCOLS - 1){
            int x2 = x1 - 1; int y2 = y1 + 1;
            updatePath(pq, x1, y1, x2, y2, grid, dist, parentRow, parentCol);
        }

        //Down-Right
        if (x1 < NUMROWS - 1 && y1 < NUMCOLS - 1){
            int x2 = x1 + 1; int y2 = y1 + 1;
            updatePath(pq, x1, y1, x2, y2, grid, dist, parentRow, parentCol);

        }
        //Up
        if (x1 > 0){
            int x2 = x1 - 1; int y2 = y1;
            updatePath(pq, x1, y1, x2, y2, grid, dist, parentRow, parentCol);
        }
        //Down
        if (x1 < NUMROWS - 1){
            int x2 = x1 + 1; int y2 = y1;
            updatePath(pq, x1, y1, x2, y2, grid, dist, parentRow, parentCol);
        }

        //Left
        if (y1 > 0){
            int x2 = x1; int y2 = y1 - 1;
            updatePath(pq, x1, y1, x2, y2, grid, dist, parentRow, parentCol);
        }

        //Up-Left
        if (x1 > 0 && y1 > 0){
            int x2 = x1 - 1; int y2 = y1 - 1;
            updatePath(pq, x1, y1, x2, y2, grid, dist, parentRow, parentCol);
        }

        //Down-Left
        if (x1 < NUMROWS - 1 && y1 > 0){
            int x2 = x1 + 1; int y2 = y1 - 1;
            updatePath(pq, x1, y1, x2, y2, grid, dist, parentRow, parentCol);
        }

    }

    int shortestPath = returnShortestPath(dist);
    drawDijkstraPath(parentRow, parentCol, shortestPath, NUMCOLS - 1);
    return dist[shortestPath][NUMCOLS-1];

}

int updatePath(priority_queue<vertex, vector<vertex>, greater<vertex>> &pq, int &x1, int &y1, int &x2, int &y2, apmatrix<short> &grid, int dist[NUMROWS][NUMCOLS], int parentRow[NUMROWS][NUMCOLS], int parentCol[NUMROWS][NUMCOLS]){
    //Calculates the difference in elevation between current node and its neighbour
    int w = abs(grid[x1][y1] - grid[x2][y2]);

    //if the stored elevation of the current vertex plus the change in elevation with its neighbour is smaller than
    //the stored value of the neighbours, update the neighbours value
    //Also updates the parent of the neighbour vertex
    if (dist[x2][y2] > dist[x1][y1] + w){
        dist[x2][y2] = dist[x1][y1] + w;
        pq.push(make_pair(dist[x2][y2], make_pair(x2, y2)));
        parentRow[x2][y2] = x1;
        parentCol[x2][y2] = y1;
    }
}

//Finds the smallest change in elevation of the dist array, and returns its index
int returnShortestPath(int distArr[NUMROWS][NUMCOLS]){
    int shortest = INT_MAX;
    int index;
    for (int i=0; i<NUMROWS; i++){
        if (distArr[i][NUMCOLS-1] < shortest){
            shortest = distArr[i][NUMCOLS-1];
            index = i;
        }
    }
    return index;
}

//Recursive solution to backtrack the parent arrays starting from the optimal row and col determined by the returnShortestPath function
void drawDijkstraPath(int pRow[NUMROWS][NUMCOLS], int pCol[NUMROWS][NUMCOLS], int row, int col) {

    al_draw_filled_rectangle(pCol[row][col], pRow[row][col], pCol[row][col]+1, pRow[row][col]+1, WHITE);

    //Base case if the node arrived at is in the left most column
    if (pCol[row][col] == 0)
        return;

    drawDijkstraPath(pRow, pCol, pRow[row][col], pCol[row][col]);
}



