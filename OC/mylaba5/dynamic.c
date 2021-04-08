#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define check(VALUE, MSG, BADVAL) if (VALUE == BADVAL) { perror(MSG); exit(1); }

int main() {
	char* libnames[] = {"./libfunctions1.so", "./libfunctions2.so"};
	int lib = 0;
	int (*GCD)(int, int) = NULL;
	float (*mysquare)(float, float) = NULL;

	void *handle;
	handle = dlopen(libnames[lib], RTLD_NOW);
	check(handle, dlerror(), NULL)

	int f;
	int a, b;
	float x, y;
	while (scanf("%d", &f) > 0) {
		if (f == 0) {
			if (dlclose(handle) != 0) {
				perror(dlerror());
				exit(1);
			}
			lib = (lib + 1) % 2;
			handle = dlopen(libnames[lib], RTLD_NOW);
			check(handle, dlerror(), NULL);

		} else if (f == 1) {
			if (scanf("%d %d", &a, &b) != 2) {
				perror("invalid input");
				exit(1);
			}
			GCD = (int (*)(int, int))dlsym(handle, "mygcd");
			//PrimeCount = (int (*)(int, int))dlsym(handle, "PrimeCount");
			check(GCD, dlerror(), NULL);
			printf("GCD: %d\n", (*GCD)(a, b));
		
		} else if (f == 2) {
			if (scanf("%f %f", &x, &y) != 2) {
				perror("invalid input");
				exit(1);
			}
			mysquare = (float (*)(float, float))dlsym(handle, "mysquare");
			check(mysquare, dlerror(), NULL);
			printf("mysquare = %f\n", (*mysquare)(x, y));
		}
	}
	if (dlclose(handle) != 0) {
		perror(dlerror());
		exit(1);
	}
}
