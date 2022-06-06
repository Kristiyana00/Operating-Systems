#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char* argv[]){
        if (argc != 2){
                errx(1, "Incorrect number of arguments given.");
        }

        int fifo_status = mkfifo("/tmp/fifo", S_IRUSR | S_IWUSR);

        if (fifo_status == -1){
                err(2, "Couldn't create FIFO.");
        }

        int fd = open("/tmp/fifo", O_WRONLY);
        if (fd == -1){
                err(4, "Open failed.");
        }

        dup2(fd, 1);

        if (execl("/bin/cat", "/bin/cat", argv[1], (char*) NULL) == -1){
                err(5, "Exec failed.");
        }



        exit(0);
}

