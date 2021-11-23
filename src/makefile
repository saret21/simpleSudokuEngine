CFLAGS= -ansi -pedantic -Wall

sudoku: sudoku.o main.o solve.o grid.o print.o
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	-rm *.o

solve: sudoku
	./sudoku -g | ./sudoku

solveu: sudoku
	./sudoku -g -u | ./sudoku
