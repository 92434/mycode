#include "log.h"

log::log() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

log::~log() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int log::set_logfile(std::string logfile) {
	int ret = 0;
	logfs.open(logfile.c_str(), std::ios::app);
	backupbuf = std::cout.rdbuf();//back up cout's streambuf
	logbuf = logfs.rdbuf();//get file's streambuf
	std::cout.rdbuf(logbuf);//assign streambuf to cout
	return ret;
}

int log::reset_logfile() {
	int ret = 0;
	std::cout.rdbuf(backupbuf);//restore cout's original streambuf
	logfs.close();
	return ret;
}
