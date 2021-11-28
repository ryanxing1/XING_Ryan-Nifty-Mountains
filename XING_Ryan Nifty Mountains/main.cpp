 /*****************************************************************************
 *	Name:     Ryan Xing                                                      *
 *	Date:     1/21/2021                                                     *
 *                                                                           *
 *	Description: Displays different possible paths from                      *
 *               one end of a mountain range to another with regards to
                 smallest change in elevation

    Known Issues: Stack overflow issue where error was thrown due to
                  too many large arrays being declared
                  Solution: declare arrays using static keyword
 *                                                                           *
 *****************************************************************************/

#include <fstream>
#include <iostream>
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

int main(int argc, char *argv[]){

    ALLEGRO_DISPLAY *display;
    ALLEGRO_FONT *arial;
    initializeAllegro(NUMCOLS, NUMROWS, "Temp", display, arial);

    //Checks if file was opened successfully
    ifstream infile("colorado.dat");
    if (!infile.is_open()){
        cout << "File not Opened";
        return -1;
    }

    //Creates matrix and reads/stores elevation data from file
    apmatrix<short> mountains(NUMROWS, NUMCOLS);
    for (int i=0; i<NUMROWS; i++){
        for (int j=0; j<NUMCOLS; j++){
            infile >> mountains[i][j];
        }
    }

    bool exit = false;
    string inputNum;
    drawMap(mountains);

    while (!exit){

        cout << "Enter a number between 1-6." << endl
             << "{1}: View all paths for Greedy Algorithm." << endl
             << "{2}: View optimal three move solution." << endl
             << "{3}: View Dijkstra's implementation (best possible path allowing moves in all directions)." << endl
             << "{4}: View all solutions." << endl
             << "{5}: Clear map." << endl
             << "{6}: Exit." << endl;

        cin >> inputNum;
        cout << endl;

        //Series of if statements checking for user input, draws corresponding maps
        if (inputNum.compare("1") == 0){
            int bestPathIndex = indexOfLowestElevPath(mountains, RED);
            cout << "Smallest change in elevation for greedy method is: " << drawLowestElevPath(mountains, bestPathIndex, BLUE) << endl
                 << "Starting at row: " << bestPathIndex << endl
                 << "Paths are represented by colour red, blue represents best path." << endl;
        }
        else if (inputNum.compare("2") == 0){
            cout << "Optimal three move solution results in elevation change of: " << dpApproach(mountains) << endl
                 << "Path is represented by colour green." << endl;
        }
        else if (inputNum.compare("3") == 0){
            cout << "Dijkstra's implementation results in elevation change of: " << dijkstraImplementation(mountains) << endl
                 << "Path is represented by colour white." << endl;
        }
        else if (inputNum.compare("4") == 0){
            cout << "Drawing all solutions. " << endl;
            int bestPathIndex = indexOfLowestElevPath(mountains, RED);
            drawLowestElevPath(mountains, bestPathIndex, BLUE);
            dpApproach(mountains);
            dijkstraImplementation(mountains);
        }
        else if (inputNum.compare("5") == 0){
            drawMap(mountains);
        }
        else if (inputNum.compare("6") == 0){
            cout << "Exiting.";
            exit = true;
        }
        else{
            cout << "Please enter valid input." << endl;
        }
        cout << endl;
        al_flip_display();

    }
    return 0;
}
