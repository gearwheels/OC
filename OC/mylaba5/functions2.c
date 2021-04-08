#include "functions.h"

#include <stdlib.h>
#include <stdio.h>



int mygcd(int x, int y) {
	if (x > y) {
		int tmp = x;
		x = y;
		y = tmp;
	}
	for (int i = x; i > 1; --i) {
		if (x % i == 0 && y % i == 0) {
			return i;
		}
	}
	return 1;
}

float mysquare(float x, float y) {
	return (x*y)/2;
}

/*
int factorial(int x) {
	int result = 1;
	for (int i = 2; i <= x; ++i) {
		result *= i;
	}
	return result;
}

float E(int x) {
	float result = 0;
	for (int n = 0; n <= x; ++n) {
		result += 1.0 / factorial(n);
	}
	return result;
}

int PrimeCount(int A, int B) {
	int n = 0;
	int *sieve = (int *)malloc(sizeof(int) * (B+1));
	if (sieve == NULL) {
		perror("malloc error");
		exit(1);
	}
	for (int i = 0; i <= B; ++i) sieve[i] = 0;
	for (int i = 2; i <= B; ++i) {
		if (sieve[i] != 0) {
			continue;
		}
		for (int j = i + i; j <= B; j += i) {
			sieve[j] = 1;
		}
	}
	for (int i = A; i <= B; ++i) {
		if (sieve[i] == 0) {
			++n;
		}
	}
	return n;
}
*/