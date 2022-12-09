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
	
	// GAME LOOP HERE
	//proto_game::GameState sentstate;
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

	server.Shutdown();

	return 0;
}