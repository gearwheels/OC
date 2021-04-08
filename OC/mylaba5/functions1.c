#include "functions.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int mygcd(int a, int b) {
	while (b) {
		a %= b;
		int tmp = a;
		a = b;
		b = tmp;
	}
	return a;
}

float mysquare(float x, float y) {
	return x*y;
}

/*float E(int x) {
	return powf(1 + 1.0 / x, x);
}

#define max(a, b) ((a) > (b)) ? (a) : (b)

int PrimeCount(int A, int B) {
	int n = 0, prime;
	for (int num = max(A, 2); num <= B; ++num) {
		prime = 1;
		for (int i = 2; i < num; ++i) {
			if (num % i == 0) {
				prime = 0;
				break;
			}
		}
		if (prime == 1) {
			++n;
		}
	}
	return n;
}
*/


