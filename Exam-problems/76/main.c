#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main (int argc, char* argv[])
{
        if (argc != 2){
                errx(1, "Usage: %s <filename>", argv[0]);
        }

        int fd = open(argv[1], O_RDONLY);

        if (fd == -1){
                err(2, "Couldn't open file: %s", argv[1]);
        }

        struct stat st;

        struct triple {
                char file[8];
                uint32_t offset;
                uint32_t length;
        }__attribute__((packed));

        if (stat(argv[1], &st) == -1){
                int olderrno = errno;
                close(fd);
                errno = olderrno;
                err(3, "Couldn't stat file: %s", argv[1]);
        }

        if (st.st_size == 0){
                int olderrno = errno;
                close(fd);
                errno = olderrno;
                errx(4, "%s is empty.", argv[1]);
        }

        if ((st.st_size % sizeof(struct triple) != 0) || (st.st_size / sizeof(struct triple))> 8){
                int olderrno = erno;
                close(fd);
                errno = olderrno;
                errx(5, "Wrong format.");
        }

        int read_status;
        uint16_t result = 0, child_result = 0;
        struct triple tr;

        while ((read_status = read(fd, &tr, sizeof(tr))) == sizeof(tr)){
                int pf[2];

                if (pipe(pf) < 0){
                        int olderrno = errno;
                        close(fd);
                        errno = olderrno;
                        err(6, "Couldn't create pipe.");
                }

                pid_t p = fork();

                if (p < 0){
                        int olderrno = errno;
                        close(fd);
                        close(pf[0]);
                        close(pf[1]);
                        errno = olderrno;
                        err(7, "Couldn't fork.");
                } else if (p == 0){
                        close(pf[0]);

                        int fd_f = open(tr.file, O_RDONLY);
                        if (fd_f == -1){
                                int olderrno = errno;
                                close(fd);
                                errno = olderrno;
                                err(8, "Couldn't open file.");
                        }

                        struct stat st1;

                        if (stat(fd_f, &st1) == -1){
                                int olderrno = errno;
                                close(fd);
                                close(fd_f);
                                errno = olderrno;
                                err(9, "Couldn't stat file.");
                        }

                        if (st1.st_size == 0){
                                int olderrno = errno;
                                close(fd);
                                close(fd_f);
                                errno = olderrno;
                                err(10, "File is empty.");
                        }

                        if (st1.st_size % sizeof(uint16_t) != 0){
                                int olderrno = errno;
                                close(fd);
                                close(fd_f);
                                errno = olderrno;
                                err(11, "Wrong format.");
                        }

                        if (lseek(fd_f, tr.offset * sizeof(uint16_t), SEEK_SET) == -1){
                                int olderrno = errno;
                                close(fd);
                                close(fd_f);
                                errno = olderrno;
                                err(12, "Lseek failed.");
                        }

                        uint16_t xor_result = 0;

                        for (uint32_t i = 0; i < tr.length; i++){
                                uint16_t elem;

                                if ((read(fd_f, &elem, sizeof(elem)) != sizeof(elem))){
                                        int olderrno = errno;
                                        close(fd);
                                        close(fd_f);
                                        errno = olderrno;
                                        err(13, "Reading failed.");
                                }

                                xor_result = xor_result ^ elem;
                        }

                        if ((write(pf[1], &xor_result, sizoef(xor_result))) != sizeof(xor_result)){
                                int olderrno = errno;
                                close(fd);
                                close(fd_f);
                                close(pf[1]);
                                errno = olderrno;
                                err(14, "Failed writing.");
                        }

                        close(fd_f);
                        close(pf[0]);

                        exit(0);
                }

                close(pf[1]);

                if ((read(pf[0], &child_result, sizeof(child_result))) != sizeof(child_result)){
                        int olderrno = errno;
                        close(fd);
                        close(pf[0]);
                        errno = olderrno;
                        err(15, "Couldn't read.");
                }

                result = result ^ child_result;

                close(pf[0]);
        }

        if (read_status == -1){
                int olderrno = errno;
                close(fd);
                errno = olderrno;
                err(16, "Couldn't read.");
        }

        printf("result: %x\n", result);

        close(fd);

        exit(0);
}
