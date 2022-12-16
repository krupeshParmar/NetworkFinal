#pragma once

#include "Common.h"
#include "proto_game.pb.h"
#define XMIN -37
#define XMAX 37
#define ZMIN -2
#define ZMAX 33

class Server {

public:
	Server();
	~Server();

	int Initialize();
	bool Update();
	int Receive();
	void Shutdown();
	void UpdatePlayerState();
	void UpdateClients(unsigned int messageId);
	void GetInforFromClients();
private:
	WSADATA m_wsaData;
	int g_Iteration;
	//GameStateMessage g_GameStateMessage;
	ClientInfo g_RecvClientInfo;
	ServerInfo g_ServerInfo;
	GameState gameState;
	std::vector<ClientInfo> g_ClientInfo;
	std::vector<InputMessage*> g_PlayerMessages;
	bool executing = true;
	int Bind();
	int CreateSocket();
	int Startup();
	bool FindPlayer(int port, ClientInfo& clientInfo);
	PlayerStateMessage& GetPlayerInfoReferenceById(USHORT id);
};