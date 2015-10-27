#include "file_cache.h"

file_cache::file_cache() {
	bzero(&cache, sizeof(cache));
	cache.content = new unsigned char[FILE_CONTENT_CACHE];
	cache.cached = cache.cache_offset = 0;
	if(!cache.content) {
		std::cout << "no memory" << std::endl;
	}

}

file_cache::~file_cache() {
	if(cache.content) {
		delete []cache.content;
	}
	ifs.close();
}

int file_cache::open(std::string infile) {
	int ret = 0;
	ifs.open(infile.c_str());
	//file.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
	//try {
	//	ifs.open(infile.c_str(), std::ifstream::binary);
	//}
	//catch (std::ifstream::failure e) {
	//	std::cerr << "Exception \n";
	//}
	return ret;
}

int file_cache::do_cache_file() {
	int ret = 0;
	if(ifs.good()) {
		ifs.read((char *)cache.content + cache.cached, FILE_CONTENT_CACHE - cache.cached);
		cache.cached += ifs.gcount();
		cache.file_offset += ifs.gcount();
		//std::cout << "cached " << ifs.gcount() << std::endl;
	} else {
		ifs.close();
		ret = -1;
	}
	return ret;
}

int file_cache::cache_clean() {
	int ret = 0;

	int left = cache.cached - cache.cache_offset;
	unsigned char *pleft = cache.content + cache.cache_offset;

	memcpy(cache.content, pleft, left);
	cache.cached = left;
	cache.cache_offset = 0;

	return ret;
}

int file_cache::do_match() {
	int ret = 0;
	std::cout << __PRETTY_FUNCTION__ << " must be rewrite!!!" << std::endl;
	cache.cache_offset += cache.cached;
	ret = -1;
	return ret;
}

int file_cache::cache_match() {
	int ret = 0;

	while(do_match() == 0);
	return ret;
}

int file_cache::cache_search() {
	int ret = 0;

	cache_match();
	cache_clean();
	return ret;
}

int file_cache::cache_update() {
	int ret = 0;
	std::cout << __PRETTY_FUNCTION__ << " must be rewrite!!!" << std::endl;
	cache.cache_offset += cache.cached;
	ret = -1;
	return ret;
}

int file_cache::cache_process() {
	int ret = 0;

	cache_update();
	cache_clean();
	return ret;
}
