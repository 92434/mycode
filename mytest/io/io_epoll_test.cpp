#include "io_thread.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#ifdef LOOPER_USES_EPOLL
#include <sys/epoll.h>
#else
#include <poll.h>
#endif

class my_io_thread : public io_thread {
	int pipe_fd[2];
	unsigned int i, j;

	void io_read(int io_fd) {
		//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		int ii;
		int size;
		size = read(io_fd, &ii, sizeof(ii));
		if(size > 0) {
			printf("read %d(%08x)\n", ii, ii);
		}
		if(ii >= j) {
			stop();
		}

		int status;
		status = add_io_fd(io_fd, EPOLLIN);
		if(status != 0) {
			stop();
		}

		io_write(pipe_fd[1]);
	}

	void io_write(int io_fd) {
		//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		write(io_fd, &i, sizeof(i));
		printf("write %d(%08x)\n", i, i);
		i++;
	}
public:
	my_io_thread() {
		i = 0;
		j = 0;
		//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		if (pipe(pipe_fd) == -1) {
			printf("pipe create.  errno=%d:%m\n", errno);
			exit(-1);
		}
	}

	~my_io_thread() {
		//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		wait_for_thread();
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}

	void test_pipe(unsigned int ii, unsigned int jj) {
		//printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
		i = ii;
		j = jj;

		start();

		int status;
#ifdef LOOPER_USES_EPOLL
		status = add_io_fd(pipe_fd[0], EPOLLIN);
		io_write(pipe_fd[1]);
#else
		status = add_io_fd(pipe_fd[0], POLLIN);
		io_write(pipe_fd[1]);
#endif
		if(status != 0) {
			stop();
		}
	}
};

int main(int argc, char **argv) {
	int loop = 0;
	while(true) {
		poll_thread *poll_thread_instance = poll_thread::get_instance();
		if(poll_thread_instance == NULL) {
			return -1;
		}

		poll_thread_instance->start();

		int total = 100;

		int i;
		std::vector<my_io_thread *> vec_io_thread;
		for(i = 0; i < total; i++) {
			vec_io_thread.push_back(new my_io_thread());
		}

		for(i = 0; i < total; i++) {
			vec_io_thread[i]->test_pipe(i * 5, i * 5 + 3);
		}
		
		usleep(100);

		for(i = 0; i < total; i++) {
			usleep(1);
			vec_io_thread[i]->stop();
			delete vec_io_thread[i];
			//usleep(10000);
		}

		vec_io_thread.clear();
		
		poll_thread_instance->stop();
		poll_thread_instance->del_instance();
		printf("-------------------%d-------------------------\n", loop++);
		//sleep(1);
	}

	return 0;
}
