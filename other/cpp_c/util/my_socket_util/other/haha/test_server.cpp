#include "my_tcp_socket_op.h"
#include "my_thread.h"

bool b_exit = false;

DWORD WINAPI start_a_server(LPVOID lp_para)
{
	my_tcp_socket_op my_tcp;
	my_tcp.create_tcp_server("0.0.0.0", 27015);
	my_tcp.accept_tcp_client();

	return 0;
}

DWORD WINAPI start_a_client(LPVOID lp_para)
{
	my_tcp_socket_op my_tcp;
	if(!my_tcp.create_tcp_client("127.0.0.1", 27015))
	{
		b_exit = true;
	}

	return 0;
}


int main()
{
	my_tcp_socket_op my_tcp;
	if (!my_tcp.init_env())
	{
		return 1;
	}

	my_thread::create_thread(start_a_client, 0);

	while (!b_exit)
	{
		Sleep(100);
	}

	return 0;
}