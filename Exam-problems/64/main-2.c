#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

int main (int argc, char* argv[])
{
        if (argc != 4){
                errx(1, "Usage: %s <3 filenames>", argv[0]);
        }

        struct stat st1;
        struct stat st2;

        if (stat(argv[1], &st1) == -1){
                err(2, "Couldn't stat file: %s", argv[1]);
        }

        if (st1.st_size == 0){
                errx(3, "%s is empty.", argv[1]);
        }

        if (st1.st_size % sizeof(uint8_t) != 0){
                errx(4, "%s is corrupted.", argv[1]);
        }

        if (stat(argv[2], &st2) == -1){
                err(5, "Couldn't stat file: %s", argv[2]);
        }

        if (st2.st_size == 0){
                errx(6, "%s is empty.", argv[2]);
        }

        if (st2.st_size % sizeof(uint16_t) != 0){
                errx(7, "%s is corrupted.", argv[2]);
        }

        if ((st2.st_size / sizeof(uint16_t)) != st1.st_size * 8){
                errx(8, "Invalid form of .sdl file.");
        }

        int fd1 = open(argv[1], O_RDONLY);
        if (fd1 == -1){
                err(9, "Couldn't open file: %s", argv[1]);
        }

        int fd2 = open(argv[2], O_RDONLY);
        if (fd2 == -1){
                int olderrno = errno;
                close(fd1);
                errno = olderrno;
                err(10, "Couldn't open file: %s", argv[2]);
        }

        int out = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

        if (out == -1){
                int olderrno = errno;
                close(fd1);
                close(fd2);
                errno = olderrno;
                err(11, "Couldn't open file: %s", argv[3]);
        }

        uint8_t byte;
        uint16_t n;

        ssize_t read_size, read_size2;

        while ((read_size = read(fd1, &byte, sizeof(byte))) == (ssize_t)(sizeof(byte))){
                for(int i = 1; i <= 8; i++){
                        if ((read_size2 = read(fd2, &n, sizeof(n))) != (ssize_t)(sizeof(n))){
                                int olderrno = errno;
                                close(fd1);
                                close(fd2);
                                close(out);
                                errno = olderrno;
                                err(12, "Couldn't read from file: %s", argv[2]);
                        }

                        if (byte & (1 << (i-1))){
                                if (write(out, &n, sizeof(n)) != sizeof(n)){
                                        int olderrno = errno;
                                        close(fd1);
                                        close(fd2);
                                        close(out);
                                        errno = olderrno;
                                        err(13, "Couldn't write to file: %s", argv[3]);
                                }
                        }
                }


        }

        if (read_size == -1){
                int olderrno = errno;
                close(fd1);
                close(fd2);
                close(out);
                errno = olderrno;
                err(14, "Couldn't read.");
        }

        close(fd1);
        close(fd2);
        close(out);

        exit(0);
}
