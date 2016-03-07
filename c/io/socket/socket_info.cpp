#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	int ret = 0;
	char *host_name;
	struct hostent *ent;
	int opt;
	char szHostName[255];

	while ((opt = getopt(argc, argv, "a:")) != -1) {
		switch (opt) {
			case 'a':
				host_name = optarg;
				break;
			default: /* '?' */
				exit(EXIT_FAILURE);
		}
	}

	ent = gethostbyname(host_name);


	memset(szHostName, 0, 255);
	gethostname(szHostName, 255);
	ent = gethostbyname(szHostName);

	return ret;
}
