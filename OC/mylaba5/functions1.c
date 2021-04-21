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


