/********************************************************************
   sudoku.h

   Sudoku solver/generator interface.
*********************************************************************/
#ifndef _SUDOKU_H
#define _SUDOKU_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "grid.h"
#define SIDE 9
#define SQRT_SIDE 3


#define FINISHED_AND_CORRECT 10
int prepare_next_move(Grid_T* g, int value);

int find_unique_choise(Grid_T* g);
void sudoku_init_grid_from_values(Grid_T *g);
void reduce_choices(Grid_T *g, int val, int i, int j, int l, int m, FILE* s);
void print_mistake(Grid_T *g, int val, int i, int j, int l, int m, FILE* s);
void  apply_to_relevant_squares( Grid_T *g, 
                                        int val, int i, int j, FILE* s,
                                        void (*actions)(Grid_T*, int,int,int,int,int, FILE*));
Grid_T sudoku_read(void);
void sudoku_print(FILE *s, Grid_T g);
int sudoku_solve_by_ref(Grid_T* top_g);
int sudoku_find_move_coordinates(Grid_T *g);
int sudoku_find_value(Grid_T* g, int min, int i, int j );
Grid_T sudoku_generate_solutions(int mode, int required_level, double* mean_var_estimation, int* attempts);

/* Read a sudoku grid from stdin and return an object Grid_T
   initialized to these values. The input has the format:

1 2 3 4 5 6 7 8 9 
4 5 6 7 8 9 1 2 3
7 8 9 1 2 3 4 5 6
2 3 4 5 6 7 8 9 1 
5 6 7 8 9 1 2 3 4
8 9 1 2 3 4 5 6 7 
3 4 5 6 7 8 9 1 2
6 7 8 9 1 2 3 4 5 
9 1 2 3 4 5 6 7 8 

   Each number is followed by a space. Each line is terminated with
   \n. Values of 0 indicate empty grid cells.
*/
/*Grid_T sudoku_read(void);*/

/* Print the sudoku puzzle defined by g to stream s in the same format
   as expected by sudoku_read(). */
void sudoku_print(FILE *s, Grid_T g);

/* Print all row, col, sub-grid errors/conflicts found in puzzle g;
   some errors may be reported more than once. */
void sudoku_print_errors(Grid_T g);

/* Return true iff puzzle g is correct. */
int sudoku_is_correct(Grid_T g);

/* Sovle puzzle g and return the solved puzzle; if the puzzle has
   multiple solutions, return one of the possible solutions. */
Grid_T sudoku_solve(Grid_T g, int , int , int , int*);

/* Returns true if solution g, as returned by sudoku_solve, has a
   unique choice for each step (no backtracking required). (Note, g
   must have been computed with the use of sudoku_solve.) */
int sudoku_solution_is_unique(Grid_T g);

/* Generate and return a sudoku puzzle with "approximately" nelts
   elements having non-0 value. The smaller nelts the harder may be to
   generate/solve the puzzle. For instance, nelts=81 should return a
   completed and correct puzzle. */
Grid_T sudoku_generate(int nelts);


#endif