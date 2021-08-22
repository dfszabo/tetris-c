build:
	gcc -std=c99 -Wall ./src/*.c -o Tetris -O3

debug:
	gcc -std=c99 -Wall ./src/*.c -o Tetris -g

clean:
	rm Tetris
