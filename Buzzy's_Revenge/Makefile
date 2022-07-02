all: compile link

compile:
	g++ -Isrc/include -c code/main.cpp

link:
	g++ main.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	