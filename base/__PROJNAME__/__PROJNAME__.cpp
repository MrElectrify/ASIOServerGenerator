#include <__PROJNAME__/Server.h>

#include <iostream>

using __PROJNAME__::Server;

int main(int argc, char* argv[])
{
	if (argc > 3)
	{
		std::cerr << "Usage: " << argv[0] << " <address:ip:0.0.0.0> <port:uint16_t:1234>\n";
		return 1;
	}
	std::string address = (argc >= 2) ? argv[1] : "0.0.0.0";
	std::string port = (argc == 3) ? argv[2] : "1234";
	Server s(address, port);
	s.Run();
	return 0;
}