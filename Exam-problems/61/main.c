#include<stdlib.h>
#include<err.h>
#include<errno.h>
#include<stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>

uint32_t power ( uint32_t num , uint32_t expo){
        for(uint32_t i=0;i<expo;i++){
                num*=num;
        }

        return num;
}

int main (int argc, char* argv[])
{
        if (argc != 2){
                errx(1, "Usage: %s <filename>", argv[0]);
        }

        struct reader {
                uint32_t uid;
                uint16_t reserved1;
                uint16_t reserved2;
                uint32_t start;
                uint32_t end;
        }__attribute__((packed));

        struct reader rd;

        struct stat st;

        if (stat(argv[1], &st) == -1){
                err(2, "Couldn't stat file: %s", argv[1]);
        }

        if (st.st_size == 0){
                errx(3, "%s is empty.");
        }

        if (st.st_size % sizeof(struct reader) != 0){
                errx(4, "%s is corrupted.");
        }

        int fd = open(argv[1], O_RDWR);

        if (fd == -1){
                err(5, "Couldn't open file: %s", argv[1]);
        }

        uint32_t num_members = st.st_size / sizeof(rd);

        uint32_t sum_time = 0;

        while (read(fd, &rd, sizeof(rd)) == sizeof(rd)){
                sum_time = sum_time + (rd.end - rd.start);
        }

        if (lseek(fd, 0, SEEK_SET) == -1){
                int olderrno = errno;
                close(fd);
                errno = olderrno;
                err(6, "ERRROR: lseek failed.");
        }

        uint32_t avg = sum_time / num_members;
        uint32_t disp = 0;

        while (read(fd, &rd, sizeof(rd)) == sizeof(rd)){
                disp = disp + ( power((rd.end - rd.start) - avg, 2));
        }

        disp = disp / num_members;

        if (lseek(fd, 0, SEEK_SET) == -1){
                int olderrno = errno;
                close(fd);
                errno = olderrno;
                err(7, "ERROR: lseek failed.");
        }

        while (read(fd, &rd, sizeof(rd)) == sizeof(rd)){
                uint32_t result = power(rd.end - rd.start, 2);

                if (result > disp){
                        uint32_t uid = rd.uid;
                        if (write(1, &uid, sizeof(uid)) != sizeof(uid)){
                                int olderrno = errno;
                                close(fd);
                                errno = olderrno;
                                err(8, "Couldn't write.");
                        }
                }
        }

        close(fd);
}
