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
	
<<<<<<< HEAD
	// GAME LOOP HERE
	proto_game::SentState sentstate;
	/*
	*
	* 
	*	inputs
	*   pos
	*	newPos = pos, inputs
	*	
	* 
	*	Client:
	*		
	*/

	printf("Closing . . . \n");
	closesocket(g_ServerInfo.socket);
	WSACleanup();
=======
	server.Shutdown();
>>>>>>> 25538db8b9c3707e134ffd45ef2cc8c35ae9129c

	return 0;
}