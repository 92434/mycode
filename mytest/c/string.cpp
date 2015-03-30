#include <iostream>
#include <string.h>
using namespace std;

static void trim_file_name(const char *path, char **name){
	*name = (char *)path;
	char *pos = (char *)path;

	while(pos = strchr(pos, '/')){
		*name = pos;
		pos += 1;
		;
	}

	if(*name && **name == '/'){
		*name = (*name) + 1;
	}
}

int main(){
	char *filename = NULL;
	trim_file_name("xiaofei1/xiaofei.txt", &filename);
	cout << filename << endl;
}
