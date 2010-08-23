SDL_LIBS = sdl SDL_gfx SDL_image SDL_ttf
CC_FLAGS = $(shell pkg-config --cflags $(SDL_LIBS)) -Wall -O2
LD_FLAGS = $(shell pkg-config --libs $(SDL_LIBS))

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

all: quatris

quatris: $(OBJECTS)
	gcc $(LD_FLAGS) -o $@ $?

%.o: %.c
	gcc -c $(CC_FLAGS) $<

clean:
	rm -f quatris
	rm -f *.o

