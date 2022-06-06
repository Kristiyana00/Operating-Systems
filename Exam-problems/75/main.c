#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>


int main (int argc, char* argv[])
{
	if (argc != 3){
		errx(1, "Invalid number of arguments given.");
	}

	int pf[2];

	if (pipe(pf) == -1){
		err(2, "Couldn't create pipe.");
	}

	int pid = fork();

	if (pid == -1){
		err(3, "Couldn't fork.");
	}

	if (pid == 0){
		close(pf[0]);
		close(1);
		dup2(pf[1], 1);
		if (execlp("/bin/cat", "/bin/cat", argv[1], (char*)NULL) == -1){
			err(4, "Couldn't execlp cat.");
		}
	}

	close(pf[1]);

	int fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IWUSR | S_IRUSR);

	if (fd == -1){
		err(6, "Couldn't open file.");
	}

	unsigned char byte;
	int read_status;

	while ((read_status = read(pf[0], &byte, 1)) > 0){
		if (byte == 0x7D){
			if (read(pf[0], &byte, 1) != 1){
				//smqna na errno
				close(fd);
				close(pf[0]);
				err(8, "Couldn't read byte.");
			}
			byte = byte ^ 0x20;
		}

		if (write(fd, &byte, 1) != 1){
			//smqna na errno
			close(fd);
			close(pf[0]);
			err(7, "Couldn't write to file.");
		}
	}

	if (read_status == -1){
		close(fd);
		close(pf[0]);
		err(5, "Read failed.");
	}

	close(fd);
	close(pf[0]);

	exit(0);
}
