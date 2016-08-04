#ifndef _UTILS_H
#define _UTILS_H
#include <signal.h>
typedef struct {
	int fd;
	unsigned char *buffer;
} thread_arg_t;
typedef void (*sig_action_t)(int, siginfo_t *, void *);
int catch_signal(sig_action_t sig_action);
void printids(const char *s);
#endif//#ifndef _UTILS_H
