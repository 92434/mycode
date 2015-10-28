#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	system("pwd") ;
	sleep(1) ; /*wait 9 second*/
	printf("Calling abort()\n");
	abort();
	printf("abort() after\n") ;
	return 0; /* This is never reached */
}
