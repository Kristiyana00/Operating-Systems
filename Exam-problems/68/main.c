#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	if (argc != 2){
		errx(1, "Usage: %s <filename>", argv[0]);
	}

	int pf[2];

	if (pipe(pf) == -1){
		err(2, "Couldn't create pipe.");
	}

	pid_t p1 = fork();

	if (p1 < 0){
		err(3, "Fork error.");
	} else if (p1 == 0) {
		close(pf[0]);
		if((dup2(pf[1], 1)) < 0){
			err(4, "Couldn't create duplicate.");
		}
		if ((execlp("cat", "cat", argv[1], (char*) NULL)) == -1){
			err(5, "Exec error with command cat.");
		}
	}

	wait(NULL);
	close(pf[1]);

	if((dup2(pf[0],0)) < 0){
		err(6, "Couldn't create duplicate.");
	}

	if ((execlp("sort", "sort", (char*)NULL)) == -1){
		err(7, "Exec error with command sort.");
	}

	exit(0);
}
