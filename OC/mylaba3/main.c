#include <stdio.h>
#include "matrix.h"
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <float.h>


//n - строка
// m - столбец

struct Matrix erM;
struct Matrix dilM;

struct thr_data{
	int numstr; // массив номеров строк которые мы обрабатываем
	float out; // массив для ответов из ф-ий  matrixMin  matrixMax
};

void matrixMin(struct thr_data *tmpdata){
    tmpdata->out = FLT_MAX;  // MIN
    for(int i = 0; i < erM.dimm; i++){
        if (tmpdata->out > erM.matr[tmpdata->numstr][i]) { 
            if (erM.matr[tmpdata->numstr][i] == 0){
                erM.matr[tmpdata->numstr][i] += (rand() % 1000);
                erM.matr[tmpdata->numstr][i] /= 10000;
            }
            tmpdata->out = erM.matr[tmpdata->numstr][i];
        }
    }
}

void matrixMax(struct thr_data *tmpdata){
    tmpdata->out = -(FLT_MAX); // MAX
    for(int i = 0; i < dilM.dimm; i++){
        if (tmpdata->out < dilM.matr[tmpdata->numstr][i]) { 
            if (dilM.matr[tmpdata->numstr][i] == 0){
                dilM.matr[tmpdata->numstr][i] += (rand() % 1000);
                dilM.matr[tmpdata->numstr][i] /= 10000;
            }
            tmpdata->out = dilM.matr[tmpdata->numstr][i];
        }
    }
}

void erosion(struct thr_data *tmpdata){ // struct thr_data *tmpdata передает минимум и номер строки которую нужно обработать
    for(int i = 0; i < erM.dimm; i++){
        erM.matr[tmpdata->numstr][i] -= tmpdata->out;
    }
}

void dilatation(struct thr_data *tmpdata){
    for(int i = 0; i < dilM.dimm; i++){
        dilM.matr[tmpdata->numstr][i] += tmpdata->out;
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int k, n, m;

int main(int argc, char const *argv[]) {
    unsigned int max_threads = 1;
    if (argc > 1 && atoi(argv[1]) > 0) {
        max_threads = atoi(argv[1]);
    }

    printf("Threads %d\nEnter size of matrix. Enter N and M\n ", max_threads);
    fflush(stdout);
    scanf("%d %d", &n, &m);
    fflush(stdout);

    if (n == 0 || m == 0) {
        printf("I cannot create matrix with this n and m\n");
        return 0;
    }
    
    scanfMatrix(&erM, n, m); 
    init_Matr(&dilM,n,m);

    for(int q = 0; q < erM.dimn; q++){ // копируем значения из введенной матрицы во 2-ю 
        for(int l = 0; l < erM.dimm; l++){
            dilM.matr[q][l] = erM.matr[q][l];
        }
    }

    printf("Matrix for erosion\n");
    printMatrix(&erM);
    printf("Matrix for dilatation\n");
    printMatrix(&dilM);
    
    fflush(stdout);

    printf("Enter your K\n ");
    fflush(stdout);
    scanf("%d", &k);

    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * max_threads);

    struct thr_data arr_thr_data[n];
    for(int i = 0; i < n; ++i){
        arr_thr_data->numstr = 0;
        arr_thr_data->out = 0;
    }
    double start, end;
    start = clock();

//  erosion     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    int count = 0;
    while(count < k) {
        count++;
        float max = -(FLT_MAX);
        float min = FLT_MAX;
        for (unsigned int i = 0; i < n; i += max_threads){
            for (unsigned int j = 0; j < max_threads; j++){
                if (i + j >= n) {//чтобы не запускался под конец новый процесс
                    break;
                }
                arr_thr_data[i + j].numstr = i + j;

                if (pthread_create(&threads[j], NULL, matrixMin, /*(void *) &args[i + j]*/ &arr_thr_data[i+j]) != 0){
                    perror("Cannot create thread\n");
                    return 1;
                }
            }

            //ждем завершения потоков
            for (int j = 0; j < max_threads; ++j){ // пока ждем процессы проверяем массив arr_thr_data на минимум
                if (i + j >= n) {
                    break;
                }
                if (pthread_join(threads[j], NULL)){
                    perror("Thread didn't finished\n");
                    return 1;
                }
                if(arr_thr_data[i + j].out < min){ 
                    min = arr_thr_data[i + j].out;
                }
            }

        }

        for(int i = 0; i < n; i++){ // в arr_thr_data заменяем все out на минимальное значение чтобы потом передать в erosion
            arr_thr_data[i].out = min;
        }

        for (unsigned int i = 0; i < n; i += max_threads){
            for (unsigned int j = 0; j < max_threads; j++){
                if (i + j >= n) {//чтобы не запускался под конец новый процесс
                    break;
                }
                arr_thr_data[i + j].numstr = i + j;

                if (pthread_create(&threads[j], NULL, erosion, /*(void *) &args[i + j]*/ &arr_thr_data[i+j]) != 0){
                    perror("Cannot create thread\n");
                    return 1;
                }
            }

            //ждем завершения потоков
            for (int j = 0; j < max_threads; ++j){ // пока ждем процессы проверяем массив arr_thr_data на минимум
                if (i + j >= n) {
                    break;
                }
                if (pthread_join(threads[j], NULL)){
                    perror("Thread didn't finished\n");
                    return 1;
                }
            }
        }

        //      dilatation      %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        for (unsigned int i = 0; i < n; i += max_threads){
            for (unsigned int j = 0; j < max_threads; ++j){
                if (i + j >= n){//чтобы не запускался под конец новый процесс
                    break;
                }
                arr_thr_data[j].numstr = i + j;
                
                if (pthread_create(&threads[j], NULL, matrixMax, /*(void *) &args[i + j]*/ &arr_thr_data[i+j]) != 0){
                    perror("Cannot create thread\n");
                    return 1;
                }
            }

            //ждем завершения потоков
            for (int j = 0; j < max_threads; ++j){// пока ждем процессы проверяем массив arr_thr_data на максимум
                if (i + j >= n){
                    break;
                }
                if (pthread_join(threads[j], NULL)){
                    perror("Thread didn't finished\n");
                    return 1;
                }
                if(arr_thr_data[i + j].out > max){
                    max = arr_thr_data[i + j].out;
                }
            }

        }

        for(int i = 0; i < n; i++){ // в arr_thr_data заменяем все out на минимальное значение чтобы потом передать в erosion
            arr_thr_data[i].out = max;
        }

        for (unsigned int i = 0; i < n; i += max_threads){
            for (unsigned int j = 0; j < max_threads; ++j){
                if (i + j >= n) {//чтобы не запускался под конец новый процесс
                    break;
                }
                arr_thr_data[i + j].numstr = i + j;

                if (pthread_create(&threads[j], NULL, dilatation, /*(void *) &args[i + j]*/ &arr_thr_data[i+j]) != 0){
                    perror("Cannot create thread\n");
                    return 1;
                }
            }

            //ждем завершения потоков
            for (int j = 0; j < max_threads; j++){ // пока ждем процессы проверяем массив arr_thr_data на минимум
                if (i + j >= n) {
                    break;
                }
                if (pthread_join(threads[j], NULL)){
                    perror("Thread didn't finished\n");
                    return 1;
                }
            }

        }
    }
    end = clock();
    printf("\nITERATION %d\n\n",count);
    printf("Matrix for erosion\n");
    printMatrix(&erM);
    printf("Matrix for dilatation\n");
    printMatrix(&dilM);

    freematrix(&erM);
    freematrix(&dilM);
    free(threads);

    FILE *file = fopen("log", "a");
    FILE *thread = fopen("treads", "a");
    FILE *time = fopen("time", "a");

    double threTime = (end - start) / 1000;

    fprintf(file, "threads = %d\ntime = %fms\n\n", max_threads, (end - start) / 1000);
    fprintf(thread, "%d\n", max_threads);
    fprintf(time, "%f\n", threTime);

    fclose(file);
    fclose(thread);
    fclose(time);

    return 0;
}

