all: tetris

tetris: tetris.c graphics.c
	gcc -o tetris tetris.c graphics.c game.c -lSDL -lSDL_gfx -lSDL_image

clean:
	rm tetris
