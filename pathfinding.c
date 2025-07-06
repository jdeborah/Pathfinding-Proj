

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*==========================================================*
*                 PREPROCESSOR DIRECTIVES                   *
*==========================================================*/
#define PLACEHOLDER_VALUE ' '
// The following set is for easy map rendering
#define START_SPACE 'S'
#define END_SPACE 'E'
#define BLOCK_SPACE 'X'
#define PATH_SPACE '+'
#define VISITED_SPACE '*'

// The following directives are for map creation
#define EMPTY_SPACE ' '  // ASCII char 32
#define N 5

/*==========================================================*
*                   FUNCTION PROTOTYPES                     *
*==========================================================*/

// Functions for map rendering and maintenance
void ClearMap(char MAP[][N]);
void RefreshMap(char MAP[][N]);
// Functions for printing/output readability
void LevelHeader(int LevelNum);

void Level01(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn);
void Level02(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn);
void Level03(char MAP[][N], int startRow, int startColumn);
void Level04(char MAP[][N], int startRow, int startColumn);


// Render map
void PrintMap(char MAP[][N]);
// Map maintenance
void FillMap(char MAP[][N], int *startRow, int *startColumn, int *endRow,
             int *endColumn);
// Pathfinding algorithms
int SimpleDirections(char MAP[][N], int startRow, int startColumn, int endRow,
                     int endColumn);
void ClosestFreeNeighbour(char MAP[][N], int currentRow, int currentColumn);
int BonusClosestFreeNeighbour(char MAP[][N], int currentRow, int currentColumn);

/*----------------------------------------------------------*
*        PROTOTYPE FOR HELPER FUNCTIONS         *
*----------------------------------------------------------*/
int valid_move(char MAP[][N], int *current_row, int *current_col, int row_step,
               int col_step, int *count_steps, int *endRow, int *endColumn);
int determine_dir(int *current, int *end);


/*==========================================================*/
int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-level")) {
        printf(
            "You must run this program specifying the level to run as an "
            "argument\n");
        exit(EXIT_FAILURE);
    }
    int level = atoi(argv[2]);

    char MAP[N][N];
    int startRow, startColumn, endRow, endColumn;

    ClearMap(MAP);
    FillMap(MAP, &startRow, &startColumn, &endRow, &endColumn);

    if (level == 1) {
        Level01(MAP, startRow, startColumn, endRow, endColumn);
    } else if (level == 2) {
        Level02(MAP, startRow, startColumn, endRow, endColumn);
    } else if (level == 3) {
        Level03(MAP, startRow, startColumn);
    } else if (level == 4) {
        Level04(MAP, startRow, startColumn);
    }

    return 0;
}

/*==========================================================*
*                  FUNCTIONS PER LEVEL                    *
*==========================================================*/
// Level 1
void PrintMap(char MAP[][N]) {
    for (int rows = 0; rows < N; rows++){
        for (int cols = 0; cols < N; cols++){
            printf("[%c]", MAP[rows][cols]);
        }
        printf("\n");
    }
}

// Level 1
void FillMap(char MAP[][N], int *startRow, int *startColumn, int *endRow,
             int *endColumn) {
    int amountBlocks;
    int row_block, col_block;
    scanf("%d %d", startRow, startColumn);
    scanf("%d %d", endRow, endColumn);

    // Label start and end
    MAP[*startRow][*startColumn] = START_SPACE;
    MAP[*endRow][*endColumn] = END_SPACE;

    // Read and label blocks
    scanf("%d", &amountBlocks);
    for (int i = 0; i < amountBlocks; i++){ 
        scanf("%d %d", &row_block, &col_block);
        MAP[row_block][col_block] = BLOCK_SPACE;
    }
}

// Level 2
int SimpleDirections(char MAP[][N], int startRow, int startColumn, int endRow,
                     int endColumn){
    int current_row = startRow;
    int current_col = startColumn;
    int numberSteps = 0;

    // Move vertically
    if (startRow == endRow && startColumn == endColumn){
        numberSteps++;
        return numberSteps;
    }

    while (current_row != endRow){
        int vertical_dir = determine_dir(&current_row, &endRow); // up or down
        // Obstacle found so move right or left
        if (!valid_move(MAP, &current_row, &current_col, 
            vertical_dir, 0, &numberSteps, &endRow, &endColumn)){
            int horizontal_dir = determine_dir(&current_col, &endColumn); // left or right
            
            if (!valid_move(MAP, &current_row, &current_col, 0, 
                horizontal_dir, &numberSteps, &endRow, &endColumn)){
                // When stuck
                if (current_row != endRow && current_col != endColumn){
                    printf("SimpleDirections took %d steps and got stuck.\n\n", numberSteps);
                    return 0;
                }
            }
        }
    }
    // Move horizontally
    while (current_col != endColumn){
        int horizontal_dir = determine_dir(&current_col, &endColumn);
        // Bumped into obstacle, move up or down
        if (!valid_move(MAP, &current_row, &current_col, 
            0, horizontal_dir, &numberSteps, &endRow, &endColumn)){
            int vertical_dir = determine_dir(&current_row, &endRow); // Vertical movement

            if (!valid_move(MAP, &current_row, &current_col, 
                vertical_dir, 0, &numberSteps, &endRow, &endColumn)){
                // When stuck
                if (current_row != endRow && current_col != endColumn){
                    printf("SimpleDirections took %d steps and got stuck.\n\n", numberSteps);
                    return 0;
                }
            }
        }
    }
    if (current_row == endRow && current_col == endColumn){
        numberSteps++;
    }
    return numberSteps;
}

// Level 3
void ClosestFreeNeighbour(char MAP[][N], int currentRow, int currentColumn){
    if (MAP[currentRow][currentColumn] == BLOCK_SPACE || 
        MAP[currentRow][currentColumn] == VISITED_SPACE ||
        MAP[currentRow][currentColumn] == PATH_SPACE){
        return;
    }

    if (MAP[currentRow][currentColumn] == EMPTY_SPACE && 
        MAP[currentRow][currentColumn] != START_SPACE){
        MAP[currentRow][currentColumn] = VISITED_SPACE;
    }

    // Base case
    if (MAP[currentRow][currentColumn] == END_SPACE){
        return;
    }
    if (MAP[currentRow][currentColumn] != START_SPACE && 
        MAP[currentRow][currentColumn] != END_SPACE){
        MAP[currentRow][currentColumn] = PATH_SPACE;
    }

    if (currentRow > 0 && MAP[currentRow - 1][currentColumn] != VISITED_SPACE && 
        (MAP[currentRow - 1][currentColumn] == EMPTY_SPACE ||
        MAP[currentRow - 1][currentColumn] == END_SPACE)){
        // Up
        ClosestFreeNeighbour(MAP, currentRow - 1, currentColumn);
        return;

    } else if (currentColumn < N - 1 && MAP[currentRow][currentColumn + 1] != VISITED_SPACE &&
        (MAP[currentRow][currentColumn + 1] == EMPTY_SPACE ||
        MAP[currentRow][currentColumn + 1] == END_SPACE)){
        // Right
        ClosestFreeNeighbour(MAP, currentRow, currentColumn + 1);
        return;

    } else if (currentRow < N - 1 && MAP[currentRow + 1][currentColumn] != VISITED_SPACE &&
        (MAP[currentRow + 1][currentColumn] == EMPTY_SPACE ||
        MAP[currentRow + 1][currentColumn] == END_SPACE)){
        // Down
        ClosestFreeNeighbour(MAP, currentRow + 1, currentColumn);
        return;
        
    } else if (currentColumn > 0 && MAP[currentRow][currentColumn - 1] != VISITED_SPACE &&
        (MAP[currentRow][currentColumn - 1] == EMPTY_SPACE ||
        MAP[currentRow][currentColumn - 1] == END_SPACE)) {
        // Left
        ClosestFreeNeighbour(MAP, currentRow, currentColumn - 1);
        return;
    }
}
// Level 4
int BonusClosestFreeNeighbour(char MAP[][N], int currentRow, int currentColumn){
    if (currentColumn < 0 || currentColumn >= N || currentRow < 0 || currentRow >= N){
        return 0;
    }

    if (MAP[currentRow][currentColumn] == BLOCK_SPACE ||
        MAP[currentRow][currentColumn] == VISITED_SPACE ||
        MAP[currentRow][currentColumn] == PATH_SPACE){
        return 0;
    }

    if (MAP[currentRow][currentColumn] == EMPTY_SPACE &&
        MAP[currentRow][currentColumn] != START_SPACE && 
        MAP[currentRow][currentColumn] != END_SPACE){
        MAP[currentRow][currentColumn] = VISITED_SPACE;
    }

    // Base case
    if (MAP[currentRow][currentColumn] == END_SPACE){
        return 1;
    }
    if (MAP[currentRow][currentColumn] != START_SPACE &&
        MAP[currentRow][currentColumn] != END_SPACE){
        MAP[currentRow][currentColumn] = PATH_SPACE;
    }

    if (BonusClosestFreeNeighbour(MAP, currentRow - 1, currentColumn) ||
        BonusClosestFreeNeighbour(MAP, currentRow, currentColumn + 1) ||
        BonusClosestFreeNeighbour(MAP, currentRow + 1, currentColumn) ||
        BonusClosestFreeNeighbour(MAP, currentRow, currentColumn - 1)){
        return 1;
    }
    if (MAP[currentRow][currentColumn] != START_SPACE &&
        MAP[currentRow][currentColumn] != END_SPACE){
        MAP[currentRow][currentColumn] = VISITED_SPACE;
    }
    return 0;
}

/*----------------------------------------------------------*
*        SPACE FOR HELPER FUNCTIONS         *
*----------------------------------------------------------*/
// Function to check if we can validly move to a particular position
int valid_move(char MAP[][N], int *current_row, int *current_col, int row_step,
                int col_step,int *count_steps, int *endRow, int *endColumn){
    int col_next = *current_col + col_step;
    int row_next = *current_row + row_step;

    if (*current_row == *endRow && *current_col == *endColumn){
        return 0;
    }

    if (MAP[row_next][col_next] != BLOCK_SPACE){
        *current_col = col_next;
        *current_row = row_next;
        if (MAP[row_next][col_next] != START_SPACE && MAP[row_next][col_next] != END_SPACE){
            // can move
            MAP[row_next][col_next] = PATH_SPACE;
            (*count_steps)++;
            return 1;
        }
    }
    return 0;
}

// Function to determine if we are moving in the positive or negative direction
int determine_dir(int *current, int *end){
    if (*current < *end){
        return 1;
    } else{
        return -1;
    }
}

/*==========================================================*
*                    FUNCTIONS                     *
*==========================================================*/
// Function to generate header
void LevelHeader(int LevelNum) {
    printf("=================================\n");
    printf("Level %d:\n", LevelNum);
    printf("=================================\n");
}

// Function to execute level 1: print and fill map
void Level01(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn) {
    LevelHeader(1);
    PrintMap(MAP);
    printf("The starting position is at MAP[%d][%d]\n", startRow, startColumn);
    printf("The ending position is at MAP[%d][%d]\n", endRow, endColumn);
}

// Function to execute level 2: SimpleDirections process
void Level02(char MAP[][N], int startRow, int startColumn, int endRow,
             int endColumn) {
    LevelHeader(2);
    int steps = SimpleDirections(MAP, startRow, startColumn, endRow, endColumn);
    if (steps != 0){
        printf("SimpleDirections took %d steps to find the goal.\n\n", steps);
    }
    PrintMap(MAP);
}

// Function to execute level 3: ClosestFreeNeighbour process
void Level03(char MAP[][N], int startRow, int startColumn) {
    LevelHeader(3);
    RefreshMap(MAP);
    ClosestFreeNeighbour(MAP, startRow, startColumn);
    PrintMap(MAP);
}

// Function to execute level 4: Bonus Challenge to improve ClosestFreeNeighbour
void Level04(char MAP[][N], int startRow, int startColumn) {
    LevelHeader(4);
    RefreshMap(MAP);
    if (!BonusClosestFreeNeighbour(MAP, startRow, startColumn)){
        printf("No path found\n");
    }
    PrintMap(MAP);
}

// Function to clear map
void ClearMap(char MAP[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            MAP[i][j] = EMPTY_SPACE;
        }
    }
}

/*Function to make printing out a "clean" map easier*/
void RefreshMap(char MAP[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char currentNode = MAP[i][j];
            if (!(currentNode == START_SPACE || currentNode == END_SPACE ||
                  currentNode == BLOCK_SPACE)) {
                MAP[i][j] = EMPTY_SPACE;
            }
        }
    }
}
