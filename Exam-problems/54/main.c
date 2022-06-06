#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
	int option = 0;
	int counter = 1;
	int fd;
	
	if (strcmp(argv[1], "-n") == 0){
		option = 1;
	}

	for (int i = 0 + option; i < argc - 1; i++){
		if (strcmp(argv[i+1], "-") == 0){
			fd = 0;
		} else {
			fd = open(argv[i+1], O_RDONLY);
			if (fd == -1){
				err(1, "Failed to open file: %s", argv[i+1]);
			}
		}
		char c;
		ssize_t read_size;

		if (option == 1){
			setbuf(stdout, NULL);
			printf("%d. ", counter);
		}

		while ((read_size = read(fd, &c, sizeof(c))) == (ssize_t)(sizeof(c))) {
			if (c == '\n' && (read_size = read(fd, &c, sizeof(c)) != 0)){
				write(1, "\n", 1);
				counter++;
				if (option == 1){
					setbuf(stdout, NULL);
					printf("%d. ", counter);
				}
				if (write(1, &c, sizeof(c)) != sizeof(c)){
					err(2, "Failed to write in STDOUT!");
				}
			} else {
				if (write(1, &c, sizeof(c)) != sizeof(c)){
					err(3, "Failed to write in STDOUT!");
				}
			}
		}
		close(fd);
	}

	exit(0);

}
