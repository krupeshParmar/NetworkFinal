#include "Server.h"

#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

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

#define DEFAULT_PORT 5555

Server::Server() {

}

Server::~Server() {

}

int Server::Initialize() {

	if (Startup() != 0)
		return -1;

	if (CreateSocket() != 0)
		return -2;

	if (Bind() != 0)
		return -3;

	g_RecvClientInfo.clientAddrSize = sizeof(g_RecvClientInfo.clientAddr);
	g_RecvClientInfo.HaveInfo = false;
}

bool Server::Update() {

	const int bufsize = sizeof(UserInputMessage);
	char buf[bufsize];

	int recvResult = recvfrom(
		g_ServerInfo.socket,
		buf,
		bufsize,
		0,
		(SOCKADDR*)&g_RecvClientInfo.clientAddr,
		&g_RecvClientInfo.clientAddrSize
	);

	if (recvResult == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(g_ServerInfo.socket);
			WSACleanup();
			return false;
		}
	}

	std::cout << "recvbuffer: " << buf;

	g_RecvClientInfo.clientAddrSize = sizeof(g_RecvClientInfo.clientAddr);
	g_RecvClientInfo.HaveInfo = false;

	// GAME LOOP HERE
	proto_game::PlayerState playerObj;
	playerObj.set_requestno(420);
}

int Server::Receive()
{
	const int bufsize = sizeof(UserInputMessage);
	char buf[bufsize];

	int result = recvfrom(
		g_ServerInfo.socket, 
		buf, 
		bufsize, 
		0, 
		(SOCKADDR*)&g_RecvClientInfo.clientAddr, 
		&g_RecvClientInfo.clientAddrSize);
	
	if (result == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(g_ServerInfo.socket);
			WSACleanup();
			//g_doQuit = true; Remove client from connection.
		}
		return 0;
	}

	if (result == 0) {
		//TODO remove from socket
		return 0;
	}

	ClientInfo clientInfo;
	USHORT port = g_RecvClientInfo.clientAddr.sin_port;
	if (!FindPlayer(port, clientInfo)) {

		USHORT numPlayersConnected = g_ClientInfo.size();

		clientInfo.clientAddr = g_RecvClientInfo.clientAddr;
		clientInfo.clientAddrSize = g_RecvClientInfo.clientAddrSize;
		clientInfo.HaveInfo = true;
		clientInfo.playerID = numPlayersConnected;

		g_ClientInfo.push_back(clientInfo);
		g_PlayerMessages.push_back(UserInputMessage());
	}
	UserInputMessage& userInputMessage = g_PlayerMessages[clientInfo.playerID];
	std::string serializedPlayerState;

	memcpy(&userInputMessage, (const void*)buf, bufsize);
	//memcpy((char*)serializedPlayerState.data(), (const void*)buf, bufsize);

	proto_game::PlayerState playerState = proto_game::PlayerState();
	bool succ = playerState.ParseFromString(serializedPlayerState);

	if (!succ) {
		return 1;
	}
	else {
		printf("Bulletposx: %.2f, bulletposz: %.2f, input: %d, isShot: %d\n", playerState.bulletposx(), playerState.bulletposz(), playerState.input(), playerState.isshot());
	}

	int hasUserInput = userInputMessage.W || userInputMessage.A || userInputMessage.S || userInputMessage.D;

	if (hasUserInput)
		printf("%d RECV: [%d: %d] %d %d %d %d\n", g_Iteration++, clientInfo.playerID, userInputMessage.messageID,
			userInputMessage.W, userInputMessage.A, userInputMessage.S, userInputMessage.D);

	return 1;
}

void Server::Shutdown() {
	printf("Closing . . . \n");
	closesocket(g_ServerInfo.socket);
	WSACleanup();
}

void Server::UpdatePlayerState()
{
	for (int i = 0; i < g_PlayerMessages.size(); i++) {
		UserInputMessage& playerMessage = g_PlayerMessages[i];
		PlayerInfo& playerInfo = GetPlayerInfoReferenceById(i);

		// Update the game state
		if (playerMessage.A) {
			playerInfo.x += 10.f;
		}
		if (playerMessage.D) {
			playerInfo.x -= 10.f;
		}
		if (playerMessage.W) {
			playerInfo.z += 10.f;
		}
		if (playerMessage.S) {
			playerInfo.z -= 10.f;
		}

		playerMessage.W = 0;
		playerMessage.A = 0;
		playerMessage.S = 0;
		playerMessage.D = 0;
	}
}

void Server::UpdateClients(unsigned int messageId)
{
	int gameStateSize = sizeof(GameStateMessage);
	g_GameStateMessage.messageID = messageId;

	//printf("SEND: { %.2f, %.2f }\n", g_GameStateMessage.player.x, g_GameStateMessage.player.y);

	for (int i = 0; i < g_ClientInfo.size(); i++) {
		const ClientInfo& clientInfo = g_ClientInfo[i];
		int sendResult = sendto(
			g_ServerInfo.socket,
			(const char*)&g_GameStateMessage,
			gameStateSize,
			0,
			(SOCKADDR*)&clientInfo.clientAddr,
			clientInfo.clientAddrSize
		);

		if (sendResult == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("[%d] send failed with error: %d\n", i, WSAGetLastError());
				closesocket(g_ServerInfo.socket);
				WSACleanup();
				//g_doQuit = true;
			}

			return;
		}
	}
}

int Server::Bind()
{
	int result = -1;

	struct sockaddr_in recvAddr;
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(5555);
	recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(g_ServerInfo.socket, (SOCKADDR*)&recvAddr, sizeof(recvAddr));
	if (result != 0) {
		wprintf(L"bind failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	else {
		printf("bind: Success!\n");
	}
	return result;
}

int Server::CreateSocket()
{
	printf("Creating our Listen Socket . . . ");
	g_ServerInfo.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (g_ServerInfo.socket == INVALID_SOCKET) {
		printf("Socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	else {
		printf("socket: Success!\n");
	}

	DWORD NonBlock = 1;
	int result = -1;
	result = ioctlsocket(g_ServerInfo.socket, FIONBIO, &NonBlock);
	if (result != 0) {
		wprintf(L"ioctlsocket failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	else {
		printf("ioctlsocket: Success!\n");
	}

	return 0;
}

int Server::Startup()
{
	int result;
	WORD wVersionRequested = MAKEWORD(2, 2);
	result = WSAStartup(wVersionRequested, &m_wsaData);

	if (result != 0)
		printf("Failed to startup WSAStartup. Error - %d\n", result);
	else
		printf("WSAStartup was successful! \n");

	return result;
}

bool Server::FindPlayer(int port, ClientInfo& clientInfo)
{
	for (int i = 0; i < g_ClientInfo.size(); i++) {
		if (g_ClientInfo[i].clientAddr.sin_port == port) {
			clientInfo = g_ClientInfo[i];
			return true;
		}
	}

	return false;
}

PlayerInfo& Server::GetPlayerInfoReferenceById(USHORT id)
{
	if (id == 0) return g_GameStateMessage.player1;
	if (id == 1) return g_GameStateMessage.player2;
	if (id == 2) return g_GameStateMessage.player3;
	if (id == 3) return g_GameStateMessage.player4;

	assert(-1);
	return g_GameStateMessage.player1;
}
