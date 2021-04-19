FLAGS = -std=gnu11 -pedantic 

all: A.out B.out C.out

A.out: mainA.c
	gcc $(FLAGS) mainA.c -o A.out

B.out: B.c
	gcc $(FLAGS) B.c -o B.out

C.out: C.c
	gcc $(FLAGS) C.c -o C.out

clean:
	rm *.out
	