//Noah Redden
//SwinesMeeper

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
enum State{gameStart, gameActive, gameOver, gameWin, quit};
State state;

//sets up grid, returns 2D int array
int ** initGrid(int sizeX, int sizeY);

//fills board grid with mines
//makes sure that (X, Y) does not have a mine
void fillBoard(int X, int Y, Board board);

//checks surrounding spaces for mines
int checkForMines(int X, int Y,  Board board);

//reveals how many adjacent mines are on a space by changing the grid
void revealSpace(int X, int Y, Board board);

//checks if there are no more unchecked spaces, for ending the game
bool checkForUnchecked(Board board);

//checks input for coordinates
int * inputCoordinates();

//does game actions using input
void runGameMechanics(Board board);

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
    board.minesLeft = board.mines;
    board.grid = initGrid(board.sizeX, board.sizeY);

    printBoard(board);
    
    runGameMechanics(board);
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

void fillBoard(int X, int Y, Board board){
    
    //coordinates for mines
    int minesX;
    int minesY;
    
    //randomly create mine coordinates, place mines in position.
    for(int i = 0; i<board.mines; i++){
        
        int minesX = UNCHECKED;
        int minesY = UNCHECKED;
        
        //assigns random coordinates to mines. tries again if coordinates already have a mine.
        //also tries again if random coordinates are (X, Y). done so first move of the game at (X, Y) is not a loss.
        while((minesX == UNCHECKED || minesY == UNCHECKED) || board.grid[minesX][minesY] == MINE || (minesX == X && minesY == Y)){
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
    
    
}

bool checkForUnchecked(Board board){
    //should be true if there are no unchecked tiles encountered
    bool result = true;
    for(int i = 0; i < board.sizeY && result; i++)
        for(int j = 0; j < board.sizeX && result; j++){
            if(board.grid[j][i]==-1){
                result=false;
            }
        }
    return result;
}

int * inputCoordinates(){
    int coords[2];

    return coords;
}

void runGameMechanics(Board board){
    state = gameStart;
    string input;
    int X = UNCHECKED;
    int Y = UNCHECKED;
    
    cout << "Please give coordinates separated by a space (Ex. 1 2).\nYou may quit by typing 'quit'\n";
    
    cin >> input;
    if(input==QUIT)state = quit;
    while(state!=quit){
        if(state==gameOver){
            cout << "Game Over! You stepped on a mine\n";
            //TODO: ask to play again
            state = quit;
        }else if (state==gameWin){
            cout << "Winner! \n";
            state = quit;
        }else if(state==gameActive||state==gameStart){
            //TODO: Check for numerical inputs
            if(X==UNCHECKED && Y==UNCHECKED){
                //get coordinates from input
                //TODO: check that they are in bounds
                X = stoi(input);
                cin >> input;
                Y = stoi(input);

                if(state==gameStart){
                    fillBoard(X, Y, board);
                    state = gameActive;
                }
                
                revealSpace(X, Y, board);
                
                if(checkForUnchecked(board))
                    state = gameWin;
                   
                //check if x and y are a Mine
                if(board.grid[X][Y]==MINE)
                    state = gameOver;
                    
                printBoard(board);
                
                X = UNCHECKED;
                Y = UNCHECKED;
                
                if(state==gameActive){
                    cout << "What are your next coordinates?\n";
                    cin >> input;
                }
            } 
        }
        if(input==QUIT)state = quit;
    }
    cout << "Quitting program...\n";
}

void printBoard(Board board){
    cout << "Total Mines: " << board.mines << '\n';
    //print out legend
    cout << "-*-";
    for(int j = 0; j < board.sizeX; j++)
        cout << "|" << j << "|";
    cout << '\n';
    //prints board with Y as vertical, X as horizontal
    for(int i = 0; i < board.sizeY; i++){
        cout << "=" << i << "=";
        for(int j = 0; j < board.sizeX; j++){
            if(board.grid[j][i]==MINE)
                if(state!=gameActive)
                    cout<<"[!]";
                else
                    cout<<"[~]";
            else if(board.grid[j][i]==UNCHECKED)
                cout<<"[~]";
            else if(board.grid[j][i]==0)
                cout<<"[ ]";
            else
                cout<<"["<<board.grid[j][i]<<"]";
        }
        cout<<'\n';
    }
}