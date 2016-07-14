#include <time.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	time_t tmpcal_ptr = {0};
	struct tm *tmp_ptr = NULL;

	tmpcal_ptr = time(NULL);
	printf("tmpcal_ptr=%d\n", tmpcal_ptr);   

	tmp_ptr = gmtime(&tmpcal_ptr);
	printf("after gmtime, the time is:%d:%d:%d\n", tmp_ptr->tm_hour, tmp_ptr->tm_min, tmp_ptr->tm_sec);

	tmp_ptr = localtime(&tmpcal_ptr);
	printf("after localtime, the time is:%d:%d:%d\n", tmp_ptr->tm_hour, tmp_ptr->tm_min, tmp_ptr->tm_sec);

	return 0;
}
