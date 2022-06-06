#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>

int main(int argc, char* argv[]){
	if (argc != 5){
		errx(1, "Usage: %s <filename> <filename> <filename> <filename>", argv[0]);
	}

	struct stat st;

	if (stat(argv[1], &st) == -1){
		err(2, "Failed to stat file: %s", argv[1]);
	}
	
	if (st.st_size == 0){
		errx(3, "%s is empty!", argv[1]);
	}

	if (st.st_size % sizeof(uint8_t) != 0){
		errx(4, "%s is corrupted!", argv[1]);
	}
	
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1){
		err(5, "Failed to open %s.", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1){
		int olderrno = errno;
		close(fd1);
		errno = olderrno;
		err(6, "Failed to open %s.", argv[2]);
	}
	
	int out1 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (out1 == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(7, "Failed to open %s.", argv[3]);
	}

	int out2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (out2 == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		close(out1);
		errno = olderrno;
		err(8, "Failed to open %s.", argv[4]);
	}

	struct str {
		uint16_t offset;
		uint8_t length;
		uint8_t saved;
	}__attribute__((packed));

	struct str s;
	ssize_t read_size;
	uint8_t byte;
	ssize_t read_size2;
	uint8_t counter = 0;
	struct str input;

	while ((read_size = read(fd2, &s, sizeof(s))) == (ssize_t)(sizeof(s))){
		lseek(fd1, s.offset, SEEK_SET);
		if ((read_size2 = read(fd1, &byte, sizeof(byte))) != (ssize_t)(sizeof(byte))){
			int olderrno = errno;
			close(fd1);
			close(fd2);
			close(out1);
			close(out2);
			errno = olderrno;
			err(9, "Failed to read %s.", argv[1]);
		}

		if (byte >= 0x41 && byte <= 0x5A){
			input.offset = lseek(out1, 0, SEEK_CUR);
			write(out1, &byte, sizeof(byte));
			counter = 1;
			while ((read_size2 = read(fd1, &byte, sizeof(byte))) == (ssize_t)(sizeof(byte))){
				if(write(out1, &byte, sizeof(byte)) != sizeof(byte)){
					err(10, "Failed to write in %s.", argv[3]);
				}
				counter++;
				if (counter == s.length){
					input.length = s.length;
					input.saved = s.saved;
					if (write(out2, &input, sizeof(input)) != sizeof(input)){
						err(11, "Failed to write in %s.", argv[4]);
					}
					break;
				}
			}

			if (read_size2 == -1){
				int olderrno = errno;
				close(fd1);
				close(fd2);
				close(out1);
				close(out2);
				errno = olderrno;
				err(12, "Failed to read from %s.", argv[1]);
			}
		}
	}

	if (read_size == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		close(out1);
		close(out2);
		errno = olderrno;
		err(13, "Failed to read from %s.", argv[2]);
	}

	close(fd1);
	close(fd2);
	close(out1);
	close(out2);

	exit(0);
}
