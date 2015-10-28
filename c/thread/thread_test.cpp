#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include "thread.h"

class mythread : public Thread {
	int tag;
	void thread();
public:
	void start();
	void stop();
	static void default_sig_action(int signo, siginfo_t *info, void *context);
	int add_kill_action();
	mythread(int tag_id);
	~mythread();
} ;

void mythread::thread() {
	printf("%d:%s:%s:%d\n", tag, __FILE__, __func__, __LINE__);
	hasStarted();
	int i = 0;
	while(i < 5) {
		printf("%d:xiaofei:%08d\n", tag, i++);
		sleep(1);
	}

}

void mythread::start() {
	printf("%d:%s:%s:%d\n", tag, __FILE__, __func__, __LINE__);
	add_kill_action();
	run();
}

void mythread::default_sig_action(int signo, siginfo_t *info, void *context) {
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	/* Ensure it's our signal */
	if (info->si_signo == SIGUSR1) {
		abort();
	}

	return;
}
int mythread::add_kill_action() {
	printf("%d:%s:%s:%d\n", tag, __FILE__, __func__, __LINE__);
	struct sigaction sig_act;

	/* Set up the signal handler */
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = SA_SIGINFO;
	sig_act.sa_sigaction = default_sig_action;

	/* Map the Signal to the Signal Handler */
	return sigaction(SIGUSR1, &sig_act, NULL);
}

void mythread::stop() {
	printf("%d:%s:%s:%d\n", tag, __FILE__, __func__, __LINE__);
	sendSignal(SIGUSR1);
	//kill(true); /* Kill means join actually */
}

mythread::mythread(int tag_id) {
	tag = tag_id;
	printf("%d:%s:%s:%d\n", tag, __FILE__, __func__, __LINE__);
}

mythread::~mythread() {
	printf("%d:%s:%s:%d\n", tag, __FILE__, __func__, __LINE__);
}

int main(int argc, char **argv) {
	{
		mythread thread(0);
		thread.start();
		sleep(1);
		//thread.stop();
	}

	{
		std::vector<mythread *> vector_mythread;
		std::vector<mythread *>::iterator it;
		int i = 0;

		vector_mythread.clear();

		while(i < 6) {
			mythread *t = new mythread(i);
			vector_mythread.push_back(t);
			t->start();
			sleep(1);
			i++;
		}

		for(it = vector_mythread.begin(); it != vector_mythread.end(); it++) {
			delete *it;
		}
	}
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	sleep(6);
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);

	return 0;
}
