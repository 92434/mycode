#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aio_rw.h"

int test_suspend_read_and_write(int fd, char *buffer, size_t size) {
	int ret;

	bzero(buffer, size);
	sprintf(buffer, "%s", "xiaofei");
	{
		aio aio(fd);
		aio.add_aiocb(0, buffer, strlen(buffer));
		aio.add_aiocb(8, buffer, strlen(buffer));
		aio.attach_aio_to_sigio(0, -1);
		aio.aio_rw_suspend(op_write);
	}

	bzero(buffer, size);
	{
		aio aio(fd);
		aio.add_aiocb(0, buffer, size);
		aio.add_aiocb(8, buffer + strlen("xiaofei"), size - strlen("xiaofei") - 1);
		aio.attach_aio_to_thread_callback(0, -1);
		aio.aio_rw_suspend(op_read);
	}
	printf("read from file:%s\n", buffer);

	return 0;
}

int test_lio_read_and_write(int fd, char *buffer, size_t size) {
	int ret;

	bzero(buffer, size);
	sprintf(buffer, "%s", "xiaofei");
	{
		aio aio(fd);
		aio.add_aiocb(16, buffer, strlen(buffer), true, LIO_WRITE);
		aio.add_aiocb(24, buffer, strlen(buffer), true, LIO_WRITE);
		aio.attach_aio_to_sigio(0, -1);
		aio.aio_rw_listio();
	}

	bzero(buffer, size);
	{
		aio aio(fd);
		aio.add_aiocb(16, buffer, size, true, LIO_READ);
		aio.add_aiocb(24, buffer + strlen("xiaofei"), size - strlen("xiaofei") - 1, true, LIO_READ);
		aio.attach_aio_to_thread_callback(0, -1);
		aio.aio_rw_listio();
	}
	printf("read from file:%s\n", buffer);

	return 0;
}

int main(int argc, char **argv)
{
	int fd = open("file.txt", O_RDWR | O_CREAT);
	if (fd < 0) {
		perror("open");
		return -1;
	}

	void *buffer = malloc(BUFSIZE);
	if(buffer == 0) {
		perror("malloc");
		return -1;
	}

	test_suspend_read_and_write(fd, (char *)buffer, BUFSIZE);
	test_lio_read_and_write(fd, (char *)buffer, BUFSIZE);

	free(buffer);
	close(fd);
	remove("file.txt");

	return 0;
}
