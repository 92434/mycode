#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "optparse.h"
#include "xiaofei_debug.h"

int main(int argc, char **argv)
{
	const char *file_name = NULL;
	optparse opt;
	int rtn = 0;

	opt.add_option('f', true, "file name");
	opt.get_option(argc, argv);

	if(opt.have_option("f")) {
		file_name = opt.option_value("f").c_str();
	} else {
	}

	mydebug("file_name:%s\n", file_name);

	if(file_name == NULL) {
		rtn = -1;
		return rtn;
	}

	int fd = open(file_name, O_RDONLY);

	if(fd == -1) {
		mydebug("create file %s\n", file_name);
		int fd = open(file_name, O_CREAT, S_IRUSR);
	}

	if(fd == -1) {
		mydebug("error(errno): %s(%d)\n", strerror(errno), errno);
		rtn = fd;
		return rtn;
	}

	mydebug("fd:%d\n", fd);

	int c;

	rtn = flock(fd, LOCK_SH | LOCK_NB);
	mydebug("flock rtn:%d\n", rtn);

	mydebug("flock(LOCK_SH) ");

	if (rtn == -1) {
		mydebug("error(errno): %s(%d)\n", strerror(errno), errno);
		myprintf("failed!");
	} else {
		myprintf("successed!");
	}
	myprintf("(%s)\n", file_name);

	rtn = fread(&c, sizeof(char), 1, stdin);

	rtn = flock(fd, LOCK_EX | LOCK_NB);
	mydebug("flock rtn:%d\n", rtn);

	mydebug("flock(LOCK_EX) ");
	if (rtn == -1) {
		mydebug("error(errno): %s(%d)\n", strerror(errno), errno);
		myprintf("failed!");
	} else {
		myprintf("successed!");
	}
	myprintf("(%s)\n", file_name);

	rtn = fread(&c, sizeof(char), 1, stdin);

	rtn = flock(fd, LOCK_UN | LOCK_NB);
	mydebug("flock rtn:%d\n", rtn);

	mydebug("flock(LOCK_UN) ");
	if (rtn == -1) {
		mydebug("error(errno): %s(%d)\n", strerror(errno), errno);
		myprintf("failed!");
	} else {
		myprintf("successed!");
	}
	myprintf("(%s)\n", file_name);

	rtn = fread(&c, sizeof(char), 1, stdin);

	close(fd);

	return 0;
}
