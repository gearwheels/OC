#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
	int f;
	int a, b;
	float x, y;
	while (scanf("%d", &f) > 0) {
		if (f == 1) {
			if (scanf("%d %d", &a, &b) != 2) {
				perror("invalid input");
				exit(1);
			}
			printf("GCD: %d\n", mygcd(a, b));
		
		} else if (f == 2) {
			if (scanf("%f %f", &x, &y) != 2) {
				perror("invalid input");
				exit(1);
			}
			printf("mysquare = %f\n", mysquare(x, y));
		}
	}
}