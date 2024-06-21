/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions

/* one type definition from my sample solution -------------------------------*/
#define MIN_COL 65
#define MAX_COL MIN_COL + BOARD_SIZE
#define INITIAL 2
#define TO_HICASE -32
#define DATA_TO_ARRAY -1
#define TRUE 1

/* Function prototypes */
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

/* Struct for all possible moves */
typedef struct {
    unsigned char board[BOARD_SIZE][BOARD_SIZE]; // stores possible board
    int node_val; // board cost
    int first_pos; // first position of that branch 
    int last_pos; // last position of that branch
} node;

void initial(board_t setup);
void print_board(board_t board);
void move(board_t board, board_t upd_board, int old_col, int old_row, 
	int new_col, int new_row, int action);
int check_error(board_t board, int action, int old_col, int old_row, 
	int new_col, int new_row);
void board_details(board_t board, char old_col,  int old_row, char new_col,
	int new_row, int action);
int board_cost(board_t upd_board);
int count_pieces(board_t board, char elem);
void print_error(int error);
void board_swap(board_t input, board_t input_upd);
void print_dividers();


void stage_1(board_t board, board_t upd_board, int action);
void poss_move(board_t board, board_t upd_board, int action);
void move_ne(board_t board, board_t upd_board, int action, int i, int j, 
	int* arr1_size);
void move_se(board_t board, board_t upd_board, int action, int i, int j, 
	int* arr1_size);
void move_sw(board_t board, board_t upd_board, int action, int i, int j, 
	int* arr1_size);
void move_nw(board_t board, board_t upd_board, int action, int i, int j, 
	int* arr1_size);
void store_arr(board_t board, int* arr1_size);
char turn(int action);

/****************************************************************/

/* Main function */
int
main(int argc, char *argv[]) {
    unsigned char board[BOARD_SIZE][BOARD_SIZE], 
    	upd_board[BOARD_SIZE][BOARD_SIZE];
    char old_col, new_col;
    int old_row, new_row, action = 0;
	
    // YOUR IMPLEMENTATION OF STAGES 0-2
    initial(board);
    /* Read moves in the input data */
    while (scanf("%c%d-%c%d\n", &old_col, &old_row, &new_col, &new_row) == 4) 
    		{
    	action++;
    	move(board, upd_board, old_col, old_row, new_col, new_row, action);    	
    	
    	/* Print board details and repeat for every move in input data */
    	board_details(upd_board, old_col, old_row, new_col, new_row, action);
    	print_board(upd_board);
    	board_swap(board, upd_board);
    }
    
	/* Implement stage 1 */
	stage_1(board, upd_board, action);
	
    return EXIT_SUCCESS;            // exit program with the success code
}

/****************************************************************/

/* Store the board setup into a 2D array */
void
initial(board_t setup) {
	int row, col;
	int count_w = 0;
	int count_b = 0;
	
	/* Iterate for all rows in board */
	for (row = 0; row < BOARD_SIZE; row++) {
		
		/* Iterate for all columns in board */
		for (col = 0; col < BOARD_SIZE; col++) {
			
			/* Pieces  exist */
			if (((row % 2 != 0 && col % 2 == 0)  || 
					(row % 2 == 0 && col % 2 != 0)) && row != 3 && row != 4) {
				
				if (row < ROWS_WITH_PIECES) {
					/* Place white pieces */
					setup[row][col] = CELL_WPIECE;
					count_w = count_pieces(setup, CELL_WPIECE);
				} else if (row >= (BOARD_SIZE - ROWS_WITH_PIECES)) {
					/* Place black pieces */
					setup[row][col] = CELL_BPIECE;
					count_b = count_pieces(setup, CELL_BPIECE );
				}
			
			/* Pieces do not exist */	
			} else {
				setup[row][col] = CELL_EMPTY;
			}
			
		}
		
	}
	
	/* Print board details and initial board */
	printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
	printf("#BLACK PIECES: %d\n", count_b);
	printf("#WHITE PIECES: %d\n", count_w);
	
	print_board(setup);
	return;
}

/****************************************************************/

/* Print a neatly-formatted board structure */
void
print_board(board_t board) {
	int row, col, alph;
	int num=1;
	
	/* Column configuration */
	printf("  ");
	for (alph = MIN_COL; alph < MAX_COL; alph++) {
		printf("%4c", alph);
	}
	
	print_dividers();
	for (row = 0; row<BOARD_SIZE; row++) {
		/* Row configuration */
		printf("%2d |", num++);
		for (col = 0; col < BOARD_SIZE; col++) {
			/* Input board contents */
			printf("%2c |", board[row][col]);
		}
		print_dividers();
	}
	return;
}

/****************************************************************/
	
/* Read moves in the input data and implement */
void
move(board_t board, board_t upd_board, int old_col, int old_row, int new_col, 
		int new_row, int action) {
	int row, col, error, capture;
	
	if (old_col > 7) {
		/* Convert data into readable arrays */
		old_col -= MIN_COL;
		old_row += DATA_TO_ARRAY;
		new_col -= MIN_COL;
		new_row += DATA_TO_ARRAY;
	}
	
	/* Detect and print error when move is invalid */
	error = check_error(board, action, old_col, old_row, new_col, new_row);
	if (error) {
		print_error(error);
		exit(EXIT_SUCCESS); 
	}
		
	/* Check if the movement is a capture */
	capture = 0;
	if ((old_col-new_col)==2 || (new_col-old_col)==2) {
		capture = TRUE;
	} 
		
	/* Update board after movement */
	/* Iterate for all rows in board */
	for (row = 0; row < BOARD_SIZE; row++) {
		
		/* Iterate for all columns in board */
		for (col = 0; col < BOARD_SIZE; col++) {
			
			if (row == new_row && col == new_col) {
				/* Fill the target cell */
				upd_board[row][col] = board[old_row][old_col];	
			} else if (row == old_row && col == old_col) {
				/* Empty the source cell */
				upd_board[row][col] = CELL_EMPTY;
			}  else if (capture && row == (old_row + new_row)/2 
					&& col == (old_col + new_col)/2) {
				/* Empty the captured cell */
				upd_board[row][col] = CELL_EMPTY;
			} else {
				/* Other cells remain the same */
				upd_board[row][col] = board[row][col];
			}
			
			/* When target cell reaches the opposite end */
			if (row == new_row && col == new_col) {
				if ((action % 2 != 0 && new_row == 0) || 
						(action % 2 == 0 && new_row == 7)) {
					upd_board[row][col] -= 32;
				}
			}
				
		}
			
	} 
	return;
}

/****************************************************************/

/*  Check for invalid moves.
	Return the type of error. */
int
check_error(board_t board, int action, int old_col, int old_row, 
		int new_col, int new_row) {
	int error;
	int step_col = old_col - new_col;
	int step_row = old_row - new_row;
	char captured = board[(old_row + new_row)/2][(old_col + new_col)/2];
	char old_cell = board[old_row][old_col];
	char new_cell = board[new_row][new_col];

	error = 0;
	if ((old_col < 0) || (old_col > 7) || (old_row < 0) || (old_row > 7)) {
		/* Invalid source cell */
		error = 1;
	} else if ((new_col < 0) || (new_col > 7) || (new_row < 0) || 
			(new_row > 7)) {
		/* Invalid target cell */
		error = 2;
	} else if (old_cell == CELL_EMPTY) {
		/* Empty source cell */
		error = 3;
	} else if (new_cell != CELL_EMPTY) {
		/* Filled arget cell */
		error = 4;
	} else if ((action%2 == 0 && (old_cell != CELL_WPIECE && 
			old_cell != CELL_WTOWER)) || (action%2 != 0 && 
			(old_cell != CELL_BPIECE && old_cell != CELL_BTOWER))) {
		/* Opponent's action */
		error = 5;		
	} else if (((step_col != 1 && step_col != -1) || 
			(step_row != 1 && step_row != -1)) && 
			((step_col != 2 && step_col != -2) || 
			(step_row != 2 && step_row != -2))) {
		/* Illegal action */
		error = 6;
	} else if ((step_col == 2 || step_col == -2) && 
			(step_row == 2 || step_row == -2)){
		if ((action%2 == 0 && (captured != CELL_BPIECE && 
				captured != CELL_BTOWER)) || (action%2 != 0 && 
				(captured != CELL_WPIECE && captured != CELL_WTOWER))) {
		/* Illegal action */
			error = 6;
		}
	}
	return error;
}

/****************************************************************/

/* Print board details */
void
board_details(board_t board, char old_col, int old_row, char new_col,
		int new_row, int action) {
	int cost, i;
	
	for (i=1; i<=37; i++) {
		printf("=");
	}
	printf("\n");
	if (action%2==0) {
		printf("WHITE ");
	} else {
		printf("BLACK ");
	}
	printf("ACTION #%d: %c%d-%c%d\n", action, old_col, old_row, new_col, 
		new_row);
	cost = board_cost(board);
	
	printf("BOARD COST: %d\n", cost);
	return;
}

/****************************************************************/

/* Count board cost */
int
board_cost(board_t upd_board) {
	int count_b, count_w, count_B, count_W, cost;
	count_b = count_pieces(upd_board, CELL_BPIECE);
	count_w = count_pieces(upd_board, CELL_WPIECE);
	count_B = count_pieces(upd_board, CELL_BTOWER );
	count_W = count_pieces(upd_board, CELL_WTOWER);
	
	cost = COST_PIECE*count_b + COST_TOWER*count_B - COST_PIECE*count_w 
		- COST_PIECE*count_W;
	return cost;
}

/****************************************************************/

/* Count black and white pieces on board */
int
count_pieces(board_t board, char elem) {
	int i, j, count;
	count = 0;
	for (i=0; i<BOARD_SIZE; i++) {
		for (j=0; j<BOARD_SIZE; j++) {
			if (board[i][j] == elem) {
				count++;
			}
		}
	}
	return count;
}

/****************************************************************/

/* Print out the errors according to the type of error */
void
print_error(int error) {
	if (error==1) {
		printf("ERROR: Source cell is outside of the board.\n");
	} else if (error==2) {
		printf("ERROR: Target cell is outside of the board.\n");
	} else if(error==3) {
		printf("ERROR: Source cell is empty.\n");
	} else if (error==4) {
		printf("ERROR: Target cell is not empty.\n");
	} else if(error==5) {
		printf("ERROR: Source cell holds opponent's piece/tower.\n");		
	} else if(error==6) {
		printf("ERROR: Illegal action.\n");
	}
	return;
}

/****************************************************************/

/* Swap every elements in the two arrays */	
void 
board_swap(board_t board, board_t board_upd) {
	int row, col;
	char tmp;
	
	for (row = 0; row < BOARD_SIZE; row++) {
		for (col = 0; col < BOARD_SIZE; col++) {
			tmp = board[row][col];
			board[row][col] = board_upd[row][col] ;
			board_upd[row][col] = tmp;
		}
	}
	return;
}

/****************************************************************/

/* Implement stage 1 */
void
stage_1(board_t board, board_t upd_board, int action) {
	poss_move(board, upd_board, action);
	return;
}

/****************************************************************/

/* Store all possible moves */
void
store_arr(board_t upd_board, int* arr1_size) {
	node arr1[50];
	int i, j;
	
	/* Store array from board to struct */
	for (i=0; i<BOARD_SIZE; i++) {
		for (j=0; j<BOARD_SIZE; j++) {
			arr1[(*arr1_size)].board[i][j] = upd_board[i][j];
		}
	}
	(*arr1_size)++;
	return;
}

/****************************************************************/

/* Attempting all possible moves */
void
poss_move(board_t board, board_t upd_board, int action) {
	int arr1_size = 0;
	int i, j;
	
	for (i=0; i<BOARD_SIZE; i++) {
		for (j=0; j<BOARD_SIZE; j++) {
			if (board[i][j] == turn(action)) {
				if (turn(action) == 'b') {
					move_ne(board, upd_board, action, i, j, &arr1_size);
					move_nw(board, upd_board, action, i, j, &arr1_size);
				} if (turn(action) == 'w') {
					move_se(board, upd_board, action, i, j, &arr1_size);
					move_sw(board, upd_board, action, i, j, &arr1_size);
				}
			} 
			if (board[i][j] == 'B' || board[i][j] == 'W') {
				move_ne(board, upd_board, action, i, j, &arr1_size);
				move_se(board, upd_board, action, i, j, &arr1_size);
				move_sw(board, upd_board, action, i, j, &arr1_size);
				move_nw(board, upd_board, action, i, j, &arr1_size);
			}	
		}
	}
	return;
}

/****************************************************************/

/* Moves for every direction */
void
move_ne(board_t board, board_t upd_board, int action, int i, int j, 
		int* arr1_size) {
	int m = 0;
	if (!check_error(board, action, j, i, j+1, i-1)) {
		m = 1;
		move(board, upd_board, j, i, j+1, i-1, action);
	} else if (!check_error(board, action, j, i, j+2, i-2)) {
		m = 1;
		move(board, upd_board, j, i, j+2, i-2, action);	
	} 
	if (m) {
		store_arr(upd_board, arr1_size);
	}
	return;
}

void
move_se(board_t board, board_t upd_board, int action, int i, int j, 
		int* arr1_size) {
	int m = 0;
	if (!check_error(board, action, j, i, j+1, i+1)) {
		m = 1;
		move(board, upd_board, j, i, j+1, i+1, action);
	} else if(!check_error(board, action, j, i, j+2, i+2)) {
		m = 1;
		move(board, upd_board, j, i, j+2, i+2, action);
	}
	if (m) {
		store_arr(upd_board, arr1_size);
	}
	return;
}

void
move_sw(board_t board, board_t upd_board, int action, int i, int j, 
		int* arr1_size) {
	int m = 0;
	if (!check_error(board, action, j, i, j-1, i+1)) {
		m = 1;
		move(board, upd_board, j, i, j-1, i+1, action);
	} else if (!check_error(board, action, j, i, j-2, i+2)) {
		m = 1;
		move(board, upd_board, j, i, j-2, i+2, action);
	}  
	if (m) {
		store_arr(upd_board, arr1_size);
	}
	return;
}

void
move_nw(board_t board, board_t upd_board, int action, int i, int j, 
		int* arr1_size) {	
	int m = 0;
	if (!check_error(board, action, j, i, j-1, i-1)) {
		m = 1;
		move(board, upd_board, j, i, j-1, i-1, action);
	} else if (!check_error(board, action, j, i, j-2, i-2)) {
		m = 1;
		move(board, upd_board, j, i, j-2, i-2, action);
	}
	if (m) {
		store_arr(upd_board, arr1_size);
	}
	return;
} 
	
/****************************************************************/

/* Other helper function */
char
turn(int action) {
	if (action%2==0) {
		return 'w';
	} else {
		return 'b';
	}	
}

void
print_dividers() {
	printf("\n   +---+---+---+---+---+---+---+---+\n");
	return;
}

/* THE END -------------------------------------------------------------------*/

/* algorithms are fun */


/* 
Struct Explanation

 // node_val will be updated after I generate and calculate all child nodes.
	* If current piece is white -> arr1[arr1_size].node_value = MAX_VAL
	* If current piece is black -> arr1[arr1_size].node_value = MIN_VAL

 // as all possible movements are stored in an array standing one after 
 another,
	* first_pos denotes the first index of the node's first child 
	* last_pos denotes the first index of the node's last child 

*/

/* 
My plan

for all board in arr1 
{
	generate child_nodes and store it into arr2
}

for all board in arr2 
{
	generate child_nodes and store it into arr3
}

for all board in arr3
{
	arr3[].node_val = board_cost
}

for all board in arr2 
{
	arr2[].node_val = min_or_max of all of the node child in arr3
}

for all board in arr1 
{
	arr1[].node_val = min_or_max of all of the node child in arr2
}

After having all arr1 updated, find min_or_max to decide next move
Example: If arr1[10] has the min node_val then do arr1[10] move and set 
current board to arr1[10].board

Stage 2 should be implementing stage 1 for 10 times
*/


/* 
Failure of stage 1 explanation 

Failure of implementing stage 1 was because the fact that when I store the 
board, it looped over all cells in the board on that particular direction, 
however when I tried to print all the possible moves on function move, it 
presents the possible board that I am expecting. 
*/
