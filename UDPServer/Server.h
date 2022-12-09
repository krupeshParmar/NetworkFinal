#pragma once

#include "Common.h"

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
private:
	WSADATA m_wsaData;
	int g_Iteration;
	GameStateMessage g_GameStateMessage;
	ClientInfo g_RecvClientInfo;
	ServerInfo g_ServerInfo;
	std::vector<ClientInfo> g_ClientInfo;
	std::vector<UserInputMessage> g_PlayerMessages;
	bool executing = true;
	int Bind();
	int CreateSocket();
	int Startup();
	bool FindPlayer(int port, ClientInfo& clientInfo);
	PlayerInfo& GetPlayerInfoReferenceById(USHORT id);
};