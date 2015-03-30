#ifndef __LOG_H
#define __LOG_H
#include <iostream>
#include <fstream>
class log {
private:
	std::ofstream logfs;
	std::streambuf *logbuf, *backupbuf;
public:
	log();
	~log();
	int set_logfile(std::string logfile);
	int reset_logfile();

};
#endif //__LOG_H
