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
	double id = -1;
	int count;
	float posx, posz;
	float bulx, bulz;
	bool died = false;
	//int input_sum;
	//bool isshot;
};
struct InputMessage : public Message {
	int id;
	int count;
	int input_sum;
	int isshot;
};

struct GameState : public Message
{
	PlayerStateMessage player1;
	PlayerStateMessage player2;
	PlayerStateMessage player3;
	PlayerStateMessage player4;
};

