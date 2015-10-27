#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ftw.h>
#include <stdlib.h>
#include <stdint.h>


#define _XOPEN_SOURCE 500

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
	printf("%-3s %2d %7jd   %-40s %d %s\n",
			(tflag == FTW_D) ? "d" :
			(tflag == FTW_DNR) ? "dnr" :
			(tflag == FTW_DP) ? "dp" :
			(tflag == FTW_F) ? "f" :
			(tflag == FTW_NS) ? "ns" :
			(tflag == FTW_SL) ? "sl" :
			(tflag == FTW_SLN) ? "sln" :
			"???",
			ftwbuf->level,
			(intmax_t)sb->st_size,
			fpath,
			ftwbuf->base,
			fpath + ftwbuf->base);
	return 0;           /* To tell nftw() to continue */
}


int test_ftw(int argc, char *argv[]) {
	int ret = 0;
	int flags = 0;

	if(argc > 2 && strchr(argv[2], 'd') != NULL)
		flags |= FTW_DEPTH;
	if(argc > 2 && strchr(argv[2], 'p') != NULL)
		flags |= FTW_PHYS;

	if(nftw((argc < 2) ? "." : argv[1], display_info, 20, flags) == -1) {
		perror("nftw");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);

	return ret;
}

static int out_filename(const char *filename) {
	int ret = 0;

	int len = strlen(filename);
	int i;

	for(i = 0; i < len; i++) {
		printf("0x%02x ", (unsigned char)filename[i]);
	}
	printf("\n");
	printf("%s\n", filename);

	return ret;
}

static int trim_filename(char *filename) {
	int ret = 0;

	int len = strlen(filename);
	int i;

	for(i = len - 1; i >= 0; i--) {
		if(
				filename[i] != '.'
				&& filename[i] != ' ') {
			break;
		}
		filename[i] = 0;
	}

	return ret;
}

static char str_path[PATH_MAX] = {0};

static int open_dir(const char *dirname)
{
	char devname[PATH_MAX] = {0};
	char *filename;
	DIR *dir;
	struct dirent *de;
	int ret = 0;

	dir = opendir(dirname);
	if(dir == NULL)
		return -1;

	strcpy(devname, dirname);
	filename = devname + strlen(devname);
	*filename++ = '/';

	while((de = readdir(dir))) {
		if(
				de->d_name[0] == '.'
				&& (
					de->d_name[1] == '\0' 
					|| (de->d_name[1] == '.' && de->d_name[2] == '\0')
				   )
		  ) {
			continue;
		}

		//out_filename(de->d_name);
		trim_filename(de->d_name);
		//out_filename(de->d_name);
		strcpy(filename, de->d_name);
		out_filename(devname);
		ret = chdir(devname);
		if(ret != 0) {
			printf("de->d_ino: %d\n", (int)de->d_ino);
			printf("%s\n", strerror(errno));
			printf("%s\n", devname);
		}
		strcpy(str_path, devname);
	}
	closedir(dir);
	return ret;
}

int test_open_dir(int argc, char **argv) {
	int ret = 0;

	//open_dir("/media/xiaofei/E0CA-5EE7/Thumbs.ms/com1.{d3e34b21-9d75-101a-8c3d-00aa001a1652}");
	if(argc <= 1) {
		return ret;
	}
	open_dir(argv[1]);

	return ret;
}

int main(int argc, char **argv) {
	int ret = 0;
	test_ftw(argc, argv);
	return ret;
}
