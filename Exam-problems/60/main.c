#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
	if (argc != 3){
		errx(1, "Usage: %s <filename> <filename>");
	}

	struct stat st;

	if (stat(argv[1], &st) == -1){
		err(2, "Couldn't use stat with file: %s", argv[1]);
	}

	if (st.st_size % sizeof(uint16_t) != 0){
		err(3, "Corrupted file: %s", argv[1]);
	}

	if (!(st.st_mode & S_IRUSR)){
		err(4, "%s is not readable.", argv[1]);
	}

	int fdinput = open(argv[1], O_RDONLY);
	if (fdinput == -1){
		err(5, "Failed to open file: %s", argv[1]);
	}

	uint32_t counting[0xffff+1] ={0}; //65535 + 1
	uint16_t buf[1<<10];

	ssize_t read_sz;

	while ((read_sz = read(fdinput, &buf, sizeof(buf))) > 0){
		for(uint16_t i = 0; i< read_sz / sizeof(uint16_t); ++i){
			++counting[buf[i]];
		}

		if (read_sz == -1){
			close(fdinput);
			err(6, "Couldn't read from file: %s", argv[1]);
		}

		int fdoutput = open(argv[2], O_CREAT | O_TRUNC, O_RDWR, 0644);
		if (fdoutput == -1){
			close(fdinput);
			err(7, "Couldn't open file: %s", argv[2]);
		}

		for(uint32_t i = 0; i <= 0xffff; ++i){
			while(counting[i]--){
				if(write(fdoutput, &i, sizeof(uint16_t)) != sizeof(uint16_t)){
					close(fdinput);
					close(fdoutput);
					err(8, "Couldn't write to file: %s", argv[2]);
			}
		}
	}
	close(fdinput);
	close(fdoutput);
	exit(0);
}
