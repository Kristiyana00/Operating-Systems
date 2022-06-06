#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char* argv[])
{
	if (argc != 4){
		errx(1, "Usage: %s <f1> <f2> <f3>", argv[0]);
	}
	
	int fd1, fd2, fd3;

	if ((fd1 = open(argv[1], O_RDONLY)) == -1){
		err(2, "Error opening %s.", argv[1]);
	}

	if ((fd2 = open(argv[2], O_RDONLY)) == -1){
		int olderrno = errno; 
		close(fd1);
		errno = olderrno;
		err(3, "Error opening %s.", argv[2]);
	}

	if ((fd3 = open(argv[3], O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(4, "Error opening %s.", argv[3]);
	}

	struct stat st;
	off_t f1_size;
	off_t f2_size;

	if (stat(argv[1], &st) == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		close(fd3);
		errno = olderrno;
		err(5, "Failed to use stat with file: %s", argv[1]);
	}

	f1_size = st.st_size;

	if (st.st_size == 0){
		err(6, "File is empty!");
	}

	if (stat(argv[2], &st) == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		close(fd3);
		errno = olderrno;
		err(7, "Failed to use stat with file: %s", argv[2]);
	}

	f2_size = st.st_size;

	struct pair_str {
		uint32_t start_pos;
		uint32_t length;
	};

	if (f1_size % sizeof(struct pair_str) != 0){
		err(8, "Failed f1_size.");
	}

	if (f2_size % sizeof(uint32_t) != 0){
		err(9, "Failed f2_size.");
	}

	struct pair_str pair;

	while (read(fd1, &pair, sizeof(pair))){
		if (lseek(fd2, pair.start_pos*sizeof(uint32_t), SEEK_SET) < 0){
			int olderrno = errno; 
			close(fd1);
			close(fd2);
			close(fd3);
			errno = olderrno;
			err(10, "Error with lseek.");
		}

		uint32_t bytes;
		uint32_t index;
		ssize_t read_size;

		while ((read_size = read(fd2, &bytes, sizeof(bytes))) > 0){
			if (write(fd3, &bytes, sizeof(read_size) != read_size)){
				int olderrno = errno;
				close(fd1);
				close(fd2);
				close(fd3);
				errno = olderrno;
				err(11, "Error while writing in: %s", argv[3]);
			}	
			index++;
			if (index == pair.length){
				break;
			}
		}
	}

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}
