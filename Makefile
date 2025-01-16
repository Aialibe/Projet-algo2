CFLAGS=-Wall -pedantic -std=c99 -fsanitize=address,undefined

build: demineur.c
	gcc -Wall -Wextra -fsanitize=address,undefined -g -o demineur ELT.c ARBRE.c demineur.c -lm

