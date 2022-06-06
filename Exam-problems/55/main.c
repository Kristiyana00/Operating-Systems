#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
	if (argc != 4){
		errx(1, "Usage: %s <patch.bin> <f1.bin> <f2.bin>");
	}

	struct triple {
		uint16_t offset;
		uint8_t originalbyte;
		uint8_t newbyte;
	}__attribute__((packed));
	
	struct stat st;

	if (stat(argv[1], &st) == -1){
		err(2, "Failed to use stat with file: %s", argv[1]);
	}

	if (st.st_size % sizeof(struct triple) != 0){
		err(3, "Corrupted file: %s", argv[1]);
	}

	if (stat(argv[2], &st) == -1){
		err(4, "Failed to use stat with file: %s", argv[2]);
	}

	if (st.st_size > 0xFFFF) {
		warnx("%s file size greater than 0xFFFF", argv[2]);
	}
	
	int fd1, fd2, fd3;

	if ((fd1 = open(argv[1], O_RDONLY)) == -1){
		err(5, "Failed to open file: %s", argv[1]);
	}	

	if ((fd2 = open(argv[2], O_RDONLY)) == -1){
		int olderrno = errno;
		close(fd1);
		errno = olderrno;
		err(6, "Failed to open file: %s", argv[2]);
	}

	if ((fd3 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(7, "Failed to open file: %s", argv[3]);
	}
	
	uint8_t byte;
	ssize_t read_size, write_size;

	while ((read_size = read(fd2, &byte, sizeof(byte))) > 0 && ((write_size = write(fd3, &byte, sizeof(byte))) > 0)) { }

	if ((read_size != 0) || (write_size != 0)){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		close(fd3);
		errno = olderrno;
		err(8, "Error reading/writing %s -> %s", argv[2], argv[3]);
	}

	close(fd2);

	struct triple tr;
	
	while (read(fd3, &tr, sizeof(tr)) == sizeof(tr)) {
		if (lseek(fd3, tr.offset, SEEK_SET) == -1){
			int olderrno = errno;
			close(fd1);
			close(fd3);
			errno = olderrno;
			err(9, "Cannot seed to %d in %s", tr.offset, argv[3]);
		}

		read(fd3, &byte, sizeof(byte));
		if (byte == tr.originalbyte){
			if (lseek(fd3, -1, SEEK_CUR) == -1){
				int olderrno = errno;
				close(fd1);
				close(fd3);
				errno = olderrno;
				err(10, "Cannot seek to %d in %s", tr.offset, argv[3]);
			}

			write(fd3, &(tr.newbyte), sizeof(tr.newbyte));
		} else {
			int olderrno = errno;
			close(fd1);
			close(fd3);
			errno = olderrno;
			err(11, "There is no such byte in %s", argv[3]);
		}
	}

	close(fd1);
	close(fd3);
	exit(0);
}
