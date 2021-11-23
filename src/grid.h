/********************************************************************
   grid.h

   Provides definition of grid ADT used for solving sudoku puzzles and
   functions required to access and modify elements of the structure.
*********************************************************************/
#ifndef _GRID_H
#define _GRID_H
#include <stdio.h>
#include <assert.h>
#define TOTAL_SQUARES 81
typedef struct grid_s {
  int unique;     /* if 1 after solving, puzzle has unique solution */
  int level;      /*total number of nonzero elements*/
  int I;
  int J;           /** the coordinates where the change happens*/
  struct { 
    int val;       /* value of element i,j */
    struct {         
      int count;   /* number of possible choices for element i,j */ 
      int num[10]; /* map of choices: num[k]==1 if k is a valid choice
                      for element i,j, else num[k]=0 */
    } choices;     /* struct choices is used while solving the
		      puzzle and is otherwise undefined */
  } elts[9][9];    /* sudoku grid elements; 0<=i,j<=9 */
} Grid_T;

/* update value of i,j to n */
void grid_update_value(Grid_T *g, int i, int j, int n);
/* return value of i,j */
int grid_read_value(Grid_T g, int i, int j);

/* set (to 1) or clear (to 0) choice n for elt i,j */
void grid_set_choice(Grid_T *g, int i, int j, int n);
void grid_clear_choice(Grid_T *g, int i, int j, int n);
/* true if choice n for elt i,j is valid */
int grid_choice_is_valid(Grid_T g, int i, int j, int n);
/* remove n from choices of elt i,j and adjust count only if n is a
   valid choice for elt i,j */
void grid_remove_choice(Grid_T *g, int i, int j, int n);

/* return count, set (to 9), or clear (to 0) count for elt i, j */
int grid_read_count(Grid_T g, int i, int j);
void grid_set_count(Grid_T *g, int i, int j);
void grid_clear_count(Grid_T *g, int i, int j);

/* return val, set (to 1), or clear (to 0) unique flag for g */
int grid_read_unique(Grid_T g);
void grid_set_unique(Grid_T *g);
void grid_clear_unique(Grid_T *g);

int sudoku_find_value(Grid_T* g, int min, int i, int j );
void grid_randomness_set(int level);
int grid_randomness_get(void);
void reduce_choices(Grid_T *g, int val, int i, int j, int l, int m, FILE* s);
void verify_removal_candidate(Grid_T* g, int best_so_far, int i, int j, int l, int m, FILE* s);
int sudoku_find_value(Grid_T* g, int min, int i, int j );
void  apply_to_relevant_squares(        Grid_T *g, 
                                        int val, int i, int j, FILE* s,
        void (*actions)(Grid_T*, int, int,int,int,int, FILE*));

#endif