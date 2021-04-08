#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/mman.h"
#include "string.h"
#include "signal.h"
#include "fcntl.h"
#include <sys/types.h>

#define check(VALUE, MSG, BADVAL) if (VALUE == BADVAL) { perror(MSG); exit(1); }

int main() {
	size_t pagesize = sysconf(_SC_PAGESIZE);

	int pid = getpid();

	char fn1[256];
	char fn2[256];
	if (scanf("%s", fn1) <= 0) {
		perror("scanf error");
		return -1;
	}
	if (scanf("%s", fn2) <= 0) {
		perror("scanf error");
		return -1;
	}
	getchar();

	FILE* file1 = fopen(fn1, "wt");
	check(file1, "fopen 1 error", NULL)
	FILE* file2 = fopen(fn2, "wt");
	check(file2, "fopen 2 error", NULL)

	char mmfilename1[] = "mmfile1";
	char mmfilename2[] = "mmfile2";
	int mmf1 = open(mmfilename1, O_RDWR | O_CREAT, S_IRWXU);
	check(mmf1, "open 1 error", 0)
	int mmf2 = open(mmfilename2, O_RDWR | O_CREAT, S_IRWXU);
	check(mmf2, "open 2 error", 0)
	size_t i1 = 0, i2 = 0;

	ftruncate(mmf1, pagesize);
	ftruncate(mmf2, pagesize);

	int id1 = fork();

	if (id1 == -1)  {
		perror("fork 1 error");
		return -1;
		
	} else if (id1 == 0) {
		fclose(file2);
		close(mmf2);
		check(dup2(fileno(file1), fileno(stdout)), "dup2 error", -1)
		fclose(file1);

		char spid[10];
		snprintf(spid, 10, "%d", pid);
		char* const args[] = {"child.out", mmfilename1, spid, (char *)NULL};
		check(execv("child.out", args), "execv child.out 1 error", -1)

	} else {
		int id2 = fork();

		if (id2 == -1)  {
			perror("fork 2 error");
			return -1;

		} else if (id2 == 0) {
			fclose(file1);
			close(mmf1);
			check(dup2(fileno(file2), fileno(stdout)), "dup2 error", -1)
			fclose(file2);

			char spid[10];
			snprintf(spid, 10, "%d", pid);
			char* const args[] = {"child.out", mmfilename2, spid, (char *)NULL};
			check(execv("child.out", args), "execv child.out 2 error", -1)

		} else {
			fclose(file1);
			fclose(file2);

			char *fmap1 = (char *)mmap(NULL, pagesize, PROT_WRITE | PROT_READ, MAP_SHARED, mmf1, 0);
			check(fmap1, "mmap 1 error", MAP_FAILED)
			char *fmap2 = (char *)mmap(NULL, pagesize, PROT_WRITE | PROT_READ, MAP_SHARED, mmf2, 0);
			check(fmap2, "mmap 2 error", MAP_FAILED)

			sigset_t set;
			check(sigemptyset(&set), "sigemptyset error", -1)
			check(sigaddset(&set, SIGUSR1), "sigaddset error", -1)
			check(sigprocmask(SIG_BLOCK, &set, NULL), "sigprocmask error", -1)
			int sig;

			char c;
			int n = 1;
			//while (getchar() != '\n');
			while (scanf("%c", &c) > 0){// заполняем pipes
				if (c != '\n'){
					if (n % 2 != 0){
						fmap1[i1] = c;
						if (++i1 == pagesize) {
							i1 = 0;
							check(kill(id1, SIGUSR1), "send signal to child.out 2 error", -1)
							check(sigwait(&set, &sig), "sigwait error", -1)
						}
					}else{
						fmap2[i2] = c;
						if (++i2 == pagesize) {
							i2 = 0;
							check(kill(id2, SIGUSR1), "send signal to child.out 2 error", -1)
							check(sigwait(&set, &sig), "sigwait error", -1)
						}
					}
				}else{
					if (n % 2 != 0){
						fmap1[i1] = c;
						i1 = 0;
						check(kill(id1, SIGUSR1), "send signal to child.out 1 error", -1)
						check(sigwait(&set, &sig), "sigwait error", -1)
					}else{
						fmap2[i2] = c;
						i2 = 0;
						check(kill(id2, SIGUSR1), "send signal to child.out 2 error", -1)
						check(sigwait(&set, &sig), "sigwait error", -1)
					}
					n++;
				}
			}
			c = '\0';
			fmap1[0] = c;
			fmap2[0] = c;
			check(kill(id1, SIGUSR1), "send signal to child.out 1 error", -1)
			check(kill(id2, SIGUSR1), "send signal to child.out 2 error", -1)
			check(munmap(fmap1, pagesize), "munmap error", -1);
			check(munmap(fmap2, pagesize), "munmap error", -1);
			close(mmf1);
			close(mmf2);
		}
	}
	return 0;
}
