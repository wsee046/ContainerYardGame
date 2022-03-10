/* ENGGEN131 Project - C Project - 2021 */
/* Container Yard */

/* << Include your information here - name, user name, ID number >> 
Author: Wen Jie See
UPI: wsee046
ID: 185513479
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define NUM_ROWS 8
#define NUM_COLS 8

#define VACANT 0
#define BOUNDARY -1
#define ENTRY -2
#define EXIT -3

void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index);
void PrintFloor(int floor[NUM_ROWS][NUM_COLS]);
double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width);
void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction);
int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int* rowStart, int* colStart, int* rowEnd, int* colEnd);
int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked);

/* Function to obtain capital letter as input */
char GetMove(void)
{
	char move;
	printf("\nMove container: ");
	scanf("%c", &move);
	// Ignore non-capital letter inputs
	while ((move < 'A') || (move > 'Z')) {
		scanf("%c", &move);
	}
	return move;
}

int main()
{
	int gameOver = 0;
	int isBlocked = 0;
	int floor[NUM_ROWS][NUM_COLS];
	int rowStart, colStart, rowEnd, colEnd;
	char input;

	/* Print banner */
	printf("............**********************************............\n");
	printf("............* CONTAINER YARD GAME SIMULATION *............\n");
	printf("............**********************************............\n");

	/* Initialise the yard floor grid and add containers */
	InitialiseFloor(floor, 'R', 3);
	AddContainer(floor, 28, 2, 0);
	AddContainer(floor, 11, 3, 1);
	AddContainer(floor, 41, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 34, 2, 0);
	AddContainer(floor, 36, 3, 1);
	AddContainer(floor, 37, 2, 1);
	AddContainer(floor, 53, 2, 0);
	AddContainer(floor, 30, 3, 1);

	/* Print status */
	printf("ENGGEN131 2021 - C Project\nContainer Yard!  The containers are rushing in!\n");
	printf("In fact, %.2f%% of the yard floor is available for containers!\n\n", FloorAreaAvailable(floor, 20.5, 10.3));

	/* Main simulation loop */
	while (gameOver != 2) {
		PrintFloor(floor);
		input = GetMove();
		isBlocked = LocateContainer(floor, input, &rowStart, &colStart, &rowEnd, &colEnd);
		gameOver = MoveContainer(floor, rowStart, colStart, rowEnd, colEnd, isBlocked);
	}

	/* A container is ready to exit - the simulation is over */
	PrintFloor(floor);
	printf("\nCongratulations, you've succeeded!");

	return 0;
}

double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width)
{
	/*
	FloorAreaAvailable will calculate the total available area of the container yard
	Inputs:		floor[NUM_ROWS][NUM_COLS] = a 2D array representing the container yard
				length: double variable representing the length of a cell in feet
				width: double variable reprsenting the width of a cell in feet
	Output:		returns a double variable representing the total available area
	*/
	//variable free represents the number of free cells
	int free = 0;
	int i, j;

	//calculate the area of each grid cell
	double cellArea = width * length;

	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (floor[i][j] == 0) {
				//if the cell is vacant, increment free by 1
				free++;
			}
		}
	}
	//total area = number of vacant cells x cell area
	return free * cellArea;
}

void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index)
{
	/*
	InitialiseFloor will initialise a 2D array representing a container yard.
	Inputs:		floor[NUM_ROWS][NUM_COLS] = a 2D array representing the container yard
				entryboundary: a character of T, R, B or L representing where the entry will be
				index: the row or column index of the entry
	Output:		none
	*/
	int i, j;

	//sets boundary and vacant elements first
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (i == 0 || i == NUM_ROWS - 1 || j == 0 || j == NUM_COLS - 1) {
				//sets -1 to the boundary
				floor[i][j] = BOUNDARY;
			} else {
				floor[i][j] = VACANT;
			}
		}
	}

	//sets entry and exits
	if (entryboundary == 'T') {
		floor[0][index] = ENTRY;
		floor[NUM_ROWS - 1][index] = EXIT;
	} else if (entryboundary == 'R') {
		floor[index][NUM_COLS - 1] = ENTRY;
		floor[index][0] = EXIT;
	} else if (entryboundary == 'B') {
		floor[NUM_ROWS - 1][index] = ENTRY;
		floor[0][index] = EXIT;
	} else if (entryboundary == 'L') {
		floor[index][0] = ENTRY;
		floor[index][NUM_COLS - 1] = EXIT;
	}
}

void PrintFloor(int floor[NUM_ROWS][NUM_COLS])
{
	/*
	PrintFloor will will print out the graphical representation of the container yard, 
	using U for entry, X for exit, empty space for vacancy and the letter corresponding 
	to the container in the cell.
	Input:		floor[NUM_ROWS][NUM_COLS] = a 2D array representing the container yard
	Output:		none
	*/
	int i, j;

	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			//if boundary cell, print @
			if (floor[i][j] == -1) {
				printf("@");
			} else if (floor[i][j] == -2) {
				//if entry cell, print U
				printf("U");
			} else if (floor[i][j] == -3) {
				//if exit cell, print X
				printf("X");
			} else if (floor[i][j] == 0) {
				//if vacant cell, print an empty space
				printf(" ");
			} else {
				// if not vacant, print the letter corresponding to the container
				printf("%c",floor[i][j]);
			} 
		}
		//prints a new line for each row
		printf("\n");
	}
}

void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction)
{
	/*
	AddContainer will add a container to the 2D array 
	Inputs:		floor[NUM_ROWS][NUM_COLS] = a 2D array representing the container yard
				position = int variable representing the locaton of container
				size = int variable representing the size of container
				direction = int variable of either 0 or 1 representing horizontal 
					or vertical orientation for the container, repectively
	Output:		none
	*/
	int i, j;
	
	//variable check is used to check if cells are empty
	int check = 1;

	//following code checks if the cells are empty
	if (direction) {
		//calculates the column index for vertical direction
		j = position % (NUM_COLS);
		for (i = position / (NUM_COLS); i < (position / (NUM_COLS)+size); i++) {
			if (i < NUM_ROWS && floor[i][j] != 0) {
				check = 0;
			}
		}
	}
	else {
		//calculates the row index for horizontal direction
		i = position / NUM_COLS;
		for (j = position % (NUM_COLS); j < (position % (NUM_COLS)+size); j++) {
			if (j < NUM_COLS && floor[i][j] != 0) {
				check = 0;
			}
		}
	}

	char letter = 'A';

	//container will be added only if cells are vacant
	if (check) {
		//checks which letter to use for container
		int count = 1;
		while (count > 0) {
			count = 0;
			for (i = 0; i < NUM_ROWS; i++) {
				for (j = 0; j < NUM_COLS; j++) {
					//if the letter is used, use the next letter	
					if (floor[i][j] > 0 && floor[i][j] == letter) {
						letter = letter + 1;
						count++;
					}
				}
			}
		}

		//checks whether the direction if vertical or horizontal
		if (direction) {
			j = position % (NUM_COLS);
			for (i = position / (NUM_COLS); i < (position / (NUM_COLS)+size); i++) {
				if (i < NUM_ROWS) {
					//sets element to container letter
					floor[i][j] = letter;
				}
			}
		}
		else {
			i = position / NUM_COLS;
			for (j = position % (NUM_COLS); j < (position % (NUM_COLS)+size); j++) {
				if (j < NUM_COLS) {
					//sets element to container letter
					floor[i][j] = letter;
				}
			}
		}
	}
}

int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int* rowStart, int* colStart, int* rowEnd, int* colEnd)
	{
		/*
		LocateContainer will locate a specified container and store the location of the left-most
		or upper-most part, and the right-most or lower-most part of the container in predefined variables.
		It will also return an integer (0 or 1) indicating if the container is blocked from front and back or
		if unblocked, respectively.
		Inputs:		floor[NUM_ROWS][NUM_COLS] = a 2D array representing the container yard
					move = a character variable representing the container to find
					*rowStart = an address to store the starting row index
					*colStart = an address to store the starting column index
					*rowEnd = an address to store the ending row index
					*colEnd = an address to store the ending column index
		Output:		an integer (0 or 1) indicating if the container is blocked from front and back or
					if unblocked, respectively.
		*/
		int i, j;
		int count = 0;

		//finds the starting and ending positions of the container
		for (i = 0; i < NUM_ROWS; i++) {
			for (j = 0; j < NUM_COLS; j++) {
				if (floor[i][j] == move) {
					count++;
					//for the first search, set indices for starting position
					if (count == 1) {
						*rowStart = i;
						*colStart = j;
					}
					//these two variables will end up being the indices of the end position
					*rowEnd = i;
					*colEnd = j;
				}
			}
		}

		//checks if the container is blocked at front and back
		//check if horizontal
		if (*rowStart == *rowEnd) {
			//returns 0 only if horizontal container is blocked from left and right
			return (floor[*rowStart][*colStart - 1] == 0 || floor[*rowEnd][*colEnd + 1] == 0);
		}
		else {
			//returns 0 only if vertical container is blocked from top and bottom
			return (floor[*rowStart - 1][*colStart] == 0 || floor[*rowEnd + 1][*colEnd] == 0);
		}
	}

int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked)
{
	/*
	MoveContainer will move a container until it hits a boundary. It will prioritize moving left or up
	for horizontal and vertical containers, respectively. If blocked from those two directions, it will
	move the horizontal or vertical container right or down respectively.
	Inputs:		floor[NUM_ROWS][NUM_COLS] = a 2D array representing the container yard
				r0 = the starting row index
				c0 = the starting column index
				r1 = the ending row index
				c1 = the ending column index
				isBlocked = integer value representing whether the container is blocked from both front and back
	Output:		returns 1 if contaienr is next to entry (lengthwise)
				returns 2 if container is next to exit (lengthwise)
				returns 0 if container was moved and is not next to entry or exit (lengthwise)
				returns -1 if container was not moved and is not next to entry or exit (lengthwise)
	*/

	//check if horizontal
	if (r0 == r1) {
		//check if the cell immediately to the left is empty to move block left
		if (floor[r0][c0 - 1] == 0) {
			//while the left cell is empty and column index is greater than 0, replace empty left cell with rightmost cell. 
			while (floor[r0][c0 - 1] == 0 && c0 > 0) {
				floor[r0][c0 - 1] = floor[r1][c1];
				//Change rightmost cell to a 0 to indicate vacancy
				floor[r1][c1] = 0;
				c0--;
				c1--;
			}
		}
		//else move the block to the right
		else {
			while (floor[r1][c1 + 1] == 0 && c1 < NUM_ROWS - 1) {
				floor[r1][c1 + 1] = floor[r0][c0];
				//change leftmost cell to a 0 to indicate vacancy
				floor[r0][c0] = 0;
				c0++;
				c1++;
			}
		}
		if (floor[r0][c0 - 1] == EXIT|| floor[r1][c1 + 1] == EXIT) {
			return 2;
		}
		else if (floor[r0][c0 - 1] == ENTRY || floor[r1][c1 + 1] == ENTRY) {
			return 1;
		}
		else {
			return (isBlocked - 1);
		}
	}//else if vertical
	else if (c0 == c1) {
		//checks if cell immediately above is empty
		if (floor[r0 - 1][c0] == 0) {
			//while the top cell is empty and row index is greater than 0
			while (floor[r0 - 1][c0] == 0 && r0 > 0) {
				floor[r0 - 1][c0] = floor[r1][c1];
				//change bottom cell to 0 to indicate vacancy
				floor[r1][c1] = 0;
				//decrease the starting and ending row index by 1
				r0--;
				r1--;
			}
		}
		//if cell above is not empty, move container down
		else {
			//while the bottom cell is empty and row index is less than the limit
			while (floor[r1 + 1][c1] == 0 && r1 < NUM_ROWS - 1) {
				floor[r1 + 1][c1] = floor[r0][c0];
				//set top cell to 0 to indicate vacancy
				floor[r0][c0] = 0;
				//increment the starting and ending row index by 1
				r1++;
				r0++;
			}
		}
		if (floor[r0-1][c0] == EXIT || floor[r1 + 1][c1] == EXIT) {
			return 2;
		}
		else if (floor[r0-1][c0] == ENTRY || floor[r1 + 1][c1] == ENTRY) {
			return 1;
		}
		else {
			return (isBlocked - 1);
		}
	}

	return 0;
} 

