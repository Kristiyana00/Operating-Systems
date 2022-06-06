#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	while(1){
		if ((write(1, "Enter a command: ", 17)) != 17){
			err(1, "Couldn't write prompt name.");
		}

		char cmd[10];
		ssize_t read_size;

		if ((read_size = read(0, &cmd, sizeof(cmd))) == -1){
			err(2, "Error while reading command from STDIN.");
		}

		cmd[read_size - 1] = '\0';

		if (strcmp("exit", cmd) == 0){
			break;
		}

		pid_t p = fork();

		if (p < 0){
			warn("Fork error for %s", cmd);
		} else if (p == 0) {
			if ((execlp(cmd, cmd, (char*) NULL)) == -1){
				err(3, "Exec error with command: %s", cmd);
			}
		}

		if (wait(NULL) == -1){
			err(4, "Couldn't wait %s", cmd);
	}
	}
	
	exit(0);
}
