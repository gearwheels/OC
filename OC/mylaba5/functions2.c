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

