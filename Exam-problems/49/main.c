#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		errx(1, "Invalid number of arguments!");
	}	

	int fd = open(argv[1], O_RDWR);

	if (fd == -1)
	{
		err(2, "The file could not be opened!");
	}

	unsigned int arr[256];

	for(int i = 0; i < 256; i++)
	{
		arr[i]=0;
	}

	unsigned char buff;
	int read_status;

	while ((read_status=read(fd,&buff,1)) > 0)
	{
		++arr[buff];
	}

	if (read_status == -1)
	{
		close(fd);
		err(3, "Problem with reading the file!");
	}

	if (lseek(fd,0,SEEK_SET) == -1)
	{
		close(fd);
		err(4,"Problem with lseek!");
	}

	for (int i = 0; i < 256; i++)
	{
		for (int j = arr[i]; j > 0; --j)
		{
			char value = (char)i;
			if (write(fd, &value, 1) == -1)
			{
				close(fd);
				err(5, "There was a problem with writing to the file!");
			}
		}
	}

	close(fd);
	exit(0);
}
