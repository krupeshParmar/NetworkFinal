#pragma once

#include "Common.h"
#include "proto_game.pb.h"

class Server {

	int g_Iteration;
	GameStateMessage g_GameStateMessage;
	ClientInfo g_RecvClientInfo;
	ServerInfo g_ServerInfo;

	bool executing = true;

public:
	Server();
	~Server();

	int Initialize();
	bool Update();
	void Shutdown();
};