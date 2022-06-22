#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

// find ~ -type f -printf "%f %T@\n" | sort -nr -t ' ' -k2 | cut -d ' ' -f 1 | head -n 1

int main (int argc, char* argv[])
{
        if (argc !=  2){
                errx(1, "Usage: %s <dirname>", argv[0]);
        }

        int pf[2];

        if (pipe(pf) < 0){
                err(2, "Couldn't create pipe.");
        }

        pid_t p1 = fork();

        if (p1 < 0){
                err(3, "Couldn't fork.");
        } else if (p1 == 0){
                close(pf[0]);

                if ((dup2(pf[1], 1)) == -1){
                        err(4, "Couldn't create duplicate.");
                }

                if ((execlp("find", "find", argv[1], "-type", "f", "-printf", "%p %T@\n", (char*)NULL)) == -1){
                        err(5, "Exec failed with command find.");
                }
        }

        if (wait(NULL) == -1){
                err(6, "Couldn't wait for first child.");
        }

        close(pf[1]);

        int pf2[2];

        if (pipe(pf2) < 0){
                err(7, "Couldn't create pipe.");
        }

        pid_t p2 = fork();

        if (p2 < 0){
                err(8, "Fork failed.");
        } else if (p2 == 0){
                close(pf2[0]);
                if ((dup2(pf[0], 0)) == -1){
                        err(9, "Couldn't create duplicate.");
                }
                if ((dup2(pf2[1], 1)) == -1){
                        err(10, "Couldn't create duplicate.");
                }

                if ((execlp("sort", "sort", "-nr", "-k2", (char*) NULL)) == -1){
                        err(11, "Exec failed with command sort.");
                }
        }

        if (wait(NULL) == -1){
                err(12, "Couldn't wait for second child.");
        }

        close(pf2[1]);

        int pf3[2];
        if (pipe(pf3) < 0){
                err(13, "Couldn't create pipe.");
        }

        pid_t p3 = fork();

        if (p3 < 0){
                err(14, "Couldn't fork.");
        } else if (p3 == 0){
                close(pf3[0]);
                if ((dup2(pf2[0], 0)) == -1){
                        err(15, "Couldn't create duplicate.");
                }

                if ((dup2(pf3[1], 1)) == -1){
                        err(16, "Couldn't create duplicate.");
                }

                if ((execlp("cut", "cut", "-d", " ", "-f1", (char*) NULL)) == -1){
                        err(17, "Exec failed with command cut.");
                }
        }

        if (wait(NULL) == -1){
                err(18, "Couldn't wait for third child.");
        }

        close(pf3[1]);

        if ((dup2(pf3[0], 0)) == -1){
                err(19, "Couldn't create duplicate.");
        }

        if ((execlp("head", "head", "-n", "1", (char*) NULL)) == -1){
                err(20, "Exec failed with command head.");
        }

        exit(0);

}
