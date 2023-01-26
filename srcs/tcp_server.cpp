#include "functions.hpp"

int tcp_server(int port) {

    /* 
        int socket(int domain, int type, int protocol); 
        domain refers to the protocol the socket will use for communication
        we use AF_INET - IPv4 Internet protocols
        and for type of communication we use SOCK_STREAM - Two-way reliable communication (TCP)
        When the protocol parameter is set to 0, it means that the socket function should choose
        the most appropriate protocol based on the specified socket type.
    */
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
        std::cerr << "ERROR, socket() failed!!" << std::endl;

    /*
        The next block of code is using the setsockopt() function to set the SO_REUSEADDR option on the socket, 
        which allows the socket to reuse the address even if it's in a timeout state.
        [ When a socket is in a timeout state, it is not able to communicate or send data.
        This can happen if a connection is closed or if there is a network error that causes the connection to be lost.]
    */
    int reuse = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR)
    if (reuse == -1)
        std::cerr << "ERROR, setsockopt() failed!!" << std::endl;
    /*
        the fcntl() function is used to set the O_NONBLOCK flag on the socket, making it non-blocking.
        This means that any operations performed on the socket,
        such as connecting, sending or receiving data, will not block the execution of the program.
    */
    reuse = fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	if (reuse < 0)
		std::cerr << "ERROR, fcntl() failed!!" << std::endl;

    struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	reuse = bind(socket_d, (struct sockaddr *)&addr, sizeof(addr));
	if (reuse == -1)
        std::cerr << "ERROR, bind() failed!!" << std::endl;

	reuse = listen(socket_d, BACKLOG);
	if (reuse == -1)
        std::cerr << "ERROR, listen() failed!!" << std::endl;

	return (socket_d);
}