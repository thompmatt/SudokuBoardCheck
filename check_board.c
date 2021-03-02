///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
//
// We have provided comments and structure for this program to help you get 
// started.  Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        check_board.c
// This File:        check_board.c
// Other Files:      N/A
// Semester:         CS 354 Spring 2021
//
// Author:           Matthew Thompson Soto
// Email:            mathompson23@wisc.edu
// CS Login:         thompson-soto
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
     
char *DELIM = ",";  // commas ',' are a common delimiter character for data strings
     
/* COMPLETED (DO NOT EDIT):       
 * Read the first line of input file to get the size of that board.
 * 
 * PRE-CONDITION #1: file exists
 * PRE-CONDITION #2: first line of file contains valid non-zero integer value
 *
 * fptr: file pointer for the board's input file
 * size: a pointer to an int to store the size
 */
void get_board_size(FILE *fptr, int *size) {      
    char *line1 = NULL;
    size_t len = 0;
    if ( getline(&line1, &len, fptr) == -1 ) {
        printf("Error reading the input file.\n");
        exit(1);
    }

    char *size_chars = NULL;
    size_chars = strtok(line1, DELIM);
    *size = atoi(size_chars);
}

/* Returns 1 if and only if the board is in a valid Sudoku board state.
 * Otherwise returns 0.
 * 
 * A valid row or column contains only blanks or the digits 1-size, 
 * with no duplicate digits, where size is the value 1 to 9.
 * 
 * Note: p2A requires only that each row and each column are valid.
 * 
 * board: heap allocated 2D array of integers 
 * size:  number of rows and columns in the board
 */
int valid_board(int **board, int size) {
	// Outermost for loop iterates through the "rows" of board
	for (int row = 0; row < size; row++) {
		// Middle for loop that iterates through the "columns" of board
		for (int column = 0; column < size; column++) {
			// Holds current value at position [row, column] in board
			int currentNum = *(*(board + row) + column);
			
			// If value currentNum is not an int between 0-n
			if (currentNum < 0 || currentNum > size) {
				return 0;
			}
			
			// If currentNum is a 0, skips current iteration
			if (currentNum == 0) {
				continue;
			}
			
			// For loop that checks currentNum against other numbers in same row
			for (int nextInRow = column + 1; nextInRow < size; nextInRow++) {
				if (currentNum == *(*(board + row) + nextInRow)) {
					return 0;
				}
			}
			
			// For loop that checks currentNum against other numbers in same column
			for (int nextInCol = row + 1; nextInCol < size; nextInCol++) {
				if (currentNum == *(*(board + nextInCol) + column)) {
					return 0;
				}
			}
		}
	}

    return 1; // Valid board state   
}    
   
/* This program prints "valid" (without quotes) if the input file contains
 * a valid state of a Sudoku puzzle board wrt to rows and columns only.
 *
 * argc: CLA count
 * argv: CLA value
 */
int main( int argc, char *argv[] ) {
    // Checks if number of command-line arguments is correct (should be 2).
	if (argc != 2) {
		printf("Usage: ./check_board <input_filename>\n");
		exit(1);
	}	

    // Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) { // Checks return value
        printf("Can't open file for reading.\n");
        exit(1);
    }

    int size; // Variable to hold board size

    // Calls get_board_size to read first line of file as the board size.
	get_board_size(fp, &size);	
	
	if (size < 1 || size > 9) { // Checks if size is valid before proceeding
		printf("invalid\n");
		exit(1);
	} 

    // Dynamically allocate a 2D array for given board size.
	int **arr = malloc(sizeof(int *) * size);

	// Check the return value of previous malloc (check for NULL)
	if (arr == NULL) {
		printf("Failed to use malloc(), returned NULL\n");
		exit(1);
	}
	
	// Allocate a row of n ints to arr pointers
	for (int i = 0; i < size; i++) {
		*(arr + i) = malloc(sizeof(int) * size);
		
		// Check the return value of previous malloc (check for NULL)
		if (*(arr + i) == NULL) {
			printf("Failed to use malloc(), returned NULL\n");
			exit(1);
		}
	}

    // Read the rest of the file line by line.
    // Tokenize each line wrt the delimiter character 
    // and store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {
        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading line %i of the file.\n", i+2);
            exit(1);
        }

        token = strtok(line, DELIM);

        for (int j = 0; j < size; j++) {
            // Initialized arr at row i, column j with appropriate values
            *(*(arr + i) + j)  = atoi(token);
            token = strtok(NULL, DELIM);
        }
    }
	
	int retVal = valid_board(arr, size); // Calls the function valid_board()

	if (retVal == 1) { // If retVal is 1, board was valid, otherwise invalid or 0
		printf("valid\n");
	} else {
		printf("invalid\n");
	}
	
	// First frees the dynamically allocated memory of inner pointers of arr...
	for (int i = 0; i < size; i++) {
		free(*(arr + i));
	}
	free(arr); // ...then free heap memory initially allocated for arr

    // Closes the file and checks return value for fclose()
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
     
    return 0;       
}       

// s21
