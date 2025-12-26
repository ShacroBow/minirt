#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	char file[1000];
	size_t bytes;
	int fd = open("output.ppm", O_RDWR);
	bytes = read(fd, file, 1000);
	file[bytes] = 0;
	write(1, file, bytes);
}