#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

//cut -d ":" -f7 /etc/passwd | sort | uniq -c | sort -n

int main (int argc, char* argv[])
{
        int pf[2];

        if (pipe(pf) < 0){
                err(1, "Couldn't create pipe.");
        }

        pid_t p1 = fork();

        if (p1 < 0){
                err(2, "Couldn't fork.");
        } else if (p1 == 0){
                close(pf[0]);

                if ((dup2(pf[1], 1)) == -1){
                        err(3, "Couldn't create duplicate.");
                }

                if ((execlp("cut", "cut", "-d", ":", "-f7", "/etc/passwd", (char*) NULL)) == -1){
                        err(4, "Exec failed with command cut.");
                }
        }

        if (wait(NULL) == -1){
                err(5, "Couldn't wait for first child.");
        }

        close(pf[1]);

        int pf2[2];

        if (pipe(pf2) < 0){
                err(6, "Couldn't create pipe.");
        }

        pid_t p2 = fork();

        if (p2 < 0){
                err(7, "Couldn't fork.");
        } else if (p2 == 0){
                close(pf2[0]);

                if ((dup2(pf[0], 0)) == -1){
                        err(8, "Couldn't create duplicate.");
                }

                if ((dup2(pf2[1], 1)) == -1){
                        err(9, "Couldn't create duplicate.");
                }

                if ((execlp("sort", "sort", (char*) NULL)) == -1){
                        err(10, "Exec failed with command sort.");
                }
        }

        if (wait(NULL) == -1){
                err(11, "Couldn't wait for second child.");
        }

        close(pf2[1]);

        int pf3[2];

        if (pipe(pf3) < 0){
                err(12, "Couldn't create pipe.");
        }

        pid_t p3 = fork();

        if (p3 < 0){
                err(13, "Couldn't fork.");
        } else if (p3 == 0){
                close(pf3[0]);

                if ((dup2(pf2[0], 0)) == -1){
                        err(14, "Couldn't create duplicate.");
                }

                if ((dup2(pf3[1], 1)) == -1){
                        err(15, "Couldn't create duplicate.");
                }

                if ((execlp("uniq", "uniq", "-c", (char*) NULL)) == -1){
                        err(16, "Exec failed with command uniq.");
                }
        }

        if (wait(NULL) == -1){
                err(17, "Couldn't wait for third child.");
        }

        close(pf3[1]);

        if ((dup2(pf3[0], 0)) == -1){
                err(18, "Couldn't create duplicate.");
        }

        if ((execlp("sort", "sort", "-n", (char*) NULL)) == -1){
                err(19, "Exec failed with command sort.");
        }

        exit(0);
}
