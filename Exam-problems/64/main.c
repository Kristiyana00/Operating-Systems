#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

int main (int argc, char* argv[])
{
        if (argc != 4){
                errx(1, "Usage: %s <3 filenames>", argv[0]);
        }

        int fd1 = open(argv[1], O_RDONLY);

        if (fd1 == -1){
                err(2, "Couldn't open file: %s", argv[1]);
        }

        struct stat st1;

        if (stat(argv[1], &st1) == -1){
                int olderrno = errno;
                close(fd1);
                errno = olderrno;
                err(3, "Couldn't stat file: %s", argv[1]);
        }

        if (st1.st_size == 0){
                errx(4, "%s is empty.");
        }

        if (st1.st_size % sizeof(uint8_t) != 0){
                int olderrno = errno;
                close(fd1);
                errno = olderrno;
                err(5, "%s is corrupted.");
        }

        int fd2 = open(argv[2], O_RDONLY);

        if (fd2 == -1){
                int olderrno = errno;
                close(fd1);
                errno = olderrno;
                err(6, "Couldn't open file: %s", argv[2]);
        }

        struct stat st2;

        if (stat(argv[2], &st2) == -1){
                int olderrno = errno;
                close(fd1);
                close(fd2);
                errno = olderrno;
                err(7, "Couldn't stat file: %s", argv[2]);
        }

        if (st2.st_size == 0){
                errx(8, "%s is empty.");
        }

        if (st2.st_size % sizeof(uint16_t) != 0){
                int olderrno = errno;
                close(fd1);
                close(fd2);
                errno = olderrno;
                err(9, "%s is corrupted.", argv[2]);
        }

        if ((st2.st_size / sizeof(uint16_t)) != st1.st_size){
                int olderrno = errno;
                close(fd1);
                close(fd2);
                errno = olderrno;
                err(10, "Invalid format of .sdl file");
        }

        int fd = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

        if (fd == -1){
                int olderrno = errno;
                close(fd1);
                close(fd2);
                errno = olderrno;
                err(11, "Couldn't open file: %s", argv[3]);
        }

        uint8_t bit;
        uint16_t n;

        int r_status1, r_status2;

        while((r_status1 = read(fd1, &bit, sizeof(bit))) == sizeof(bit) && (r_status2 = read(fd2, &n, sizeof(n))) == sizeof(n)){
                if (bit == 1){
                        if ((write(fd, &n, sizeof(n))) != sizeof(n)){
                                int olderrno = errno;
                                close(fd);
                                close(fd1);
                                close(fd2);
                                errno = olderrno;
                                err(12, "Couldn't write to file: %s", argv[3]);
                        }
                } else if (bit != 0){
                        int olderrno = errno;
                        close(fd);
                        close(fd1);
                        close(fd2);
                        errno = olderrno;
                        err(13, "Invalid bit.");
                }
        }

        if (r_status1 == -1 || r_status2 == -1){
                int olderrno = errno;
                close(fd);
                close(fd1);
                close(fd2);
                errno = olderrno;
                err(14, "Reading failed.");
        }

        close(fd1);
    close(fd2);
    close(fd);

    exit(0);
}
