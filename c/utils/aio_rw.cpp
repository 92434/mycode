#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "aio_rw.h"

int aio::add_aiocb(off_t offset, void *buffer, size_t size, bool lio_aio, int lio_opcode) {
	int ret = 0;
	struct aiocb *paiocb = new struct aiocb;

	bzero(paiocb, sizeof(struct aiocb));
	paiocb->aio_fildes = aio_fd;
	paiocb->aio_offset = offset;
	paiocb->aio_buf = buffer;
	paiocb->aio_nbytes = size;
	if(lio_aio) {
		paiocb->aio_lio_opcode = lio_opcode;
	}
	vpaiocb.push_back(paiocb);
	return ret;
}

void aio::default_sig_action(int signo, siginfo_t *info, void *context) {
	printf("xiaofei:%s called!\n", __func__);
	/* Ensure it's our signal */
	if (info->si_signo == SIGIO) {
		struct aiocb *req;
		int ret = 0;

		req = (struct aiocb *)info->si_value.sival_ptr;

		/* Did the request complete? */
		if (aio_error(req) == 0) {
			/* Request completed successfully, get the return status */
			ret = aio_return(req);
			printf("%s:%d ret(%d):%s\n", __func__, __LINE__, ret, strerror(errno));
		}
	}

	return;
}

int aio::attach_aio_to_sigio(sig_action_t sig_action, int index) {
	int ret = 0;
	struct sigaction sig_act;
	std::vector<struct aiocb *>::iterator it;
	int i;

	if(sig_action == 0) {
		sig_action = default_sig_action;
	}

	for(it = vpaiocb.begin(), i = 0; it != vpaiocb.end(); it++, i++) {
		if(index != -1 && index != i) {
			continue;
		}

		struct aiocb *paiocb = *it;

		/* Link the AIO request with the Signal Handler */
		paiocb->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
		paiocb->aio_sigevent.sigev_signo = SIGIO;
		paiocb->aio_sigevent.sigev_value.sival_ptr = paiocb;

		/* Set up the signal handler */
		sigemptyset(&sig_act.sa_mask);
		sig_act.sa_flags = SA_SIGINFO;
		sig_act.sa_sigaction = sig_action;

		/* Map the Signal to the Signal Handler */
		if((ret = sigaction(SIGIO, &sig_act, NULL)) == -1) {
			perror("sigaction");
			return ret;
		}
	}

	return ret;
}

void aio::default_thread_callback(sigval_t sigval) {
	struct aiocb *req;
	int ret;

	printf("xiaofei:%s called!\n", __func__);

	req = (struct aiocb *)sigval.sival_ptr;

	/* Did the request complete? */
	if (aio_error(req) == 0) {
		/* Request completed successfully, get the return status */
		ret = aio_return(req);
		printf("%s:%d ret(%d):%s\n", __func__, __LINE__, ret, strerror(errno));
	}

	return;
}

int aio::attach_aio_to_thread_callback(thread_callback_t thread_callback, int index) {
	int ret = 0;
	std::vector<struct aiocb *>::iterator it;
	int i;

	if(thread_callback == 0) {
		thread_callback = default_thread_callback;
	}

	for(it = vpaiocb.begin(), i = 0; it != vpaiocb.end(); it++, i++) {
		if(index != -1 && index != i) {
			continue;
		}

		struct aiocb *paiocb = *it;

		/* Link the AIO request with a thread callback */
		paiocb->aio_sigevent.sigev_value.sival_ptr = paiocb;
		paiocb->aio_sigevent.sigev_notify = SIGEV_THREAD;
		paiocb->aio_sigevent.sigev_notify_function = thread_callback;
		paiocb->aio_sigevent.sigev_notify_attributes = NULL;
	}


	return ret;
}

int aio::aio_rw_suspend(read_write_t read_write) {
	int ret = 0;
	struct aiocb *cblist[vpaiocb.size()];
	std::vector<struct aiocb *>::iterator it;
	int i;

	for(it = vpaiocb.begin(), i = 0; it != vpaiocb.end(); it++, i++) {
		cblist[i] = *it;

		read_or_write_callback_t callback = 0;

		if(read_write == op_read) {
			callback = aio_read;
		} else if(read_write == op_write) {
			callback = aio_write;
		}

		if(callback == 0) {
			ret = -1;
			return ret;
		}

		if((ret = callback(cblist[i])) != 0) {
			return ret;
		}
	}

	return aio_suspend(cblist, vpaiocb.size(), NULL);
}

int aio::aio_rw_listio() {
	struct aiocb *cblist[vpaiocb.size()];
	std::vector<struct aiocb *>::iterator it;
	int i;

	for(it = vpaiocb.begin(), i = 0; it != vpaiocb.end(); it++, i++) {
		cblist[i] = *it;
	}

	return lio_listio(LIO_WAIT, cblist, vpaiocb.size(), NULL);
}
