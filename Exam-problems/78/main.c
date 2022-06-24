#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]){
        if (argc != 3){
                errx(1, "Invalid number of arguments.");
        }

        if( argv[1][0] < '0' ||
                argv[1][0] > '9' ||
                argv[2][0] < '0' ||
                argv[2][0] > '9'){
                errx(2, "Invalid type of arguments");
        }

        int N = argv[1][0] - '0';
        int D = argv[2][0] - '0';

        int pf1[2];
        int pf2[2];

        const char* ding = "DING ";
        const char* dong = "DONG\n";
        char signal = 1;

        char a, b;

        pipe(pf1);
        pipe(pf2);

        pid_t p = fork();

        if (p == 0){
                close(pf1[1]);
                close(pf2[0]);
                for (int i = 0; i < N; i++){
                        read(pf1[0], &a, 1);
                        write(1, dong, 5);
                        write(pf2[1], &signal, 1);
                }

                exit(0);
        }

        close(pf1[0]);
        close(pf2[1]);

        for (int i = 0; i < N; i++){
                write(1, ding, 5);
                write(pf1[1], &signal, 1);
                read(pf2[0], &b, 1);
                sleep(D);
        }
}
