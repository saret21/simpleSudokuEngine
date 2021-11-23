#include "sudoku.h"

/*supposes that val is the value at i,j pos and sets implications to l,m pos, 
 *only to be used if val is not 0 and l,m is in the same row, square, or column 
 *with i,j. Prints only when (i,j)<(l,m) lexicographically, but in case they
 * are pex both in the same row and square, both will be printed*/
void print_mistake(Grid_T *g, int val, int i, int j, int l, int m, FILE* s){
	int row_with_priority, column_with_priority, square_with_priority;
        if((i==l)&&(j==m)){return;}
        
        if (g->elts[l][m].val==val){
		
        row_with_priority=(i==l)&&(j<m);
		
        column_with_priority=(j==m)&&(i<l);
		
        square_with_priority=(i/SQRT_SIDE==l/SQRT_SIDE)
					&&(j/SQRT_SIDE==m/SQRT_SIDE)
					&&((i<l)||((i==l)&&(j<m)));
		
        if (row_with_priority){
			fprintf(s,
"The sudoku entries of (same) row%d at cols %d and %d respectively are both %d\n",
					i+1,j+1,m+1,val);                              
		}                                                         
		if (column_with_priority){                                        
                        fprintf(s,                                  
"The sudoku entries of (same) col%d at rows %d and %d respectively are both %d\n",
                                        j+1,i+1, l+1,val);      
                }                                               
		if (square_with_priority){                                
                                fprintf(s,                      
"The sudoku entries of (same block) at row%d col%d and row%d col%d are both %d \n",
                                        i+1,j+1,l+1,m+1,val);

                }
        }
        return;
}


void sudoku_print_errors(Grid_T g){
        int i, j;
	    Grid_T h=g;
/*        int k;*/
        /*fprintf(stderr, "PERR start:\n");*/

	    /*for (i = 0; i < SIDE; ++i){
                j=0;
                while (j<SIDE){
                        fprintf(stderr, "%d", g.elts[i][j++].val);
                        if (j<SIDE){fputc(' ',stderr);}
                        else{fputc('\n',stderr);}
                }
        }*/
        /*fputc('\n',stderr);*/

	    for(i=0;i<SIDE;i++){
                for(j=0;j<SIDE;j++){
                        apply_to_relevant_squares(  &h ,h.elts[i][j].val, i, j, stderr,
                                        print_mistake);
                }
        }
        /*fprintf(stderr, "PERR: end\n");*/
}

void sudoku_print(FILE *s, Grid_T g){
        int i, j;

        for (i = 0; i < SIDE; ++i)
        {
                j=0;
                while (j<SIDE){
                        fprintf(s, "%d", g.elts[i][j++].val);
                        if (j<SIDE){fputc(' ',s);}
                        else{fputc('\n',s);}
                }
        }
       /* fputc('\n',s);*/

}
