#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/mman.h"
#include "string.h"
#include "signal.h"
#include "fcntl.h"

#define check(VALUE, MSG, BADVAL) if (VALUE == BADVAL) { perror(MSG); exit(-1); }

int main(int agrc, char const *argv[]) {
	size_t pagesize = sysconf(_SC_PAGESIZE);
	char c = '\0';
	char c1 = '\0';
	char* str = (char *)malloc(sizeof(char) * pagesize);
	check(str, "malloc error", NULL)

	int mfileAC = open(argv[1], O_RDWR);
	check(mfileAC, "open error", -1)
	int pid = atoi(argv[2]);

	char* fmapAC = (char *)mmap(NULL, pagesize, PROT_READ, MAP_SHARED, mfileAC, 0);
	check(fmapAC, "mmap error", MAP_FAILED)
	size_t i = 0;

	int mfileBC = open(argv[3], O_RDWR);
	check(mfileBC, "open error", -1)

	char* fmapBC = (char *)mmap(NULL, pagesize, PROT_WRITE | PROT_READ, MAP_SHARED, mfileBC, 0);
	check(fmapBC, "mmap error", MAP_FAILED)

	sigset_t set;
	check(sigemptyset(&set), "sigemptyset error", -1)
	check(sigaddset(&set, SIGUSR1), "sigaddset error", -1)
	check(sigprocmask(SIG_BLOCK, &set, NULL), "sigprocmask error", -1)
	int sig;

	for(;;) {
		check(sigwait(&set, &sig), "sigwait error", -1);
        printf("C:\t");
		for (i = 0; i < pagesize; ++i) {
			c = fmapAC[i];
			if (c != '\n' && c != '\0') {
                printf("%c", c);
			} else if (c == '\0') {
				break;
			} else {
				printf("\t");
				break;
			}
		}
		for (i = 0; i < pagesize; ++i) {
			c = fmapBC[i];
			if (c != '\n' && c != '\0') {
                printf("%c", c);
			} else if (c == '\0') {
				break;
			} else {
				printf("%c", c);
				break;
			}
		}
		if (c == '\0' && c1 == '\0') { 
			break;
		} else {
			check(kill(pid, SIGUSR1), "send signal to parent error", -1)
		}
	}
	free(str);
	check(munmap(fmapAC, pagesize), "munmap error", -1)
	check(munmap(fmapBC, pagesize), "munmap error", -1)
	close(mfileAC);
	close(mfileBC);
}
