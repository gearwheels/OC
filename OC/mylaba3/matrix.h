#ifndef MATFUNC_H
#define MATFUNC_H
	
struct Matrix {
	int dimn;
	int dimm;
	float **matr; 
};

void scanfMatrix(struct Matrix *a, int n, int m) { // Ввод матрицы  while (getchar() != '\n');
	a->dimn = n;
	a->dimm = m;
 	a->matr = (float**)malloc(sizeof(float*)*n);
	for (int i=0; i<n; i++) {
		a->matr[i] = (float*) malloc(sizeof(float)*m);
	}

	for (int i=0; i<n; i++) {
		for (int j=0; j<m; j++) {
			scanf("%f", &a->matr[i][j]);
		}
	}
}

/*int random(int seed) {
    seed * 123456;
}*/


void init_Matr(struct Matrix* a, int n, int m){
    a->dimm = m;
    a->dimn = n;
    a->matr = (float**)malloc(sizeof(float*)*n);
    for (int i = 0; i < n; ++i) {
        a->matr[i] = (float*)malloc(sizeof(float)*m);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            a->matr[i][j] = 0;
        }
    }
}

void printMatrix(struct Matrix *a) { // Печать матрицы
	printf("\n");
	for (int i=0; i<a->dimn; i++) {
		for (int j=0; j<a->dimm; j++) {
			printf("%f ", a->matr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void freematrix(struct Matrix* a){
	for (int i = 0; i < a->dimn; i++){
		free(a->matr[i]);
		a->matr[i] = NULL;
	}
	free(a->matr);
	a->matr = NULL;
}

#endif