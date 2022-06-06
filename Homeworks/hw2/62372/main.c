#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

int main(int argc, char* argv[])
{
	if (argc != 2){
		errx(1, "Usage: %s <program.orc>", argv[0]);
	}

	struct stat st;
	
	if (stat(argv[1], &st) == -1){
		err(2, "Failed to use stat with file: %s", argv[1]);
	}

	if (st.st_size == 0){
		errx(3, "Empty file: %s", argv[1]);
	}
	
	struct header {
		char magic[3];
		uint32_t ram_size;
	}__attribute__((packed));

	struct header hd;
	
	int fd = open(argv[1], O_RDONLY);

	if (fd == -1){
		err(4, "Failed to open file: %s", argv[1]);
	}

	ssize_t read_size;

	if ((read_size = read(fd, &hd.magic, sizeof(hd.magic))) != sizeof(hd.magic)){
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(5, "Failed to read from file: %s", argv[1]);
	}

	if (read_size == -1){
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(6, "ERROR: reading from file %s", argv[1]);
	}
	char tocmp[4];
	strcpy(tocmp, hd.magic);
	if (strcmp(tocmp, "ORC") != 0){
		errx(7, "ERROR: wrong format of magic word.");
	}	

	if ((read_size = read(fd, &hd.ram_size, sizeof(hd.ram_size))) != sizeof(hd.ram_size)){
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(5, "Failed to read from file: %s", argv[1]);
	}

	if (read_size == -1){
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(6, "ERROR: reading from file %s", argv[1]);
	}	
	
	printf("RAM: %x\n", hd.ram_size);
	//Initializing of memmory:
	int64_t mem[hd.ram_size];

	memset( mem, 0, hd.ram_size*sizeof(int64_t) );
	
	int szmem = sizeof(mem);
	printf("Mem size: %d\n", szmem);	

	/*ssize_t wr_status;
	if ((wr_status = write(1, &hd, sizeof(hd))) != sizeof(hd)){
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(7, "ERROR: writing to STDOUT.");
	}

	if (wr_status == -1){
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(8, "ERROR: writing to STDOUT.");
	}*/

	//printf("RAM SIZE: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(hd.ram_size));
	
	struct instruction {
		uint8_t opcode;
		int64_t op1;
		int64_t op2;
		int64_t op3;
	}__attribute((packed));	

	struct instruction instr;

	/*if ((read_size = read(fd, &instr, sizeof(instr))) != sizeof(instr)){
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(8, "Failed to read from file: %s", argv[1]);
	}*/

	while ((read_size = read(fd, &instr, sizeof(instr))) == (ssize_t)sizeof(instr)){
		if (instr.opcode == 0x00){
			printf("Name: nop\n");
			continue;
		} else if (instr.opcode == 0x95){
			printf("Name: set\n");

			/*int pid = fork();
			
			if (pid == -1){
				err(8, "ERROR: fork failed.");
			} if (pid == 0){
				if((execlp("set", "set", instr.op1, instr.op2, (char*) NULL)) == -1){
					int olderrno = errno;
					close(fd);
					errno = olderrno;
					err(9, "ERROR: execlp for [set] failed.");
				}
			}

			if (wait(NULL) == -1){
				err(10, "Couldn't wait for child.");
			}*/
		} else if (instr.opcode == 0x5d){
			printf("Name: load\n");
			memcpy(&instr.op2, &instr.op1, sizeof(instr.op2));
		} else if (instr.opcode == 0x63){
			printf("Name: store\n");

		} else if (instr.opcode == 0x91){
			printf("Name: jmp\n");

		} else if (instr.opcode == 0x25){
			printf("Name: sgz\n");

		} else if (instr.opcode == 0xAD){
			printf("Name: add\n");
	
		} else if (instr.opcode == 0x33){
			printf("Name: mul\n");

		} else if (instr.opcode == 0x04){
			printf("Name: div\n");
		
		} else if (instr.opcode == 0xB5){
			printf("Name: mod\n");

		} else if (instr.opcode == 0xC1){
			printf("Name: out\n");

		} else if (instr.opcode == 0xBF){
			printf("Name: sleep\n");

		} else {
			int olderrno = errno;
			close(fd);
			errno = olderrno;
			//ERROR todo
			err(99, "ERROR: wrong opcode.");
		}
	}

	if (read_size == -1){
		int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(9, "Failed to read from file: %s", argv[1]);
	}

	//printf("\nOpcode: %x,\nOp1: %lx,\nOp2: %lx,\nOp3: %lx\n", instr.opcode, instr.op1, instr.op2, instr.op3);

	close(fd);

	exit(0);
}
