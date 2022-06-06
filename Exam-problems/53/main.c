#include <err.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
	if (argc != 4){
		errx(1, "Usage: %s <f1> <f2> <f3>");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1){
		err(2, "Failed to open %s.", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1){
		int olderrno = errno;
		close(fd1);
		errno = olderrno;
		err(3, "Failed to open %s.", argv[2]);
	}
	
	struct stat st;

	if (stat(argv[1], &st) == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(4, "Failed to use stat with file: %s", argv[1]);
	}

	if (st.st_size == 0){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		errx(5, "Empty file: %s", argv[1]);
	}

	if (st.st_size % sizeof(uint8_t) != 0){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(6, "Corrupted file: %s", argv[1]);
	}

	if (st.st_size > 0xFFFF){
		warnx("Size of %s is greater than 0xFFFF", argv[1]);
	}

	struct stat st2;

	if (stat(argv[2], &st2) == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(7, "Failed to use stat with file: %s", argv[2]);
	}

	if (st2.st_size == 0){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		errx(8, "Empty file: %s", argv[2]);
	}

	if (st2.st_size % sizeof(uint8_t) != 0){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(9, "Corruped file: %s", argv[2]);
	}

	if (st2.st_size > 0xFFFF){
		warnx("Size of %s is greater than 0xFFFF", argv[2]);
	}

	if (st2.st_size != st.st_size) {
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		errx(10, "Files don't have equal lengths.");
	}

	struct triple {
		uint16_t offset;
		uint8_t originalbyte; //On this position in file f1.bin
		uint8_t newbyte; //On this position in file f2.bin
	}__attribute__((packed));

	int patch = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (patch == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(11, "Failed to open %s.", argv[3]);
	}

	struct triple tr;
	ssize_t read_size1, read_size2;
	uint8_t byte1, byte2;

	while ((read_size1 = read(fd1, &byte1, sizeof(byte1))) == (ssize_t)(sizeof(byte1)) && 
(read_size2 = read(fd2, &byte2, sizeof(byte2))) == (ssize_t)(sizeof(byte2))){
		if (byte1 != byte2){
			tr.offset = lseek(fd1, -1*sizeof(byte1), SEEK_CUR);
			if (lseek(fd1, 1*sizeof(byte1), SEEK_CUR) == -1){
				int olderrno = errno;
				close(fd1);
				close(fd2);
				close(patch);
				errno = olderrno;
				err(12, "Error with lseek.");
			}
			tr.originalbyte = byte1;
			tr.newbyte = byte2;

			if (write(patch, &tr, sizeof(tr)) != sizeof(tr)){
				int olderrno = errno;
				close(fd1);
				close(fd2);
				close(patch);
				errno = olderrno;
				err(13, "Failed to write in file: %s", argv[3]);
			}
		}
	}

	if (read_size1 == -1 || read_size2 == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		close(patch);
		errno = olderrno;
		err(14, "Failed to read from files: %s or %s", argv[1], argv[2]);
	}

	close(fd1);
	close(fd2);
	close(patch);

	exit(0);
}
