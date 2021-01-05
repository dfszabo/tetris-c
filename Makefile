build:
	gcc -std=c99 -Wall ./src/*.c -o Tetris -lSDL2 -O3

debug:
	gcc -std=c99 -Wall ./src/*.c -o Tetris -lSDL2 -g

clean:
	rm Tetris
