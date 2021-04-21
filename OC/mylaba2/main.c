#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(){
	char fn1[256];
	char fn2[256];
	if (scanf("%s", fn1) <= 0){
		perror("scanf error");
		return -1;
	}
	if (scanf("%s", fn2) <= 0){
		perror("scanf error");
		return -1;
	}
	FILE* file1 = fopen(fn1, "wt");
	if (file1 == NULL){
		perror("fopen error");
		return -1;
	}
	FILE* file2 = fopen(fn2, "wt");
	if (file2 == NULL){
		perror("fopen error");
		return -1;
	}
	int fd1[2];
	int fd2[2];
	if ((pipe(fd1) == -1) || (pipe(fd2) == -1)){
		printf("Error creating pipe!");
		return 1;
	}

	int id1 = fork();// первый дочерний процесс

	if (id1 == -1)	{
		perror("fork error");
		return -1;
	}else if (id1 > 0){// родительский процесс

		int id2 = fork();// второй дочерний процесс

		if (id2 == -1){
			perror("fork error");
			return -1;
		}else if (id2 > 0){	// родительский процесс
			close(fd1[0]);
			close(fd2[0]);
			fclose(file1);
			fclose(file2);
			char c;
			int n = 1;
			while (getchar() != '\n');
			while (scanf("%c", &c) > 0){// заполняем pipes
				if (c != '\n'){
					if (n % 2 != 0){
						write(fd1[1], &c, sizeof(char));
					}else{
						write(fd2[1], &c, sizeof(char));
					}
				}else{
					if (n % 2 != 0){
						write(fd1[1], &c, sizeof(char));
					}else{
						write(fd2[1], &c, sizeof(char));
					}
					n++;
				}
			}
			close(fd1[1]);
			close(fd2[1]);
		}else{// второй дочерний процесс
			fclose(file1);
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[1]);
			if (dup2(fd2[0], fileno(stdin)) == -1){
				perror("dup2 error");
				return -1;
			}
			if (dup2(fileno(file2), fileno(stdout)) == -1){
				perror("dup2 error");
				return -1;
			}
			fclose(file2);
			char * const * null = NULL;
			if (execv("child.out", null) == -1){
				perror("execv error");
				return -1;
			}

		}
	}else{// первый дочерний процесс
		fclose(file2);
		close(fd2[0]);
		close(fd2[1]);
		close(fd1[1]);
		if (dup2(fd1[0], fileno(stdin)) == -1){
			perror("dup2 error");
			return -1;
		}
		if (dup2(fileno(file1), fileno(stdout)) == -1){
			perror("dup2 error");
			return -1;
		}
		fclose(file1);
		char * const * null = NULL;
		if (execv("child.out", null) == -1){
			perror("execv error");
			return -1;
		}
	}
	return 0;
}