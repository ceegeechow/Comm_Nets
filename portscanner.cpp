#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

//checks if port on given address is open
bool port_is_open(const struct sockaddr_in addr)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0); //TCP socket
    if ((connect(sfd, (struct sockaddr *) &addr, sizeof(addr))) == 0)
    {
		close(sfd);
		return true;
    }
    close(sfd);
	return false;
}

int main (int argc, char** argv)
{
	char arg;
	int start = 1;
	int stop = 1024;
	char* hostname;

	//check for input arguments
	if (argc <= 1)
	{
		std::cerr << "Error: Not enough input arguments\n";
		return -1;
	}

	//parse input arguments
	while ((arg = getopt(argc, argv, "p:")) != -1)
	{
		if (arg == 'p')
		{
			std::string str = optarg;
			char* s = const_cast<char*>(str.c_str());
			start = atoi(strtok(s, ":"));
			stop = atoi(strtok(NULL, ":"));
		}
	}
	if (optind >= argc)
	{
		std::cerr << "No hostname specified\n";
		return -1;
	}
	else
	{
		hostname = argv[optind];
	}

	//convert hostname to server addr
	struct sockaddr_in server;
	struct hostent *he;
	he = gethostbyname(hostname);
	memcpy((char*)&server.sin_addr, (char*)he->h_addr, he->h_length);
  	server.sin_family = AF_INET;
  	
  // 	server.sin_port = htons(1);
  // 	int sfd = socket(AF_INET, SOCK_STREAM, 0); //TCP socket
  //   if ((connect(sfd, (struct sockaddr *) &server, sizeof(server))) == 0)
  //   {
		// std::cout << "works!\n";
  //   }
  //   else {
  //   	std::cout << "closed :(\n";
  //   }

  	std::cout<< "Starting scan of ports " << start << " through " << stop << "\n";

	//scan range of ports
	for (int i = start; i <= stop; i++)
	{
		server.sin_port = htons(i); //set port number
		if (port_is_open(server))
		{
			std::cout << "Port " << i << " is open\n";
		}
	}
	std::cout << "Finished scanning ports " << start << " through " << stop << "\n";

	return 0;
}