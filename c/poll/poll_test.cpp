#include "poll.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <string.h>

typedef struct _pipe_fd {
	int pipefd[2];
} pipe_fd_t;

template<class T>
class test_case {
	std::vector<pipe_fd_t *> vector_pipe_fd;
	int size;
	T t;
public:
	test_case(int n = 5) : size(n) {
		vector_pipe_fd.clear();
		create_pipes(size);
	}

	~test_case() {
		t.remove_all_fds();
		release_pipes();
	}

	int create_pipes(int n) {
		int ret = 0;

		for(int i = 0; i < n; i++) {
			pipe_fd_t *ppipe_fd = new pipe_fd_t;
			if (pipe(ppipe_fd->pipefd) == -1) {
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			vector_pipe_fd.push_back(ppipe_fd);
		}

		return ret;
	}

	int release_pipes() {
		int ret = 0;
		std::vector<pipe_fd_t *>::iterator it;
		for(it = vector_pipe_fd.begin(); it != vector_pipe_fd.end(); it++) {
			close((*it)->pipefd[0]);
			close((*it)->pipefd[1]);
			delete *it;
		}
		vector_pipe_fd.clear();

		return ret;
	}

	int add_pipes_fd0_to_poll(unsigned int events) {
		int ret = 0;

		for(int i = 0; i < size; i++) {
			int flags;

			t.add_fd(vector_pipe_fd[i]->pipefd[0], events);
			flags = fcntl(vector_pipe_fd[i]->pipefd[1],F_GETFL,0);
			flags |= O_NONBLOCK;
			flags = fcntl(vector_pipe_fd[i]->pipefd[1],F_SETFL,flags);
		}

		return ret;
	}

	int write_pipes_fd1() {
		int ret = 0;

		for(int i = 0; i < size; i++) {
			char write_buf[20];
			sprintf(write_buf, "%d", i);
			write(vector_pipe_fd[i]->pipefd[1], write_buf, strlen(write_buf));
		}

		return ret;
	}
};

class my_select : public select_poll {
	int process_fd(int fd, unsigned int events) {
		int ret = 0;
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf(":fd: %d\n", fd);

		char read_buf[20];
		read(fd, read_buf, 20);
		return ret;
	}
};

class my_poll : public poll_poll {
	int process_fd(int fd, unsigned int events) {
		int ret = 0;
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf(":fd: %d\n", fd);

		char read_buf[20];
		read(fd, read_buf, 20);
		return ret;
	}
};

class my_epoll : public epoll_poll {
	int process_fd(int fd, unsigned int events) {
		int ret = 0;
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf(":fd: %d\n", fd);

		char read_buf[20];
		read(fd, read_buf, 20);
		return ret;
	}
};

int main(int argc, char **argv) {
	int ret = 0;
	bool stop = false;

	if(argc < 3) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("para error\n");
		return ret;
	}

	if(strtod(argv[1], NULL) == 0) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n====================================test select\n");

		while(!stop) {
			test_case<my_select> test(strtod(argv[2], NULL));
			test.add_pipes_fd0_to_poll(0xffffffff);

			int i = 0;

			while(i < 1) {
				test.write_pipes_fd1();
				usleep(100000);
				i++;
			}
			stop = true;
		}
	}

	if(strtod(argv[1], NULL) == 1) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n====================================test poll\n");

		while(!stop) {
			test_case<my_poll> test(strtod(argv[2], NULL));
			test.add_pipes_fd0_to_poll(POLLIN | POLLOUT);

			int i = 0;

			while(i < 2) {
				test.write_pipes_fd1();
				usleep(100000);
				i++;
			}
			stop = true;
		}
	}

	if(strtod(argv[1], NULL) == 2) {
		printf("[%s:%s:%d]", __FILE__, __PRETTY_FUNCTION__, __LINE__);
		printf("\n====================================test epoll\n");

		while(!stop) {
			test_case<my_epoll> test(strtod(argv[2], NULL));
			test.add_pipes_fd0_to_poll(EPOLLIN | EPOLLOUT);

			int i = 0;

			while(i < 2) {
				test.write_pipes_fd1();
				usleep(100000);
				i++;
			}
			stop = true;
		}
	}

	return ret;
}
