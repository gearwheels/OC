FLAGS= -std=c99 -pthread -w -pipe -O3 -Wextra -Werror -Wall -Wno-sign-compare -pedantic -lm
COMPILLER=gcc

all: start

start: main.o
	$(COMPILLER) $(FLAGS) -o lab3 main.o

main.o: main.c
	$(COMPILLER) -c $(FLAGS) main.c