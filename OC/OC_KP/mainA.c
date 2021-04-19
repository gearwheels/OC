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

	char mmfilename1AC[] = "mmfile1AC";
	char mmfilename2AB[] = "mmfile2AB";
	char mmfilename3BC[] = "mmfile3BC";
	int mmf1AC = open(mmfilename1AC, O_RDWR | O_CREAT, S_IRWXU);
	check(mmf1AC, "open 1 error", 0)
	int mmf2AB = open(mmfilename2AB, O_RDWR | O_CREAT, S_IRWXU);
	check(mmf2AB, "open 2 error", 0)
	int mmf3BC = open(mmfilename3BC, O_RDWR | O_CREAT, S_IRWXU);
	check(mmf3BC, "open 3 error", 0)
	size_t i1 = 0, i2 = 0;

	ftruncate(mmf1AC, pagesize);
	ftruncate(mmf2AB, pagesize);
	ftruncate(mmf3BC, pagesize);


	int id1 = fork();// первый дочерний процесс 

	if (id1 == -1)  {
		perror("fork 1 error");
		return -1;
		
	}else if (id1 > 0){// родительский процесс
		int id2 = fork();// второй дочерний процесс

		if (id2 == -1)  {
			perror("fork 2 error");
			return -1;

		}else if (id2 > 0){// родительский процесс
			

			char *fmap1 = (char *)mmap(NULL, pagesize, PROT_WRITE | PROT_READ, MAP_SHARED, mmf1AC, 0);
			check(fmap1, "mmap 1 error", MAP_FAILED)
			char *fmap2 = (char *)mmap(NULL, pagesize, PROT_WRITE | PROT_READ, MAP_SHARED, mmf2AB, 0);
			check(fmap2, "mmap 2 error", MAP_FAILED)

			sigset_t set;
			check(sigemptyset(&set), "sigemptyset error", -1)
			check(sigaddset(&set, SIGUSR1), "sigaddset error", -1)
			check(sigprocmask(SIG_BLOCK, &set, NULL), "sigprocmask error", -1)
			int sig;

			char c;
			int n = 0;
			while (scanf("%c", &c) > 0){// заполняем pipes
				
				if (c != '\n'){
					++n;
					fmap2[i2] = c;
					if (++i2 == pagesize) {
						i2 = 0;
						check(kill(id2, SIGUSR1), "send signal to B.out 2 error", -1)
						check(sigwait(&set, &sig), "sigwait error", -1)
					}
				}else{
					fmap2[i2] = c;
					i2 = 0;
					check(kill(id2, SIGUSR1), "send signal to B.out 2 error", -1)
					check(sigwait(&set, &sig), "sigwait error", -1)
						
					sprintf(fmap1,"%d %c",n,'\n');
					i1 = 0;
					n = 0;
					check(kill(id1, SIGUSR1), "send signal to C.out 1 error", -1)
					check(sigwait(&set, &sig), "sigwait error", -1)
				}
			}
			c = '\0';
			fmap1[0] = c;
			fmap2[0] = c;
			check(kill(id1, SIGUSR1), "send signal to C.out 1 error", -1)
			check(kill(id2, SIGUSR1), "send signal to B.out 2 error", -1)
			check(munmap(fmap1, pagesize), "munmap error", -1);
			check(munmap(fmap2, pagesize), "munmap error", -1);
			close(mmf1AC);
			close(mmf2AB);
			close(mmf3BC);
		}else{// второй дочерний процесс
			close(mmf1AC);

			char spid[10];
			snprintf(spid, 10, "%d", pid);
			char* const args[] = {"B.out", mmfilename2AB, spid, mmfilename3BC, (char *)NULL};
			check(execv("B.out", args), "execv B.out 2 error", -1)
		}

	}else{// первый дочерний процесс
		close(mmf2AB);

		char spid[10];
		snprintf(spid, 10, "%d", pid);
		char* const args[] = {"C.out", mmfilename1AC, spid, mmfilename3BC, (char *)NULL};
		check(execv("C.out", args), "execv C.out 1 error", -1)
	}
	return 0;
}
