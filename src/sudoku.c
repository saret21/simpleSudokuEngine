#include "sudoku.h"
#include <assert.h>
#define SIDE 9
#define SQRT_SIDE 3
#define FINISHED_AND_CORRECT 10

#define TOTAL_SQUARES 81
#define SOLUTION_LEVEL 81
#define SOLVE 0
#define GENERATE 50
#define RECURSIVE 0
#define UNIQUE_CHOICE 1
#define UNIQUE_BTR 2
#define INFORMATION 3


void sudoku_print_errors(Grid_T g);
Grid_T sudoku_read(void);
void  apply_to_relevant_squares(	Grid_T *g, 
 					int val, int i, int j, FILE* s,
					void (*actions)(Grid_T*, int, int,int,int,int, FILE*));
void reduce_choices(Grid_T *g, int val, int i, int j, int l, int m, FILE* s);


int sudoku_is_correct(Grid_T g){
	
	int i,j, correct;
	Grid_T copy_g=g;
	sudoku_init_grid_from_values(&copy_g);
	/*sudoku_print(stderr, g);*/
	correct=1;
	for (i=0; i<SIDE; i++){
		for(j=0; j<SIDE; j++){
			if (g.elts[i][j].choices.count==0){
				correct=0;
			}

		}
	}
	return correct;
}

void sudoku_init_grid_from_values(Grid_T *g){
	int i, j, k, val, level;
	g->unique=0;
	level=0;

	
	for(i=0;i<SIDE;i++){
		for(j=0; j<SIDE; j++){
										/*first we set all choices open =1 (count=9)
										for zero elts and =0 for nonzero (count=1) */
			val=g->elts[i][j].val;
			g->elts[i][j].choices.count=(val==0)?SIDE:1;
			g->elts[i][j].choices.num[0]=0;
			for(k=1;k<SIDE+1;k++){
				g->elts[i][j].choices.num[k]=(val==0)?1:0;
			}
			if(val>0){level++; g->elts[i][j].choices.num[val]=1;}
		}
	}
										/*then we remove all */
	g->level=level;
	/*fprintf(stderr, "INIT: level=%d\n", level);*/
	/*sudoku_print(stderr, *g);*/
	for(i=0;i<SIDE;i++){
		for(j=0;j<SIDE;j++){
			/*and here next line, s is to be set to NULL*/
			apply_to_relevant_squares(	g, g->elts[i][j].val,i,j,NULL,
					reduce_choices);
		}
	}
}
Grid_T sudoku_read_file(FILE* s){
	Grid_T g;
	int c=0; 
	int k=0;
	c=fgetc(s);
	while((k<81)&&(c!=EOF)){
		if (c==' '||c=='\n'){c=fgetc(s);} 
                else{
                        assert((unsigned)(c-'0')<10);
                        g.elts[k/SIDE][k%SIDE].val=(int)(c-'0');
                        k++;  
                        c=fgetc(s);

                }
	
	}
	while(k<81){
		g.elts[k/SIDE][k%SIDE].val=0;
		k++;
	}
	sudoku_init_grid_from_values(&g);
	return g;
}



/*supposes that val is the value at i,j pos and sets implications to l,m pos, 
 only to be used if val is not 0 and l,m is in the same row, square, or column with i,j*/
/* WENT TO GRID.C
void reduce_choices(Grid_T *g, int val, int i, int j, int l, int m, FILE* s)*/

Grid_T sudoku_read(void){
	return  sudoku_read_file(stdin);
}
/*#define REMOVE_NEXT_TO_ZEROES 1
#define REMOVE_NEXT_TO_NUMBERS -1

int main(int argc, char const *argv[]){
	
	int i,j, k,count, solved;
	srand(getpid());
	Grid_T g;

	g=sudoku_generate_solutions(UNIQUE_CHOICE, 1000, 40, REMOVE_NEXT_TO_ZEROES);
	g=sudoku_solve(g, SOLVE, 4, g.level, &solved);

	return 0;
}
*/
 


























