#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "grid.h"
#include "sudoku.h"
#define SIDE 9
#define TOTAL_SQUARES 81
#define SOLUTION_LEVEL 81
#define SOLVE 0
#define GENERATE 50
#define RECURSIVE 0
#define NON_UNIQUE 0
#define UNIQUE_CHOICE 1
#define UNIQUE_BTR 2
#define INFORMATION 3
#define CHECK 4
#define GENERATE_LEVEL 20
#define MEAN 0
#define VAR 1

Grid_T sudoku_solve(Grid_T copy_g, int action,  int mode, int required_level, int* solved);
Grid_T sudoku_generate_solutions(int mode, int required_level , double* params, int* attempts);
void print_help();


int main(int argc, char const *argv[]){
        


        int action;
        int check;
        int unique_choice;
        int info;
        int gen_level;
        int mode; 
        int randomness_level;
        int solved;
        int c;
        double parameters_generation[2];
        int attempts_generation;
        Grid_T g;
        
        
        const char* s1="-g";
        const char* s2="-c";
        const char* s3="-u";
        const char* s4="-r";
        const char* s5="-i";
	
	if(argc==2&&!strcmp(argv[1],"--help")){
		print_help();
		return 0;
	}
        action=SOLVE; 
	mode=CHECK;
        unique_choice=0; 
	gen_level=GENERATE_LEVEL; 
	info=0; 
        randomness_level=40;
	check=0;
	solved=0;
	attempts_generation=0;

        for(c=1;c<argc;c++){
                if (strcmp(argv[c], s1)==0){
                        action=GENERATE;
                        
                }
                if (strcmp(argv[c], s2)==0){
                        check=1;
                }
                if (strcmp(argv[c], s3)==0){
                        unique_choice=1;
                }
                if (strcmp(argv[c], s4)==0){
                        randomness_level=TOTAL_SQUARES; 
                                                /*this will only have effect on
                                                solving, since generating will 
                                                allways be random all the way*/
                }
                if (strcmp(argv[c], s5)==0){
                        
                        info=1; 
                }
                if (strlen(argv[c])==3){
                        if(argv[c][0]=='-'){
                                if (((argv[c][1]=='u')&&(argv[c][2]=='g'))
                                                ||
                                ((argv[c][2]=='g')&&(argv[c][1]=='u'))){
                                        action=GENERATE; unique_choice=1;
                                }
                        }  
                }                
                if (strlen(argv[c])<4&&atoi(argv[c])){

                            if (gen_level==GENERATE_LEVEL){
                                gen_level=atoi(argv[c]);
                            }

                        /*else{
                                fprintf(stderr, "sudoku: invalid input %s\n", 
                                        argv[c]);
                        }*/

                }
                 
        }

        srand(getpid());

        if (gen_level<0){gen_level=-gen_level;}
        gen_level=gen_level%(TOTAL_SQUARES+1);
        
        
        if (check&&(action!=GENERATE)){action=CHECK;}


        if (action==GENERATE){
                
                mode=(unique_choice)? 

                UNIQUE_CHOICE : UNIQUE_BTR;

                randomness_level=TOTAL_SQUARES;
        }
        else{mode=(info)?INFORMATION:CHECK;}

        grid_randomness_set(randomness_level);

       /* fprintf(stderr, "generate=%d\n", generate);*/
        /*fprintf(stderr, "check=%d\n", check);
        fprintf(stderr, "unique_choice=%d\n", unique_choice);
        fprintf(stderr, "gen_level=%d\n", gen_level);
        fprintf(stderr, "randomness_level=%d\n", randomness_level);
        fprintf(stderr, "action=%d\n", action);
        fprintf(stderr, "mode=%d\n",mode );
        fprintf(stderr, "info=%d\n",info );*/
        /*exit(1);*/
        
        switch(action){
                case CHECK:
                g=sudoku_read();
                sudoku_print(stderr,g);
                if(sudoku_is_correct(g)){
                        fprintf(stderr, "SUDOKU IS CORRECT" );
                        if (g.level==TOTAL_SQUARES){
                                fprintf(stderr, " AND ALREADY SOLVED\n" );
                        }
                        else{fprintf(stderr, "\n" );}
                }
                else{
                        fprintf(stderr, "SUDOKU IS INCORRECT:\n" );
                        sudoku_print_errors(g); /*only errors no sudoku*/
                }
                break;

                case SOLVE:
                solved=0;
                g=sudoku_read();
                sudoku_print(stderr,g);
                if (sudoku_is_correct(g)){
                
                        if(mode==INFORMATION){fprintf(stderr, "SUDOKU WAS CORRECT, GIVEN TO BE SOLVED\n" );}
                        g=sudoku_solve(g, SOLVE, mode, g.level , &solved);
                        if (!solved){
                                fprintf(stderr, "NO SOLUTION, ONE OF THE EXAMINED PUZZLES:\n" );
                                
                        }
                        
                        else{
                                if (g.unique==UNIQUE_CHOICE){
                                        fprintf(stderr, "\nUNIQUE CHOICE SOLUTION:\n");
                                }
                                else if (g.unique==UNIQUE_BTR){
                                        fprintf(stderr,"\nUNIQUE SOLUTION BUT NOT ONE WITH UNIQUE CHOICE (NEEDS BACKTRACKING):\n");
                                }
                                else{
                                        fprintf(stderr, "\nALTERNATIVE SOLUTION:\n");
                                        fprintf(stderr, "ONE DIFFERENCE: row %d column %d\n",1+g.I, 1+g.J );
                                }
                        }
                        

                }
                else{
                       fprintf(stderr, "SUDOKU IS INCORRECT:\n" ); 
                }
                sudoku_print(stdout, g);
                sudoku_print_errors(g);
                break;

                case GENERATE:
                
                
		g=sudoku_generate_solutions(mode,  gen_level , parameters_generation, &attempts_generation);
                
                sudoku_print(stdout, g);
                
                switch(g.unique){
                    
                    case UNIQUE_CHOICE: 
                            
                            fprintf(stderr, "UNIQUE CHOICE SOLUTION SUDOKU\n" ); 
                            break;
                            
                    default: 
                    
                            fprintf(stderr, "UNIQUE SOLUTION WITH BACKTRACKING SUDOKU\n" );
                            break;
                }
                fprintf(stderr, "N=%d nonzero elements, in %d total generating attempts\n", g.level, attempts_generation);
                /*mean_var_estimation(levels, k, params);*/
                fprintf(stderr, "mean and variance estimation: E[N]=%f VAR[N]=%f\n", 
                    parameters_generation[MEAN], parameters_generation[VAR]);
                
        }




        return 0;


        /* if (argc==1){sketh sudoku}*/
}
void print_help(){
	int n;
	char* buf[3000];
	const char* h1="readme.txt";
	const char* h2="../readme.txt";
	FILE* man=fopen(h1,"r");
	if(!man){man=fopen(h2,"r");}
	if(!man){
		fprintf(stderr,
			"\noption unavailable, but there was a readme.txt file somewhere..\n");
		return;
	}
	while(1){
		n=fread(buf,1,3000,man);
		fwrite(buf,1,n,stderr);
		if(n<3000){break;}
	}
	return;
}
