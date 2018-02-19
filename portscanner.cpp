#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <string.h>

//checks if port on given address is open
bool port_is_open(const struct sockaddr_in addr)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 6); 		//right type??? use getprotoent or 0??
    if ((connect(sfd, (struct sockaddr *) &addr, sizeof(addr))) == 0)
		return true;
	return false;
}

int main (int argc, char** argv)
{
	char arg;
	int start = 1;
	int stop = 1024;
	char* hostname;

	if (argc <= 1)
	{
		std::cerr << "Error: Not enough input arguments";
		exit(0);
	}

	//parse input arguments
	while ((arg = getopt(argc, argv, "p:")) != -1)
	{
		if (arg == 'p')
		{
			std::string str = optarg;
			char* s = const_cast<char*>(str.c_str());
			start = atoi(std::strtok(s, ":"));
			stop = atoi(std::strtok(NULL, ":"));
		}
	}
	if (optind >= argc)
	{
		std::cerr << "No hostname specified\n";
		exit(0);
	}
	else
	{
		hostname = argv[optind];
	}

	//convert hostname to server addr
	struct sockaddr_in server;
	struct hostent *he;
	he = gethostbyname(hostname);
	memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);
  	server.sin_family = AF_INET;

	//scan range of ports
	for (int i = start; i <= stop; i++)
	{
		server.sin_port = htons(i);
		if (port_is_open(server))
		{
			std::cout << "Port: " << i << " is open\n";
		}
	}
	std::cout << "Finished scanning ports " << start << " through " << stop << "\n";

	return 0;
}