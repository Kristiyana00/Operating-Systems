#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>

int cmp(const void* a, const void* b){
	if( *((uint32_t*) a) > *((uint32_t*) b)){
		return 1;
	}
	else if ( *((uint32_t*) b) > *((uint32_t*)a)){
		return -1;
	}
	return 0;
}

int main(int argc, char** argv){
	
	if (argc != 2){
		errx(1, "Wrong argument! Usage: %s <file> .", argv[0]);	
	}
	
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1){
		err(2, "Error opening file %s", argv[1]);
	}	
	
	struct stat st;
	if (stat(argv[1], &st) == -1){
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(3, "Stat error with file %s", argv[1]);
	}
	
	if (st.st_size == 0){
		errx(4, "Empty file!");
	}	
	if (st.st_size % sizeof(uint32_t) != 0){
		errx(5, "Failed size!");
	}
	uint32_t numel = st.st_size / sizeof(uint32_t);
	uint32_t lhalf = numel / 2;
	uint32_t* firstBytes = malloc(lhalf*sizeof(uint32_t));
	ssize_t read_size;
	if((read_size = read(fd, firstBytes, lhalf*sizeof(uint32_t))) != (ssize_t)(lhalf*sizeof(uint32_t))){
		free(firstBytes);
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(6, "Couldn't read from file %s.", argv[1]);
	}
	qsort(firstBytes, lhalf , sizeof(uint32_t) , cmp);
	
	uint32_t rhalf = numel - lhalf;
	uint32_t* secondBytes = malloc(rhalf*sizeof(uint32_t));
	if((read_size = read(fd, secondBytes, rhalf*sizeof(uint32_t))) != (ssize_t)(rhalf*sizeof(uint32_t))){
		free(firstBytes);
		free(secondBytes);
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(7, "Couldn't read from file %s.", argv[1]);
	}
    qsort(secondBytes, numel - lhalf, sizeof(uint32_t), cmp);	
	int fd2 = open("purvifile.txt", O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
	if (fd2 < 0){
		free(firstBytes);
		free(secondBytes);
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(8, "Could not open file purvifile.txt!");
	}
	int fd3 = open("vtorifile.txt", O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
	if (fd3 < 0){
		free(firstBytes);
		free(secondBytes);
		int olderrno = errno;
		close(fd);
		close(fd2);
		errno = olderrno;
		err(9, "Could not open file vtorifile.txt!");
	}
	ssize_t write_size;
	if ((write_size = write(fd2, firstBytes, lhalf*sizeof(uint32_t))) != (ssize_t)(lhalf*sizeof(uint32_t))) {
		free(firstBytes);
		free(secondBytes);
		int olderrno = errno;
		close(fd);
		close(fd2);
		close(fd3);
		errno = olderrno;
		err(10, "Could not write in file purvifile.txt!");
	}
	
	if ((write_size = write(fd3, secondBytes, rhalf*sizeof(uint32_t))) != (ssize_t)(rhalf*sizeof(uint32_t))){
		free(firstBytes);
        free(secondBytes);
        int olderrno = errno;
        close(fd);
        close(fd2);
        close(fd3);
        errno = olderrno;
		err(10, "Could not write in file vtorifile.txt!");
	}
	free(firstBytes);
	free(secondBytes);
	lseek(fd3, 0, SEEK_SET);
	lseek(fd2, 0, SEEK_SET);
	int fdsort = open("sortedNum.txt", O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
	if (fdsort < 0){
		int olderrno = errno;
		close(fd);
		close(fd2);
		close(fd3);
		errno = olderrno;
		err(11, "Could not open file sortedNum.txt!");
	}
	ssize_t read_size2;
	uint32_t a;
	uint32_t b;
	while ((read_size = read(fd2, &a, sizeof(a))) == (ssize_t)(sizeof(a)) && (read_size2 = read(fd3, &b, sizeof(b))) == (ssize_t)(sizeof(b))){
		if (a < b){
			if(write(fdsort, &a, sizeof(a)) != sizeof(a)){
				int olderrno = errno;
				close(fd);
				close(fd2);
				close(fd3);
				close(fdsort);
				errno = olderrno;
				err(12, "Write error in file sortedNum.txt!");
			}
			lseek(fd3, -1*sizeof(b), SEEK_CUR);
		}
		else if (b < a){
			if(write(fdsort, &b, sizeof(b)) != sizeof(b)){
				int olderrno = errno;
				close(fd);
				close(fd2);
				close(fd3);
				close(fdsort);
				errno = olderrno;
				err(13, "Write error in file sortedNum.txt!");
			}
			lseek(fd2, -1*sizeof(a), SEEK_CUR);
		}
	}
	
	if (read_size == sizeof(a)){
		write(fdsort, &a, sizeof(a));
	}
	
	if (read_size == -1 || read_size2 == -1){
		int olderrno = errno;
		close(fd);
		close(fd2);
		close(fd3);
		close(fdsort);
		errno = olderrno;
		err(14, "Read error from temp files!");
	}
	while((read_size = read(fd2, &a, sizeof(a))) == sizeof(a)){
		write(fdsort, &a, sizeof(a));
	}
	if (read_size == -1){
		err(15, "Read error from first temp file!");
	}
	while((read_size2 = read(fd3, &b, sizeof(b))) == sizeof(b)){
		write(fdsort, &b, sizeof(b));
	}
	if (read_size == -2){
		err(16, "Read error from second temp file!");
	}
	close(fd);
	close(fd2);
	close(fd3);
	close(fdsort);
	exit(0);
}
