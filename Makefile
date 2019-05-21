all: chess

chess: main.o display.o game.o
	g++ -o chess main.o display.o game.o

main.o: main.cpp
	g++ main.cpp -c -o main.o

display.o: display.cpp
	g++ display.cpp -c -o display.o

game.o: game.cpp
	g++ game.cpp -c -o game.o