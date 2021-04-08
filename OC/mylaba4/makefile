FLAGS = -std=gnu11 -pedantic 

all: a.out child.out

a.out: main1.c
	gcc $(FLAGS) main.c -o a.out

child.out: child.c
	gcc $(FLAGS) child.c -o child.out

clean:
	rm *.out
	