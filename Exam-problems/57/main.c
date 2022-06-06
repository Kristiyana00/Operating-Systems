#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
	if (argc != 3){
		errx(1, "Usage: %s [OPTION] SET! [SET2]");
	}

	if (strcmp(argv[1], "-d") == 0){
		char c;
		char* set2 = (char*)malloc(strlen(argv[2]) + 1);
		bool isFound = false;
		ssize_t w;

		strcpy(set2, argv[2]);
		while(read(0, &c, sizeof(c)) == sizeof(c)){
			isFound = false;
			for(size_t i = 0; i < strlen(set2); i++){
				if (c == set2[i]) {
					isFound = true;
					break;
				}
			}
			if (isFound == false){
				if ((w = write(1, &c, sizeof(c))) < 0){
					free(set2);
					err(1, "Error while writing.");
				}
			}
		}
		free(set2);
	} else if (strcmp(argv[1], "-s") == 0){
		char c;
		char set1;
		bool isFound = false;
		ssize_t w;
		set1 = argv[2][0];

		while(read(0, &c, sizeof(c)) == sizeof(c)){
			if (c == set1 && isFound == false){
				isFound = true;
				if ((w = write(1, &c, sizeof(c))) < 0) {
					err(2, "Error while writing.");
				}
			} else if (c != set1 && isFound == true){
				isFound = false;
				if ((w = write(1, &c, sizeof(c))) < 0){
					err(3, "Error while writing.");
				}
			} else if (c != set1 && isFound == false){
				if ((w = write(1, &c, sizeof(c))) < 0) {
					err(4, "Error while writing.");
				}
			} else if (c == set1 && isFound == true) continue;
		}
	} else {
		char* set1 = (char*)malloc(strlen(argv[1])+1);
		char* set2 = (char*)malloc(strlen(argv[2])+1);
		char change;
		char c;
		bool isFound = false;

		ssize_t w;
		strcpy(set1, argv[1]);
		strcpy(set2, argv[2]);

		if (strlen(set1) != strlen(set2)){
			free(set1);
			free(set2);
			errx(5, "SET1 and SET2 are not with equal length!");
		}

		while (read(0, &c, sizeof(c)) == sizeof(c)){
			isFound = false;
			for(size_t i = 0; i < strlen(set2); i++){
				if(c == set1[i]){
					isFound = true;
					change = set2[i];
					break;
				}
			}
			if (isFound == false) {
				if ((w = write(1, &c, sizeof(c)) < 0)){
					free(set1);
					free(set2);
					err(6, "Error while writing.");
				}
			}
			else {
				if ((w = write(1, &change, sizeof(change))) < 0){
					free(set1);
					free(set2);
					err(7, "Error while writing.");
				}
			}
		}
		free(set1);
		free(set2);
	}

	exit(0);
}
