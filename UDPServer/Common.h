#pragma once

#include <WinSock2.h>
#include <vector>
#include <string>

struct ServerInfo {
	SOCKET socket;
};

struct ClientInfo {
	bool HaveInfo;
	sockaddr_in clientAddr;
	int clientAddrSize;
	USHORT playerID;
};

struct Message {
	unsigned int messageID;
};

struct PlayerStateMessage : public Message {
	double id;
	int count;
	float posx, posz;
	float bulx, bulz;
	int input_sum;
	bool isshot;
};

struct GameState : public Message
{
	std::string game_state;
};

