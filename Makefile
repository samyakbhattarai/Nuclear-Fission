all:
	gcc -Isrc/Include -Lsrc/lib -o nuclear nuclear.c -lmingw32 -lSDL2main -lSDL2
