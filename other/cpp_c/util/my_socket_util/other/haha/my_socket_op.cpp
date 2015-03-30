#include "my_socket_op.h"

my_socket_op::my_socket_op( void )
{
	socket_host = SOCKET_ERROR;
	socket_accept = SOCKET_ERROR;
	memset(&addr_client, 0, sizeof(addr_client));
}

bool my_socket_op::init_env( void )
{
	bool b_rtn = false;

	// Initialize Winsock.
	WSADATA wsa_data;
	int i_wsa_status = WSAStartup( MAKEWORD(2,2), &wsa_data );
	if ( i_wsa_status != NO_ERROR )
	{
		printf("Error at WSAStartup()\n");
		uninit_env();
		return b_rtn;
	}

	b_rtn = true;

	return b_rtn;
}

int my_socket_op::uninit_env( void )
{
	WSACleanup();

	int i_err = WSAGetLastError();
	return i_err;
}

bool my_socket_op::open_socket( IN int af, IN int type, IN int protocol )
{
	bool b_rtn = false;
	// Create a socket.
	socket_host = socket( af, type, protocol );

	if ( socket_host == INVALID_SOCKET )
	{
		printf( "Error at socket(): %ld\n", WSAGetLastError() );
		return b_rtn;
	}
	else
	{
		printf( "server socket %ld\n", socket_host );
		b_rtn = true;
	}

	return b_rtn;
}

bool my_socket_op::bind_host_port( const char *cp_server_ip, u_short us_server_port )
{
	bool b_rtn = false;

	// Bind the socket.
	sockaddr_in addr_server;
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = inet_addr( cp_server_ip );
	addr_server.sin_port = htons( us_server_port );

	if ( bind( socket_host, (SOCKADDR*) &addr_server, sizeof(addr_server) ) == SOCKET_ERROR )
	{
		printf( "bind() failed.\n" );
	}
	else
	{
		b_rtn = true;
	}

	return b_rtn;
}

bool my_socket_op::listen_host_port()
{
	bool b_rtn = false;

	// Listen on the socket.
	if ( listen( socket_host, 1 ) == SOCKET_ERROR )
	{
		printf( "Error listening on socket.\n");
	}
	else
	{
		b_rtn = true;
	}

	return b_rtn;
}

void my_socket_op::accept_client( void )
{
	memset(&addr_client, 0, sizeof(addr_client));

	int addrlen = sizeof(addr_client);

	socket_accept = SOCKET_ERROR;

	while ( socket_accept == SOCKET_ERROR )
	{
		socket_accept = accept( socket_host, (struct sockaddr *)&addr_client, &addrlen );
	}

	printf("New connection from %s(%d) on socket %d\n",inet_ntoa(addr_client.sin_addr), ntohs(addr_client.sin_port), socket_accept);
}

bool my_socket_op::connect_server( const char *cp_server_ip, u_short us_server_port )
{
	bool b_rtn = false;

	// Bind the socket.
	addr_client.sin_family = AF_INET;
	addr_client.sin_addr.s_addr = inet_addr( cp_server_ip );
	addr_client.sin_port = htons( us_server_port );

	if ( connect( socket_host, (SOCKADDR*) &addr_client, sizeof(addr_client)) == SOCKET_ERROR )
	{
		printf( "Failed to connect.\n" );
		b_rtn = false;
	}
	else
	{
		printf("New connection from %s(%d) on socket %d\n",inet_ntoa(addr_client.sin_addr), ntohs(addr_client.sin_port), socket_host);
	}

	printf("-----------Client-----------\n");

	return b_rtn;
}

