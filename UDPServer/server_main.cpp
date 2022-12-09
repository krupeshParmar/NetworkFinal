#include "Server.h"

int main()
{
	Server server;

	server.Initialize();

	while (1) {
		if (!server.Update()) {
			return 1;
		}
	}
	
	server.Shutdown();

	return 0;
}