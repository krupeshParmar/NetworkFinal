#pragma once

#include <WinSock2.h>
#include <vector>

struct PlayerInfo {
	float x, z;
};

struct BulletInfo {
	float x, z;
};

struct GameState {
	std::vector<PlayerInfo[]> players;
	std::vector<BulletInfo> bullets;
};

struct ServerInfo {
	SOCKET socket;
};

struct ClientInfo {
	bool HaveInfo;
	sockaddr_in clientAddr;
	int clientAddrSize;
	USHORT playerID;
};

struct UserInput {
	bool W, S, A, D;
};

struct Message {
	unsigned int messageID;
};

struct GameStateMessage : public Message {
	PlayerInfo player1;
	PlayerInfo player2;
	PlayerInfo player3;
	PlayerInfo player4;
};

struct UserInputMessage : public Message {
	bool W;
	bool S;
	bool A;
	bool D;
};
