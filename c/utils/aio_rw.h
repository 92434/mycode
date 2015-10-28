#ifndef __AIO_RW_H
#define __AIO_RW_H

#include <vector>

#include <aio.h>
#include <signal.h>


#define BUFSIZE 1024
#define MAX_LIST 3

typedef int (*read_or_write_callback_t)(struct aiocb *);
typedef void (*sig_action_t)(int, siginfo_t *, void *);
typedef void (*thread_callback_t)(sigval_t);

typedef enum _read_write {
	op_write = 0,
	op_read,
	op_noop,
} read_write_t;

class aio {
private:
	std::vector<struct aiocb *> vpaiocb;
	int aio_fd;
	aio() {}
public:
	aio(int fd) : aio_fd(fd) {
		vpaiocb.clear();
	}

	~aio() {
		std::vector<struct aiocb *>::iterator it;
		for(it = vpaiocb.begin(); it != vpaiocb.end(); it++) {
			delete *it;
		}

		vpaiocb.clear();
	};

	int add_aiocb(off_t offset, void *buffer, size_t size, bool lio_aio = false, int lio_opcode = LIO_NOP);
	static void default_sig_action(int signo, siginfo_t *info, void *context);
	int attach_aio_to_sigio(sig_action_t sig_action, int index = -1);
	static void default_thread_callback(sigval_t sigval);
	int attach_aio_to_thread_callback(thread_callback_t thread_callback, int index = -1);
	int aio_rw_suspend(read_write_t read_write = op_noop);
	int aio_rw_listio();
};

#endif
