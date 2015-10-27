#include <unistd.h>
#include <stdio.h>

int do_sth()
{
	//Add what u want
	return 0;
}
int main(int argc, char **argv)
{
	daemon(0,0);
	while (true)
	{
		do_sth();
		sleep(1);
	}
}
