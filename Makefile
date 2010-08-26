CC_FLAGS = -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/SDL -Wall -O2
LD_FLAGS = -lSDL_gfx -lSDL_image -lSDL_ttf -lSDL -lpthread

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

