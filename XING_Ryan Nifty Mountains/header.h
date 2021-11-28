#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#define NUMROWS 480
#define NUMCOLS 844
#define GREEN   al_map_rgb(66, 178, 1)
#define RED   al_map_rgb(255, 0, 0)
#define BLUE al_map_rgb(0, 0, 255)
#define WHITE al_map_rgb(255, 255, 255)

#include <queue>
#include <vector>
#include <apmatrix.h>
#include <apvector.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

using namespace std;
typedef pair<int, pair<int, int>> vertex;

int initializeAllegro(int width, int height, const char title[], ALLEGRO_DISPLAY *&display, ALLEGRO_FONT *&arial);

short findMax(apmatrix<short> &grid);
short findMin(apmatrix<short> &grid);
void drawMap(apmatrix<short> &grid);
int drawLowestElevPath(apmatrix<short> &grid, int startRow, ALLEGRO_COLOR colour);
int indexOfLowestElevPath(apmatrix<short> grid, ALLEGRO_COLOR colour);

int dpApproach(apmatrix<short> &grid);
int drawBestPath(int vals[NUMROWS][NUMCOLS]);

int dijkstraImplementation(apmatrix<short> &grid);
int returnShortestPath(int distArr[NUMROWS][NUMCOLS]);
void drawDijkstraPath(int pRow[NUMROWS][NUMCOLS], int pCol[NUMROWS][NUMCOLS], int row, int col);
int updatePath(priority_queue<vertex, vector<vertex>, greater<vertex>> &pq, int &x1, int &y1, int &x2, int &y2, apmatrix<short> &grid, int dist[NUMROWS][NUMCOLS], int parentRow[NUMROWS][NUMCOLS], int parentCol[NUMROWS][NUMCOLS]);

#endif // HEADER_H_INCLUDED
