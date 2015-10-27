#include "net_server.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#ifdef LOOPER_USES_EPOLL
#include <sys/epoll.h>
#else
#include <poll.h>
#endif

int main(int argc, char **argv) {
	poll_thread *poll_thread_instance = poll_thread::get_instance();
	if(poll_thread_instance == NULL) {
		return -1;
	}
	poll_thread_instance->start();

	net_server net_server_instance;
	net_server_instance.init_server_socket();
	net_server_instance.start_net_server();

	while(true) {
		usleep(1000);
	}

	poll_thread_instance->stop();
	poll_thread_instance->del_instance();
	return 0;
}
