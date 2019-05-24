CC= g++
LDLIBS=-lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS=-L/usr/lib/x86_64-linux-gnu/
CPPFLAGS=-I/usr/include/

.PHONY: all clean
all: chess

chess: main.o display.o game.o
	$(CC) -o chess main.o display.o game.o $(LDFLAGS) $(LDLIBS)
	rm -f *.o

%.o: %.cpp
	$(CC) -c $< -o $@  $(LDFLAGS) $(LDLIBS)

clean:
	rm *.o