#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main (int argc, char* argv[])
{
        if (argc != 2){
                errx(1, "Incorrect number of arguments given.");
        }

        int fd = open("/tmp/fifo", O_RDONLY);

        dup2(fd, 0);

        if(execl(argv[1], argv[1], (char*) NULL) == -1){
                err(2, "Exec failed.");
        }

        exit(0);
}

