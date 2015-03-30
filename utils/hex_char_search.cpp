#include "hex_char_search.h"

std::ostream& operator<<(std::ostream& os, hex_char_t& hc) {
	if(hc.valid) {
		std::ios::fmtflags old = os.flags();
		os.setf(std::ios::hex, std::ios::basefield);
		os.width(2);
		os.fill('0');
		os << (unsigned int)hc.value;
		os.setf(old, std::ios::basefield);
	} else {
		os << '.';
	}

	return os;
}

hex_char_search::hex_char_search(std::string infile) {
	vhc.clear();
	open(infile);
}

int hex_char_search::get_hex_string(const std::string &s) {
	int ret = 0;
	int i;
	std::cout << __PRETTY_FUNCTION__ << ":";
	for(i = 0; i < (int)s.size();) {
		hex_char_t hc;
		int step = 1;
		if(s[i] == '.') {
			hc.valid = false;
			vhc.push_back(hc);
			std::cout << '.';
		} else if(i + 1 < (int)s.size() && isxdigit(s[i]) && isxdigit(s[i+1])) {
			char c[3] = {0};

			hc.valid = true;
			c[0] = s[i];
			c[1] = s[i+1];
			hc.value = strtoul(c,NULL,16);
			vhc.push_back(hc);
			step = 2;
			std::cout << hc;
		} else {
			ret = -1;
			break;
		}

		i += step;
	}

	std::cout << std::endl;

	return ret;
}

int hex_char_search::p_hex_string() {
	int ret = 0;
	int i;

	std::cout << __PRETTY_FUNCTION__ << ":";
	for(i = 0; i < (int)vhc.size(); i++) {
		std::cout << vhc[i];
	}
	std::cout << std::endl;

	return ret;
}

int hex_char_search::do_match() {
	int ret = 0;

	if(cache.cache_offset + (int)vhc.size() <= cache.cached) {
		int i;
		int step = 1;
		for(i = 0; i < (int)vhc.size(); i++) {
			if(vhc[i].valid && vhc[i].value != cache.content[cache.cache_offset + i]) {
				break;
			}
		}

		if(i == (int)vhc.size()) {
			//std::cout << "successed!" << std::endl;
			unsigned int pos = cache.file_offset - cache.cached + cache.cache_offset;
			printf("%08x\n", pos);
			step = i;
		}

		cache.cache_offset += step;
	} else {
		ret = -1;
	}

	return ret;
}
