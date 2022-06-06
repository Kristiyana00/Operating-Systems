#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int pf[2];
	if (pipe(pf) < 0){
		err(1, "Couldn't create pipe!");
	}

	pid_t p1 = fork();

	if (p1 < 0){
		err(2, "Fork error.");
	} else if (p1 == 0){
		close(pf[0]);
		if((dup2(pf[1],1)) == -1){
			err(3, "Couldn't create duplicate!");
		}
		if((execlp("cut", "cut", "-d", ":", "-f7", "/etc/passwd", (char*) NULL)) == -1){
			err(4, "Exec error with command cut!");
		}	
	}

	if (wait(NULL) == -1){
		err(5, "Couldn't wait first child.");
	}

	close(pf[1]);

	int pf2[2];
	if (pipe(pf2) < 0){
		err(6, "Couldn't create pipe!");
	}

	pid_t p2 = fork();
	if (p2 < 0){
		err(7, "Fork error!");
	} else if (p2 == 0){
		if ((dup2(pf[0], 0)) == -1){
			err(8, "Couldn't create duplicate!");
		}
		close(pf2[0]);
		if ((dup2(pf2[1], 1)) == -1){
			err(9, "Couldn't create duplicate!");
		}
		if((execlp("uniq", "uniq", "-c", (char*) NULL)) == -1){
			err(10, "Exec error with command uniq!");
		}
	}

	if (wait(NULL) == -1){
		err(11, "Couldn't wait second child!");
	}

	close(pf2[1]);

	if ((dup2(pf2[0],0)) == -1){
		err(12, "Couldn't create duplicate!");
	}

	if ((execlp("sort", "sort", "-n", (char*) NULL)) == -1){
		err(13, "Exec error with command sort!");
	}
	
	exit(0);
}
