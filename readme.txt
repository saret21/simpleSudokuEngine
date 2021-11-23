./sudoku [OPTIONS]

xwris options kanei solve, uparxoun tria modes, solve, check kai generate

-g	mpainei se mode generate, to automato mode einai solve

-c	mpainei se mode check

-i	info gia th lush, leitourgei mono se mode solve,
	polu xrhsimo gia na katalaveis ti plhroforia krataw

-u	leitourgei se mode generate, paragei unique choice sudoku, 
	enw xwris auto to option paragei unique solution pou mporei
	omws na xreiazetai backtracking

-23	to 23 einai mono paradeigma mporei na einai opoiosdhpote ari8mos
	leitourgei se mode generate, prospathei na paragei ena puzzle
	me 23 givens, an den ta kataferei dinei ena puzzle me ta ligotera pou vrhke
	ektupwnei epishs mesh timh kai diaspora twn givens stis prospatheies pou ekane
	oi opoies einai 100 (default alla allazei)

--help	ektupwnei auto edw to readme (an einai ston idio fakelo, h ston  ../  )


MODES:
*************************************** solve ********************************************

gia na lusei ena sudoku, paradeigma to s.10 sto fakelo puzzles:

	
	./sudoku <../puzzles/s.10
 

	./sudoku -i <../puzzles/s.10



to option -i ektupwnei information gia to sudoku pou lunetai


************************************** check *********************************************


omoia gia na kanei check ena sudoku kai na entopisei pi8anon lathi:


	./sudoku -c <../puzzles/s.1



************************************ generate ********************************************


generate ena sudoku me 25 stoixeia kai monadikh lush (pou omws xreaizetai backtracking):


	./sudoku -g -25


generate ena sudoku me 25 stoixeia, kai lush monadikhs epiloghs (unique choice):


	./sudoku -g -25 -u 
