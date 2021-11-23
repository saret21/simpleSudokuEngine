#include "sudoku.h"
#define REMOVE_NEXT_TO_ZEROES 1
#define REMOVE_NEXT_TO_NUMBERS -1
#define NO_BACKTRACKING_WHEN_GENERATING 0  /*this of course will not work produces puzzles with 60elts..*/

/*next defines go to grid.h*/


/*ALLWAYS SET BY MAIN*/
static unsigned int random_level=0;

/*sets randomness and returns it*/
void grid_randomness_set(int level){
	random_level=(unsigned)level;
	return;
}
int grid_randomness_get(void){
	return random_level;
}


/*new choices never made, when this is to be done, we do a full initialisation*/
void reduce_choices(Grid_T *g, int val, int i, int j, int l, int m, FILE* s){
	
	int k;
	/*if (val==0){return;}*/			/*This line is not needed because it is in apply*/
	if ((i==l)&&(j==m)){
								/*SOS!!!!!!!!! NO g->level++;

								No global changes in this, ie no changes
								of the form g->attr=? , only local
								of the form g->elts[l][m].attr=?
								global changes handled separately*/
		g->elts[i][j].val=val;
		for(k=1;k<SIDE+1;k++){
			g->elts[i][j].choices.num[k]=0;
		}
		g->elts[i][j].choices.num[val]=1;

		return;
	}
	if (g->elts[l][m].choices.num[val]==1){
			g->elts[l][m].choices.num[val]=0;
            g->elts[l][m].choices.count-=1;
        }
        return;
	/*DEBUG
	if((l==0)&&(m==0)){
                fprintf(s, "AFTER: (i,j)=(%d,%d), val=%d, num[val]=%d, choices=%d\n",
                                i,j,val,g->elts[l][m].choices.num[val],
                                g->elts[l][m].choices.count);
        }
*/
}

void verify_removal_candidate(Grid_T* g, int do_sth, int i, int j, int l, int m, FILE* s){
	int k;
	/*int temp*/
							/*when you go into here you already have initialised:
									g->elts[i][j].choices.count=0;
									elts[i][j].choices.num[k]=1; all k*/
	if ((i!=l)||(j!=m)){
		/*if((i==0)&&(j==8)){fprintf(stderr, "VRC(g, best=%d, i=%d, j=%d, l=%d, m=%d)\n",g->unique, i,j,l,m );}*/
		if (g->elts[l][m].val!=0){
			g->elts[i][j].choices.count+=1;
			g->elts[i][j].choices.num[g->elts[l][m].val]=0;
		}
	}
	/*ayth, h apply thn pairnei teleutaia*/
	if((i==l)&&(j==m)){
		/*if((i==0)&&(j==8)){fprintf(stderr, "VRC(g, best=%d, i=%d, j=%d, l=%d, m=%d)\n",g->unique, i,j,l,m );}*/
		g->elts[i][j].choices.num[0]=1;
		for(k=1; k<10;k++){
			/*if((i==0)&&(j==8)){fprintf(stderr, "g->elts[%d][%d].choices.num[%d]=%d\n", i,j,k, g->elts[i][j].choices.num[k]);}*/
			if(g->elts[i][j].choices.num[k]>0){
				/*an kapoio den exei apokleistei*/
				if(k!=g->elts[i][j].val){		/*ektos apo to value tou tetragwnou
														(pou den tha mporouse na exei apokleistei)*/
					if((i==0)&&(j==8)){
						/*fprintf(stderr, "VRC(0,8,0,8) fail time????\n" );*/
					}
					g->elts[i][j].choices.num[0]=0; break;
					/*den mporeis na valeis 0, fuge*/
				}
			}
			
		}
		/*if ((i==0)&&(j==8)){
			fprintf(stderr, "VRC(%d,%d,%d.%d): g->level=%d g->unique=%d",i,j,l,m,
									g->level, g->unique);
			fprintf(stderr, "got g->elts[%d][%d].choices.num[0]=%d, g->elts[%d][%d].choices.count=%d",
				i,j,g->elts[i][j].choices.num[0],i,j, g->elts[i][j].choices.count);
		}*/
		if(g->elts[i][j].choices.num[0]>0){
			switch (g->level){	
				case REMOVE_NEXT_TO_ZEROES:
					if(g->elts[i][j].choices.count<g->unique){
						g->I=i; g->J=j; g->unique=g->elts[i][j].choices.count; break;
					}
				case REMOVE_NEXT_TO_NUMBERS:
					if(g->elts[i][j].choices.count>g->unique){
						g->I=i; g->J=j; g->unique=g->elts[i][j].choices.count; break;
					}
				default:
					g->I=i; g->J=j; g->unique=g->elts[i][j].choices.count; break;
			}
			
		}
		/*if ((i==0)&&(j==8)){
			fprintf(stderr, "VRC(%d,%d,%d.%d): returning g->level=%d g->unique=%d",i,j,l,m,
									g->level, g->unique);
			fprintf(stderr, "got g->elts[%d][%d].choices.num[0]=%d, g->elts[%d][%d].choices.count=%d",
				i,j,g->elts[i][j].choices.num[0],i,j, g->elts[i][j].choices.count);
		}*/

	} 
}


int sudoku_find_value(Grid_T* g, int choices_count, int i, int j ){
	int k=0;
	int l=1;
/*	int debug=0;*/
	int make_nth_choice=1;
	if(choices_count%10==0){return choices_count;}


	/*sudoku_print(stderr,*g);*/


	assert((g->elts[i][j].val==0 )&&(g->elts[i][j].choices.count==choices_count));
	if (random_level>0){
		make_nth_choice+=rand()%choices_count;
	}

	while (make_nth_choice){
		k++;
		l*=2;

		if (g->elts[i][j].choices.num[k]==1){make_nth_choice--;}
	}

	g->elts[i][j].choices.num[0]+=1<<k;

	return k;
}
/*
MOVE TO GRID.C
sees all the squares that a given choice affects and applies action to them
 * no actions if val =0, this is intended to be the actual value at i,j or a fake one*/



void  apply_to_relevant_squares(	Grid_T *g, 
 					int val, int i, int j, FILE* s,
					void (*actions)(Grid_T*, int, int,int,int,int, FILE*)){
        int k, l, m;
		if(val==0){return;}
	
	/*apply to sqares in the same row, col, square*/
        for(k=0;k<SIDE;k++){
        	
        	/*if ((actions==verify_removal_candidate)&&(i==0)&&(j==8)){fprintf(stderr, "ATRS: i=%d j=%d k=%d\n",i,j,k );}*/
			l=SQRT_SIDE*(i/SQRT_SIDE)+k/SQRT_SIDE;
                m=SQRT_SIDE*(j/SQRT_SIDE)+k%SQRT_SIDE;
			
			if(j!=k){
				(*actions)(g,val,i,j,i,k,s);
				/*if ((actions==verify_removal_candidate)&&(i==0)&&(j==8)){fprintf(stderr, "ATRS: call to VRC(%d ,%d, %d, %d);\n",i,j,i,k );}*/
			}
			
			if(i!=k){
				(*actions)(g,val,i,j,k,j,s);
				/*if ((actions==verify_removal_candidate)&&(i==0)&&(j==8)){fprintf(stderr, "ATRS: call to VRC(%d ,%d, %d, %d);\n",i,j,k,j);}*/
				
			}
			
			if((i!=l)&&(j!=m)){
				(*actions)(g,val,i,j,l,m,s);
				/*if ((actions==verify_removal_candidate)&&(i==0)&&(j==8)){fprintf(stderr, "ATRS: call to VRC(%d ,%d, %d, %d);\n",i,j,l,m);}*/
			}
			
        }
    	/*FINALLY (changed) apply to (i,j) itself*/
		(*actions)(g,val,i,j,i,j,s);
		
}
#define SOLVE 0
#define GENERATE 50
#define RECURSIVE 0
#define UNIQUE_CHOICE 1
#define UNIQUE_BTR 2
#define INFORMATION 3
#define REMOVE_NEXT_TO_ZEROES 1
#define REMOVE_NEXT_TO_NUMBERS -1
#define NO_BACKTRACKING_WHEN_GENERATING 0
#define GENERATE_UNIQUE_CHOICE_DESPERATE 166
#define SIDE 9
						/*strategy info type is strategy in input
						but is overwritten by the function to signal 
						non unique choice uniqueness*/
int remove_value_keep_uniqueness(Grid_T* g, int mode, int* puzzle_type){
	int i, j, k, min, strategy_info_type;
	Grid_T copy_g;
	strategy_info_type=REMOVE_NEXT_TO_ZEROES; /*this is the backtracking strategy 
											for the unique choice part if one is to 
											change it pex to REMOVE_NEXT_TO_NUMBERS
											or to NO_BACKTRACKING_WHEN_GENERATING
											you do it from here*/
	/*fprintf(stderr,"RVKU enter\n");*/
	if(strategy_info_type==NO_BACKTRACKING_WHEN_GENERATING){return 0;}
	
	g->level= (strategy_info_type==REMOVE_NEXT_TO_NUMBERS) ? 
		REMOVE_NEXT_TO_NUMBERS : REMOVE_NEXT_TO_ZEROES ;
	g->I=10; g->J=10;
	g->unique=(g->level==REMOVE_NEXT_TO_ZEROES)? 25: -1; 

	
	for(i=0; i<SIDE ;i++){
		for(j=0; j<SIDE; j++){
			if (g->elts[i][j].val!=0){

				g->elts[i][j].choices.count=0;

				for(k=0; k<10; k++){
					g->elts[i][j].choices.num[k]=1;

				}

				apply_to_relevant_squares(g, g->level ,i, j, NULL, verify_removal_candidate);
				
			}
		}
	}
	if (g->I<10){
		g->elts[g->I][g->J].val=0;
		/*fprintf(stderr,"uniquely removed\n");*/
		return 1;
	}
	if (mode==UNIQUE_CHOICE){
		
		return 0;
	}
	for(i=0; i<SIDE; i++){
		for(j=0; j<SIDE;j++){
			if(g->elts[i][j].val!=0){
				/*for each square, if filled try to remove the value:*/
				min=0;
				for(k=1; k<SIDE+1; k++){
					/*for each other valid choice, not the one in solution, if there is a solution break for*/
					if(g->elts[i][j].choices.num[k]&&(k!=g->elts[i][j].val)){
						copy_g=*g;
						copy_g.elts[i][j].val=k;
						sudoku_init_grid_from_values(&copy_g);
						if(sudoku_find_move_coordinates(&copy_g)){
							copy_g=sudoku_solve(
							copy_g, SOLVE, RECURSIVE, TOTAL_SQUARES, &min);
							if(min){break;}
						}
					}
				}
				/*min==0 means no alternative solution was found fr given square: for was not broken*/
				if (min==0){
					/*if you dont caare for unique choice, youre done just remove it*/
					if (mode!=GENERATE_UNIQUE_CHOICE_DESPERATE){
						g->elts[i][j].val=0;
						*puzzle_type=UNIQUE_BTR;
						/*fprintf(stderr,"btr removed\n");*/
						return 1;
					}
					else{
						copy_g=*g;
						copy_g.elts[i][j].val=0;
						sudoku_init_grid_from_values(&copy_g);
						if (sudoku_solve(
							copy_g,GENERATE,UNIQUE_CHOICE,copy_g.level, &min).unique){
							g->elts[i][j].val=0;
							*puzzle_type=UNIQUE_BTR;
							/*fprintf(stderr,"uniquely desperately removed\n");*/
							return 1;
						}
					}
					
					
					/*fprintf(stderr, "succeeded, but what is this?\n");*/
					/*return 1;*/
				}
			}
		}
	}
	/*fprintf(stderr,"failed\n");*/
	return 0;
	
}
void retro_solution(Grid_T* g, int mode, int level_required, int *puzzle_type){
	int  k;
	/*sudoku_print(stderr, *g);*/
	/*fprintf(stderr,"retro solution called at level=%d",g->level);*/
	k= g->level - level_required;
	


	
	while(k >0){
		
		/*fprintf(stderr,"RS,level=%d trying to remove value..\n",k+level_required);*/
		if(!remove_value_keep_uniqueness(g, mode, puzzle_type)){
			break;
		}
		k--;
	}
	sudoku_init_grid_from_values(g);
	/*fprintf(stderr,"retro solution returned at level=%d\n",g->level);*/
	return;
}






