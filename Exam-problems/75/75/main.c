#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char* argv[])
{
	if (argc != 3){
		errx(1, "Usage: %s <input.lfld> <output.bin>", argv[0]);
	}

	struct stat st;

	if (stat(argv[1], &st) == -1){
		err(2, "Failed to use stat with file: %s", argv[1]);
	}

	if (st.st_size == 0){
		errx(3, "Empty file: %s", argv[1]);
	}

	if (st.st_size % sizeof(uint8_t) != 0){
		err(4, "Corrupted file: %s", argv[1]);
	}

	int output = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (output == -1){
		err(5, "Failed to open file: %s", argv[2]);
	}

	int pf[2];
	if (pipe(pf) < 0){
		int olderrno = errno;
		close(output);
		errno = olderrno;
		err(6, "Couldn't create pipe.");
	}

	pid_t p = fork();

	if (p < 0){
		int olderrno = errno;
		close(output);
		errno = olderrno;
		err(7, "ERROR: fork failed.");
	} else if (p == 0){
		close(pf[0]);
		dup2(pf[1], 1);
		if(execlp("cat", "cat", argv[1], (char*) NULL) == -1){
			int olderrno = errno;
			close(output);
			errno = olderrno;
			err(8, "ERROR: exec cat failed.");
		}
	}

	wait(NULL);
	close(pf[1]);
	dup2(pf[0],0);

	uint8_t byte;
	ssize_t read_size;
	int next = 0;

	uint8_t original_byte;

	while((read_size = read(0, &byte, sizeof(byte))) == (ssize_t)i(sizeof(byte))){
		if (next == 1){
			if (byte == (0x7D^0x20) || byte == (0x00^0x20) || byte == (0xFF^0x20) || byte == (0x55^0x20)) {
				original_byte = byte^0x20;
				next = 0;
				write(output, &original_byte, sizeof(original_byte));
		}
		if (byte == 0x7D) {
			next = 1;
		} else {
			next = 0;
		}
	
		}
	}
	close(output);

	exit(0);
}
