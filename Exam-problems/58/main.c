#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

int cmp(const void* a, const void* b){
        return *((uint32_t*)a) - *((uint32_t*)b);
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

        if (st.st_size % sizeof(uint32_t) != 0){
                errx(4, "%s is corrupted.", argv[1]);
        }

        uint32_t half = (st.st_size / sizeof(uint32_t))/2;

        uint32_t* fhalf = (uint32_t*)malloc(half*sizeof(uint32_t));

        int fd = open(argv[1], O_RDONLY);

        if (fd == -1){
                err(5, "Couldn't open file: %s", argv[1]);
        }

        uint32_t rd;
        if ((rd = read(fd, fhalf, half*sizeof(uint32_t)) != half*sizeof(uint32_t))){
                int olderrno = errno;
                close(fd);
                free(fhalf);
                errno = olderrno;
                err(6, "Couldn't read from file: %s", argv[1]);
        }

        qsort(fhalf, half, sizeof(uint32_t), cmp);

        int t1 = open("tmp1", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

        if (t1 == -1){
                int olderrno = errno;
                close(fd);
                free(fhalf);
                errno = olderrno;
                err(7, "Couldn't open file.");
        }

        if ((rd = write(t1, fhalf, half*sizeof(uint32_t))) != (half*sizeof(uint32_t))){
                int olderrno = errno;
                close(fd);
                free(fhalf);
                errno = olderrno;
                err(8, "Couldn't write.");
        }

        if (lseek(t1, 0, SEEK_SET) == -1){
                int olderrno;
                close(fd);
                free(fhalf);
                errno = olderrno;
                err(9, "Couldn't lseek.");
        }

        free(fhalf);
        uint32_t* shalf= (uint32_t*) malloc(half*sizeof(uint32_t));

        rd=read(fd,shalf,half*sizeof(uint32_t));
        if( rd != half * sizeof(uint32_t)) {
                int old=errno;
                close(fd);
                free(shalf);
                errno=old;
                err(-6,"Coudnt read second half from origi");
        }


        qsort(shalf,half,sizeof(uint32_t),cmp);


        int t2 = open("tmp2", O_CREAT | O_TRUNC | O_RDWR,0660);
        if (t2 == -1 ) {
                int old=errno;
                close(fd);
                free(shalf);
                errno=old;
                err(-7,"Failed to open tmp2");
        }


        rd=write(t2,shalf,sizeof(uint32_t)*half);
        if ( rd != half*sizeof(uint32_t)) {
                int old=errno;
                close(fd);
                close(t1);
                free(shalf);
                errno=old;
                err(-7,"Failed to write to tmp2");

        }

        free(shalf);

        if ( lseek(t2,0,SEEK_SET) == -1 ) {
                int old=errno;
                close(t1);
                close(fd);
                free(shalf);
                errno=old;
                err(-7,"Failed to lseek tmp2");

        }
        close(fd);


        int result=open(argv[2], O_CREAT | O_TRUNC | O_RDWR,0660);

        if( result == -1 ) {
                int old=errno;
                close(t1);
                close(t2);
                errno=old;
                err(-9,"Failed to open result file");
        }

        uint32_t c1;
        uint32_t c2;

        uint32_t i=0;
        uint32_t j=0;

        while( (read(t1,&c1,sizeof(c1)) > 0) && (read(t2,&c2,sizeof(c2)) > 0 ))
        {
                if( c1<=c2) {
                        write(result,&c1,sizeof(c1));

                        lseek(t2,-1*sizeof(c1),SEEK_CUR);
                        i++;
                }
                else {
                        write(result,&c2,sizeof(c2));

                        lseek(t1,-1*sizeof(c2),SEEK_CUR);
                        j++;
                }
        }

        //while(i<half) {
        while(  read(t1,&c1,sizeof(c1))>0){
                write(result,&c1,sizeof(c1));
                i++;
        }

        //while(j<half) {
        while(  read(t2,&c2,sizeof(c2)) > 0) {
                write(result,&c2,sizeof(c2));
                j++;
        }

        close(result);
        close(t1);
        close(t2);
}
