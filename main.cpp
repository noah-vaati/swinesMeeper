/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>

using namespace std;

//locations on grid that are unchecked or contain a mine are indicated by the following:
#define UNCHECKED -1
#define MINE -2
#define QUIT "quit"

//game board. contains grid to be filled with mines.
struct Board{
    int sizeX;
    int sizeY;
    int mines;
    int minesLeft;
    int **grid;
};

//game states
enum State{gameActive, gameOver, quit};

//sets up grid, returns 2D int array
int ** initGrid(int sizeX, int sizeY);

//fills board grid with mines
void fillBoard(Board board);

//checks surrounding spaces for mines
int checkForMines(int X, int Y,  Board board);

//reveals how many adjacent mines are on a space by changing the grid
void revealSpace(int X, int Y, Board board);

//checks input, acts accordingly
void checkInput(Board board);

//prints out grid
void printBoard(Board board);

int main()
{
    //initialize random number generator using time as seed
    //srand(time(NULL));
    
    //TODO: Command Line arguments for game parameters
    Board board;
    board.sizeX = 9;
    board.sizeY = 9;
    board.mines = 10;
    board.grid = initGrid(board.sizeX, board.sizeY);
    
    fillBoard(board);

    printBoard(board);
    
    checkInput(board);
}


int ** initGrid(int sizeX, int sizeY){
    int ** grid;
    grid = new int *[sizeX];
    for(int i = 0; i <sizeX; i++){
        grid[i] = new int[sizeY];
        for(int j = 0; j < sizeY; j++)
            grid[i][j] = UNCHECKED;
    }
    
    return grid;
}

void fillBoard(Board board){
    //TODO: Make it so first move does not result in Game Over. May need to fill board on first move.
    //coordinates for mines
    int minesX;
    int minesY;
    
    //randomly create mine coordinates, place mines in position.
    for(int i = 0; i<board.mines; i++){
        
        int minesX = UNCHECKED;
        int minesY = UNCHECKED;
        
        //assigns random coordinates to mines. tries again if coordinates already have a mine.
        while((minesX == UNCHECKED || minesY == UNCHECKED) || board.grid[minesX][minesY] == MINE){
            minesX = rand() % board.sizeX;
            minesY = rand() % board.sizeY;
        }
        
        board.grid[minesX][minesY] = MINE;
    }
    
    
}

int checkForMines(int X, int Y, Board board){
    //how many mines?
    int count = 0;
    
    //if selected space is a mine, then do not check surrounding spaces and instead return MINE (-2)
    if(board.grid[X][Y]==MINE){
        return MINE;
    }
    
    for(int i = X-1; i <= X+1; i++){
        if(i >= 0 && i < board.sizeX)
            for(int j = Y-1; j <= Y+1; j++){
                if(j >= 0 && j < board.sizeY)
                    //check if space has a mine
                    if(board.grid[i][j]==MINE)
                        count++;
            }
    }
    return count;
}

void revealSpace(int X, int Y, Board board){
    board.grid[X][Y] = checkForMines(X, Y, board);
    cout << "Reveal : " << X << ", " << Y << "\n";
    //if space has 0 mines adj., reveal mines in adjacent spaces
    if(board.grid[X][Y] == 0){
        for(int i = X-1; i <= X+1; i++){
            if(i >= 0 && i < board.sizeX)
                for(int j = Y-1; j <= Y+1; j++){
                    if(j >= 0 && j < board.sizeY)
                        if((i!=X || j!=Y) && board.grid[i][j] == UNCHECKED)
                            revealSpace(i, j, board);
                }
        }
    }
    
    printBoard(board);
}

void checkInput(Board board){
    State state = gameActive;
    string input;
    int X = UNCHECKED;
    int Y = UNCHECKED;
    
    cout << "Please give coordinates separated by a space (Ex. 1 2).\nYou may quit by typing 'quit'\n";
    
    cin >> input;
    if(input==QUIT)state = quit;
    while(state!=quit){
        if(state==gameOver){
            //ask to play again
            state = quit;
        }else if(state==gameActive){
            //TODO: Check for numerical inputs
            if(X==UNCHECKED && Y==UNCHECKED){
                //get coordinates from input
                //TODO: check that they are in bounds
                X = stoi(input);
                cin >> input;
                Y = stoi(input);
                
                revealSpace(X, Y, board);
                
                X = UNCHECKED;
                Y = UNCHECKED;
                
                cout << "What are your next coordinates?\n";
                cin >> input;
            } 
        }
        if(input==QUIT)state = quit;
    }
    cout << "Quitting program...\n";
}

void printBoard(Board board){
    cout << "SizeX: " << board.sizeX << ", SizeY: " << board.sizeY << '\n';
    //prints board with Y as vertical, X as horizontal
    for(int i = 0; i < board.sizeY; i++){
        for(int j = 0; j < board.sizeX; j++){
            if(board.grid[j][i]==MINE)
                cout<<"[!]";
            else if(board.grid[j][i]==UNCHECKED)
                cout<<"[*]";
            else if(board.grid[j][i]==0)
                cout<<"[ ]";
            else
                cout<<"["<<board.grid[j][i]<<"]";
        }
        cout<<'\n';
    }
}
