#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include "proto_game.pb.h"

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <conio.h>
#include <chrono>
#include <assert.h>

#include <map>
#include <vector>

// Need to link Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "5555"

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

struct Server {
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
	
};

struct UserInputMessage : public Message {
	bool W;
	bool S;
	bool A;
	bool D;
};


int g_Iteration;
GameStateMessage g_GameStateMessage;
ClientInfo g_RecvClientInfo;
Server g_ServerInfo;

int main()
{
	WSADATA wsaData;
	int result;

	printf("WSAStartup . . . \n");
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed with error %d\n", result);
		return -1;
	}
	else printf("Success\n");

	printf("Creating our Listen Socket . . . \n");
	g_ServerInfo.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (g_ServerInfo.socket == INVALID_SOCKET)
	{
		printf("Socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return -2;
	}
	else {
		printf("socket: Success!\n");
	}

	struct sockaddr_in recvAddr;
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(5555);
	recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(g_ServerInfo.socket, (SOCKADDR*)&recvAddr, sizeof(recvAddr));
	if (result != 0)
	{
		wprintf(L"bind failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return -3;
	}
	else printf("bind: Success!\n");

	g_RecvClientInfo.clientAddrSize = sizeof(g_RecvClientInfo.clientAddr);
	g_RecvClientInfo.HaveInfo = false;
	
	// GAME LOOP HERE
	proto_game::PlayerState playerObj;
	playerObj.set_requestno(420);

	printf("Closing . . . \n");
	closesocket(g_ServerInfo.socket);
	WSACleanup();

	return 0;
}