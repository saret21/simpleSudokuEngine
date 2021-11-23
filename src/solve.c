#include "sudoku.h"
#include <string.h>
#include <assert.h>
#define TOTAL_SQUARES 81
#define SOLUTION_LEVEL 81
#define SOLVE 0
#define GENERATE 50
#define RECURSIVE 0
#define UNIQUE_CHOICE 1
#define UNIQUE_BTR 2
#define INFORMATION 3
#define MEAN 0
#define VAR 1
#define ALREADY_SOLVED 10
#define GEN_ATTEMPTS_MAX 100
#define GEN_ATTEMPTS_SUGGESTED 100
#define GENERATE_UNIQUE_CHOICE_DESPERATE 166

void retro_solution(Grid_T* g, int mode, int level_required, int* puzzle_type);
void mean_var_estimation(int* levels, int attempts, double* params);
void sudoku_init_grid_from_values(Grid_T *g);
void reduce_choices(Grid_T *g, int val, int i, int j, int l, int m, FILE* s);
void print_mistake(Grid_T *g, int val, int i, int j, int l, int m, FILE* s);
void  apply_to_relevant_squares( Grid_T *g, 
                                        int val, int i, int j, FILE* s,
                                        void (*actions)(Grid_T*, int,int,int,int,int, FILE*));
Grid_T sudoku_read(void);
void sudoku_print(FILE *s, Grid_T g);
int sudoku_solve_by_ref(Grid_T* top_g);
int sudoku_check_counts(Grid_T *g, int* ip, int* jp);
int sudoku_find_value(Grid_T* g, int min, int i, int j );
int history_no_initial_condition(Grid_T, Grid_T*);

int sudoku_has_unique_solution(Grid_T*, Grid_T*);
int unique_from_level(Grid_T* g, Grid_T* resolving, int puzzle_level, int info);


/*this has to be in the same file as check_counts and all the solvers
THIS WAS MOVED TO GRID.C*/
/*static int random_choices_up_to_some_level;*/

int sudoku_find_move_coordinates(Grid_T *g){
	/*maybe here the attributes need initialisation*/
	int i,j, make_nth_good_choice;
	/*fprintf(stderr, "MC: (move_coordinates has this input:)\n");*/
	/*sudoku_print(stderr, *g);*/
	int min=ALREADY_SOLVED;
	int counts_of_counts[10]={0};
	/*zeroes=SIDE*SIDE - g->level;*/
	/*fprintf(stderr, "MC: total zumber of zeroes in sudoku is %d\n", zeroes);*/
	make_nth_good_choice=0;
	/*correct=1;*/
	g->I=0; g->J=0;
	for(i=0;i<SIDE;i++){
		for(j=0; j<SIDE;j++){
			if(g->elts[i][j].choices.count==0){
				g->I=i; g->J=j; return 0;
			}
			else if(g->elts[i][j].val==0){
				counts_of_counts[g->elts[i][j].choices.count]++;
				if(g->elts[i][j].choices.count<min){
					g->I=i; g->J=j; min=g->elts[i][j].choices.count;
				}
				
			}
		}
	}
	if ((min==ALREADY_SOLVED)||(grid_randomness_get() < g->level)){return min;}
	
	make_nth_good_choice+=1+rand()%counts_of_counts[min];

			/*fprintf(stderr, "make_nth_good_choice>0\n");*/
	for(i=0;i<SIDE;i++){
		for(j=0;j<SIDE;j++){

			
			if((g->elts[i][j].val==0)&&(g->elts[i][j].choices.count==min)){
				
				if(make_nth_good_choice==1){
					g->I=i; g->J=j;
				}
				make_nth_good_choice--;
			}
		}
	}

	return min;
}


int sudoku_solve_by_ref(Grid_T* top_g){
	int next_level_choices,initial_choices, i, j, value, 
	/*found,*/ 
	current_level;

	/*INSTEAD OF: min=sudoku_choose_move(top_g);*/
	current_level=top_g->level;
	i=top_g->I; j=top_g->J;
	initial_choices=top_g->elts[top_g->I][top_g->J].choices.count;



	assert((unsigned) j<9);

	/*fprintf(stderr, "SSBR: min=%d i=%d, j=%d\n", initial_choices, i, j );*/
	/* when it succeeds, it succeeds from here, 
	it knows straight away without checking correctness because
	you cannot get here, even from level 80 unless you are correct, so it 
	returns 1 and the only case this fails is if it is given a wrong
	puzzzle with no zeroes this will be post-corrected*/
	if (current_level==TOTAL_SQUARES){
		/*top_g->unique+=  */                                                        /*?????????????????den exei pollh shmasia, isws valw edw pou thelw na epistrefei*/
		return 1;
	}
	/*SOS gia UNIQUENESS:
	mia lush me G->unique=4557 leei sthn solve oti:
	me arxikh sun8hkh history[46] exei lush monadikhs epiloghs
	me arxikh sun8hkh history[57] exei monadikh lush
	G->unique=5300 shmainei oti einai monadikh apo ton xrono pou arxise*/
	if (initial_choices==1){											/*thelw sto grid pou paradidw sto parapanw level na vlepei apo poio shmeio kai meta 
																h lush einai monadikhs epiloghs, an exw monadikh epilogh alla h lush einai hdh monadikhs 
																epiloghs prin apo mena, den me grafw, me grafw mono (stis ekatontades kai xiliades) an den 
																eixe o prohgoumenos monadikh epilogh enw egw exw, apo mena kai meta h lush einai monadikhs 
																epiloghs mexri kapou na spasei h na ftasw se lush*/
		if(top_g->unique/100==0){
			top_g->unique+=(100*(1+current_level));
		}		
	}
	else{top_g->unique%=100;}
																	/*antistoixa an xalaei se mena h monadikh epilogh, th svhnw*/
	while (top_g->elts[i][j].choices.count){
		/*all backtracking is in here, now i am
		in levels ge of the current, and i dont exit this
		area, unless I have solved the puzzle, or if failed*/

		/*IF YOY HAVE CHOSEN STH ELSE BEFORE, ADD THE BACKTRACKING BIT (IF NOT THERE)*/
		if ((top_g->elts[i][j].choices.count<initial_choices)&&
			(top_g->elts[i][j].choices.num[0]%2==0)){																						
					top_g->elts[i][j].choices.num[0]+=1;	
		}
		value=sudoku_find_value(top_g, 
			top_g->elts[i][j].choices.count, i, j );
		
		next_level_choices=prepare_next_move(top_g, value);
		
		if((next_level_choices>0)&&
			(sudoku_solve_by_ref(top_g+1))){			
			return 1;
		}
		/*WE FAIL!*/
		/*THE PROBLEM HAS BEEN SOLVED!*/
		/*IF NONE OF THOSE JUST CANCEL CHOISE AND MOVE ON*/		
		top_g->elts[i][j].choices.num[value]=0;
		top_g->elts[i][j].choices.count--;	
	}
	/*IF YOU HAVE NO MORE CHOISES AND THE PROB HAS NOT BEEN SOLVED, SAY YOU RE DONE*/
	return 0;		
}
int prepare_next_move(Grid_T* g, int value){
		int next_level_choices, i, j;
		i=g->I; j=g->J;
		*(g+1)=*g;
		(g+1)->elts[i][j].val=value;
		/*I */
		(g+1)->elts[i][j].choices.num[0]=g->elts[i][j].choices.num[0]%2;     	
															/*???EDW H FV EXEI VALEI OLES TIS TIMES POU EXOUN DOKIMASTEI SE AYTO TO GRID ?? 0 or 1*/
		(g+1)->level=g->level+1;
		if(g->elts[i][j].choices.count>1){
			/*I tell him i am the last who had a choice writing my name in his two last decimals of unique*/
			(g+1)->unique=100*(g->unique/100)+g->level+1;
		}

																/*TO KATHORISTIKO, EPITREPEI NA KSEREIS MESA STHN LUSH APO POU KAI META 
																H LUSH SOU EINAI UNIQUE, WHO WAS THE LAST WHO HAD CHOICES???
																ALLA DEN KSEREIS AN EXEIS MONADIKH EPILOGH SE KATHE VHMA APO EKEI KAI META*/
		/*BACKTRACKING*/
		else{;}
		
		apply_to_relevant_squares(g+1, value, i, j, NULL, reduce_choices); /*this doesnt change choices.val[0], there i have written backtracking  */
		
		next_level_choices=sudoku_find_move_coordinates(g+1);			/*I find him his move*/
		/*here we fail SO WE DONT ALLOW HIM TO SEARCH IF WE FAILED*/
		/*if option you passed is wrong 
		and you have no other choice 
		and nobody before you does*/
		if((next_level_choices==0)&&
			(g->elts[i][j].choices.count==1)&&
			(g->unique%100==0)){
			/*write the fail time at history[81]->level to know when they fail */
			((g+TOTAL_SQUARES)- (g->level))->level=g->level+1;
		}
		return next_level_choices;
}

/*to get here a puzzle must be correct*/
Grid_T sudoku_solve(Grid_T copy_g, int action,  int mode, int required_level, int* solved){
	
	int puzzle_level, solution_level,
		 /* k, i, j,*/ 
	unique_choice_level, uniqueness_level;
	

	/*grid_randomness_set(action);*/
	/*if (flag==1){;}*/

	Grid_T history[TOTAL_SQUARES+1];
	Grid_T alternative;
	Grid_T *g;

	puzzle_level=copy_g.level;
	history[puzzle_level]=copy_g;

	if(action+mode>0){											/*ie initialise and find init condition except solve  & recursive*/
		sudoku_init_grid_from_values(history+puzzle_level);
		if(!sudoku_find_move_coordinates(history+puzzle_level)){
			*solved=0;
			return history[puzzle_level];
		}
	}
	

																	/*Next we store the level we start if the search needs to find it
																	not needed up to now
																	den grafoume se kati pou den theloume gt an ksekinaw me ola 0 apla 
																	ksanagrafw ta idia, alliws den epitrepetai na paw sto 0, opote 
																	den xreiazomai na mporw na grafw ekei*/
	history->level=copy_g.level;
								

	/*fprintf(stderr, "puzzle_level=%d\n", puzzle_level);*/
	
	

															/*give the stack to the search*/
	*solved=sudoku_solve_by_ref(history+puzzle_level);
	solution_level=(history+SOLUTION_LEVEL)->level;

	/*POST-CORRECTION*/
	if (*solved==1){
		*solved=sudoku_is_correct(history[solution_level]);
	}
	if(*solved==0){
		return history[solution_level];
	}
	/*fprintf(stderr, "SOLUTION_LEVEL=%d\n", solution_level);*/
	

	if ((action==SOLVE)&&(mode==RECURSIVE)){return history[solution_level];}
	

	g=history+puzzle_level;

	if ((action==SOLVE)&&(mode==INFORMATION)){
		while(g<=history+solution_level){
			fprintf(stderr, 
"level=%d Sudoku[%d][%d]=%d unique_choice_since=%d previous_choice_unresolved=%d choices.count=%d\n", 
				g->level, 
				g->I,g->J, history[solution_level].elts[g->I][g->J].val,
				g->unique/100-1,g->unique%100-1,
				g->elts[g->I][g->J].choices.count ); g++;
		}
		g=history+solution_level;
	}
	
	unique_choice_level=(history[solution_level].unique/100-1);

	unique_choice_level=(unique_choice_level>required_level)?
		
		unique_choice_level  :   required_level;
	
	
	if ((action==GENERATE)&&(mode==UNIQUE_CHOICE)){
		history[unique_choice_level].unique=(unique_choice_level==required_level)?1:0;
		return history[unique_choice_level];	
	}
	
	if (*solved){
		uniqueness_level  = unique_from_level(	history, 
						&alternative  , 
						required_level, 
						mode);
	}
	
	
	if (action==GENERATE){
		history[unique_choice_level].unique=(uniqueness_level==required_level)?1:0;
		return (mode==UNIQUE_CHOICE)? 
			
			history[unique_choice_level] : history[uniqueness_level];
	
	}
	
	
	

	
																					/*   THIS IS PRINTED   */
	/*sudoku_print(stderr,history[puzzle_level]);*/
	/*exit(1);*/

	/*if (*solved==0){
		fprintf(stderr, "\nNO SOLUTION, ONE OF THE EXAMINED PUZZLES:\n" );
		sudoku_print_errors(history[solution_level]);
	}*/
/*	else{*/
		if (unique_choice_level==required_level){
			history[solution_level].unique=UNIQUE_CHOICE;
		}
		else if (uniqueness_level==required_level){
			history[solution_level].unique=UNIQUE_BTR;
		}
		else{fprintf(stderr, "\nONE OF MANY SOLUTIONS:\n");
			alternative.unique=0;
			sudoku_print(stderr,history[solution_level]);
			return alternative;
		}
/*	}*/
		
	return history[solution_level];

} 
int unique_from_level(Grid_T* original, 
		Grid_T* other,
		int required_level, 
		int mode){
		
	int i, j, n, k;
		/*int level, value*/	
	int resolved=0;

	n=original[81].unique%100-1;
	while( n>=required_level){
		if(n<required_level){break;}
		i=original[n].I; j=original[n].J;

		if (mode==INFORMATION) {
			fprintf(stderr, "Resolving uniqueness at level %d..\n", n);
			fprintf(stderr, "Sudoku[%d][%d]=%d\n",i,j,original[81].elts[i][j].val);
		}
			
		/*assert(original[n].elts[i][j].choices.count);*/
		for(k=1; k<10; k++){
			if((original[n].elts[i][j].choices.num[k])
							&&
				(k!=original[81].elts[i][j].val)){
				if (mode==INFORMATION){fprintf(stderr, "alternative=%d\t", k);}
				*other=original[n];
				other->elts[i][j].val=k;
				sudoku_init_grid_from_values(other);
				if(sudoku_find_move_coordinates(other)){
					*other=sudoku_solve(
						*other, SOLVE, RECURSIVE, TOTAL_SQUARES, &resolved);
				}
				
			
									
				if (resolved==1){
					other->I=i; other->J=j;
					if(mode==INFORMATION){
						fprintf(stderr, "\talternative found\n");
						sudoku_print(stderr, *other);
					}
					
					return n+1;
				}
				if (mode==INFORMATION){fprintf(stderr, "\tresolved\n");}

			}
			
		}
		if(mode==INFORMATION){fprintf(stderr, "sudoku unique from level %d\n",n );}
		/*fprintf(stderr, "n=%d original[n].unique%100-1=%d\n required_level=%d\n", n,original[n].unique%100-1, required_level);*/
		if (n<=required_level){
			*other= original[SOLUTION_LEVEL];
			if(mode==INFORMATION){fprintf(stderr, "Uniqueness resolved: UNIQUE SOLUTION LEVEL %d\n", required_level);}
			return required_level;
		}
		n=original[n].unique%100-1;
		
	}
	*other= original[SOLUTION_LEVEL];

	if(mode==INFORMATION){fprintf(stderr, "Uniqueness resolved: UNIQUE SOLUTION LEVEL %d\n", required_level);}
	return required_level;
		/* *other=original[n];
		

		
		*other=sudoku_solve(*other,SOLVE, RECURSIVE, 81 , &resolved);*/
	/*	if ((resolved==1)&&(other->elts[i][j].val!=value)){
			return n+1;
		}*/
		

}
Grid_T sudoku_generate_solutions(int mode, int required_level, double* params, int* attempts ){
	
	Grid_T g1;
	Grid_T g2;
	Grid_T empty;
	int puzzle_type, i,j, k;
	
	int ATTEMPTS=GEN_ATTEMPTS_SUGGESTED;
	int levels[GEN_ATTEMPTS_SUGGESTED];

	
	k=0; 
	puzzle_type=UNIQUE_CHOICE;
	
    	for (i=0; i<SIDE; i++){
		for (j=0; j<SIDE; j++){
			empty.elts[i][j].val=0;
		}
	}
	
	
	g2.level=TOTAL_SQUARES+1;
	while (k < ATTEMPTS ){
		/*this is NOT a reason to have a generate action in solve, 
		one could do this just with a silent mode that also initialises herself*/
		g1=sudoku_solve(empty, GENERATE , UNIQUE_CHOICE, TOTAL_SQUARES, &i);
		/*printf("%d\n",g1.level);*/
		if(i){
			
			/*retro_solution(&g1, GENERATE_UNIQUE_CHOICE_DESPERATE, required_level, &puzzle_type);*/
			retro_solution(&g1, mode, required_level, &puzzle_type);
			levels[k]=g1.level;
		}
		else{
			levels[k]=81;
		}
		if (i&&(g1.level<g2.level)){
			g2=g1;
			g2.unique=puzzle_type;
		}
		/*changed, to be changed back*/
		if (g2.level<=required_level){
			k++;
			break;
		}
		k++;

	}
/*	attempts=k;*/ /*estimation*/

	*attempts=k;
	mean_var_estimation(levels, k, params);
	


	/*THE NEXT 3LINES I JUST COMMENT OUT. GENERATE_UNIQUE_CHOICE_DESPERATE INTERNAL MODE,
	WHEN DEFAULT FAILS, IS SWITCHED ON TO ALLOW SEARCH FOR UNIQUE CHOICE PUZZLES IN THE REGION OF
	20-28 ENTRIES. BUT THIS SOMETIMES GIVES STACK SMASHING DECTECTED -ABORTED IN MY LAPTOP
	AND HERE IT ALMOST ALLWAYS GIVES A SEGFAULT!!! SO 30 PIECE EAZY PUZZLES AND NOT 20, WILL BE OK
	4 THE MOMENT, AND ALSO VERY QUICK TO GENERATE <1SEC)*/
	
	if ((mode==UNIQUE_CHOICE)&&(g2.level>required_level)){
		g2=sudoku_generate_solutions(
			GENERATE_UNIQUE_CHOICE_DESPERATE, required_level, params, attempts);
	}


	switch(puzzle_type){
		case UNIQUE_CHOICE: 
			g2.unique=UNIQUE_CHOICE; 
			break;
		default: 
			g2.unique=UNIQUE_BTR;
			break;
	}
	
	/*fprintf(stderr, "N=%d nonzero elements, in %d total generating attempts\n", g2.level, k);
	
	fprintf(stderr, "mean and variance estimation: E[N]=%f VAR[N]=%f\n", params[MEAN], params[VAR]);*/
	

	return g2;

}


void mean_var_estimation(int* levels, int attempts, double* params){
	int sum, sqsum, *level;
	assert(levels && params && attempts);
	sum=0;
	sqsum=0;
	level=levels;

	while(level-levels<attempts){
		sum+=*level;
		sqsum+=(*level)*(*level);
		level++;
	}
	params[MEAN]=sum/(double) attempts;
	params[VAR]=sqsum/(double)(attempts)- (params[MEAN]*params[MEAN]);
	return;
}
