#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int cmp (const void* a, const void* b){
        return *((uint16_t*)a) - *((uint16_t*)b);
}

int main (int argc, char* argv[])
{
        if (argc != 3){
                errx(1, "Usage: %s <input.bin> <output.bin>", argv[0]);
        }

        struct stat st;

        if (stat(argv[1], &st) == -1){
                err(2, "Couldn't stat file: %s", argv[1]);
        }

        if (st.st_size == 0){
                errx(3, "%s is empty.", argv[1]);
        }

        if (st.st_size % sizeof(uint16_t) != 0){
                errx(4, "%s is corrupted.", argv[1]);
        }

        int fd = open(argv[1], O_RDONLY);

        if (fd == -1){
                err(5, "Couldn't open file: %s", argv[1]);
        }

        ssize_t count = st.st_size / sizeof(uint16_t);
        uint16_t* buffer = (uint16_t*)malloc(count * sizeof(uint16_t));

        ssize_t read_size;

        if ((read_size = read(fd, buffer, count * sizeof(uint16_t))) != (ssize_t)(count * sizeof(uint16_t))){
                int olderrno = errno;
                close(fd);
                errno = olderrno;
                err(6, "Couldn't read from file: %s", argv[1]);
        }

        close(fd);

        qsort(buffer, count, sizeof(uint16_t), cmp);

        int fd2 = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

        if (fd2 == -1){
                int olderrno = errno;
                free(buffer);
                errno = olderrno;
                err(7, "Couldn't open file: %s", argv[2]);
        }

        if (write(fd2, buffer, count * sizeof(uint16_t)) != count * sizeof(uint16_t)){
                int olderrno = errno;
                close(fd2);
                free(buffer);
                errno = olderrno;
                err(8, "Couldn't write to file: %s", argv[2]);
        }

        close(fd2);
        free(buffer);

        exit(0);

}
