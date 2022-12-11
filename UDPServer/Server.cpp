#include "Server.h"

#include <iostream>
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

	/*const int bufsize = sizeof(UserInputMessage);
	char buf[bufsize];*/
	this->GetInforFromClients();
	this->UpdatePlayerState();
	this->UpdateClients(1);
	return true;
}

int Server::Receive()
{
	/*
	//const int bufsize = sizeof(UserInputMessage);
	//char buf[bufsize];

	//int result = recvfrom(
	//	g_ServerInfo.socket, 
	//	buf, 
	//	bufsize, 
	//	0, 
	//	(SOCKADDR*)&g_RecvClientInfo.clientAddr, 
	//	&g_RecvClientInfo.clientAddrSize);
	//
	//if (result == SOCKET_ERROR)
	//{
	//	if (WSAGetLastError() != WSAEWOULDBLOCK)
	//	{
	//		printf("recv failed with error: %d\n", WSAGetLastError());
	//		closesocket(g_ServerInfo.socket);
	//		WSACleanup();
	//		//g_doQuit = true; Remove client from connection.
	//	}
	//	return 0;
	//}

	//if (result == 0) {
	//	//TODO remove from socket
	//	return 0;
	//}

	//ClientInfo clientInfo;
	//USHORT port = g_RecvClientInfo.clientAddr.sin_port;
	//if (!FindPlayer(port, clientInfo)) {

	//	USHORT numPlayersConnected = g_ClientInfo.size();

	//	clientInfo.clientAddr = g_RecvClientInfo.clientAddr;
	//	clientInfo.clientAddrSize = g_RecvClientInfo.clientAddrSize;
	//	clientInfo.HaveInfo = true;
	//	clientInfo.playerID = numPlayersConnected;

	//	g_ClientInfo.push_back(clientInfo);
	//	g_PlayerMessages.push_back(UserInputMessage());
	//}
	//UserInputMessage& userInputMessage = g_PlayerMessages[clientInfo.playerID];
	//std::string serializedPlayerState;

	//memcpy(&userInputMessage, (const void*)buf, bufsize);
	////memcpy((char*)serializedPlayerState.data(), (const void*)buf, bufsize);

	//proto_game::PlayerState playerState = proto_game::PlayerState();
	//bool succ = playerState.ParseFromString(serializedPlayerState);

	//if (!succ) {
	//	return 1;
	//}
	//else {
	//	printf("Bulletposx: %.2f, bulletposz: %.2f, input: %d, isShot: %d\n", playerState.bulletposx(), playerState.bulletposz(), playerState.input(), playerState.isshot());
	//}

	//int hasUserInput = userInputMessage.W || userInputMessage.A || userInputMessage.S || userInputMessage.D;

	//if (hasUserInput)
	//	printf("%d RECV: [%d: %d] %d %d %d %d\n", g_Iteration++, clientInfo.playerID, userInputMessage.messageID,
	//		userInputMessage.W, userInputMessage.A, userInputMessage.S, userInputMessage.D);
	*/
	char buf[512];

	int recvResult = recvfrom(
		g_ServerInfo.socket,
		buf,
		512,
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
		}
		return 0;
	}
	if (recvResult == 0)
	{
		return 0;
	}
	// ----
	proto_game::PlayerState playerState;
	USHORT port = g_RecvClientInfo.clientAddr.sin_port;
	ClientInfo clientInfo;
	if (FindPlayer(port, clientInfo) == false) {
		// The port does NOT exist in the map.
		// Add the player to the map
		USHORT numPlayersConnected = g_ClientInfo.size();

		// Create new client info
		clientInfo.clientAddr = g_RecvClientInfo.clientAddr;
		clientInfo.clientAddrSize = g_RecvClientInfo.clientAddrSize;
		clientInfo.HaveInfo = true;
		clientInfo.playerID = numPlayersConnected;

		// Store the client info
		g_ClientInfo.push_back(clientInfo);
		g_PlayerMessages.push_back(PlayerStateMessage());
	}

	PlayerStateMessage playerStateMessage; 
	playerStateMessage = g_PlayerMessages[clientInfo.playerID];
	memcpy(&playerStateMessage, (const void*)buf,sizeof(PlayerStateMessage));
	std::cout << "Player position: " 
		<< playerStateMessage.posx << ", " << playerStateMessage.posz << std::endl;
	std::cout << "Input Sum: " << playerStateMessage.input_sum << std::endl;
	std::string str = std::string(buf);
	//if (playerState.ParseFromString(str))
	//{
	//	/*g_RecvClientInfo.clientAddrSize = sizeof(g_RecvClientInfo.clientAddr);
	//	g_RecvClientInfo.HaveInfo = true;*/
	//	playerStateMessage.bulx = playerState.bulletposx();
	//	playerStateMessage.bulz = playerState.bulletposz();
	//	playerStateMessage.posx = playerState.posx();
	//	playerStateMessage.posz = playerState.posz();
	//	playerStateMessage.count = playerState.requestno();
	//	playerStateMessage.id = playerState.playerid();
	//	playerStateMessage.input_sum = playerState.input();
	//	playerStateMessage.isshot = playerState.isshot();	
	//	std::cout << "PosX, Posz" << playerState.posx() << ", " << playerState.posz() << std::endl;
	//}
	return 1;
}


void Server::GetInforFromClients()
{
	int recvCount = 0;
	bool stopReading = false;
	while (!stopReading)
	{
		int recvresult = Receive();
		recvCount += recvresult;

		if (recvresult == 0)
			stopReading = true;
	}
}

void Server::Shutdown() {
	printf("Closing . . . \n");
	closesocket(g_ServerInfo.socket);
	WSACleanup();
}

void Server::UpdatePlayerState()
{
	for (int i = 0; i < g_PlayerMessages.size(); i++) {
		PlayerStateMessage& playerMessage = g_PlayerMessages[i];
		bool press_w = false, press_s = false, press_a = false, press_d = false;
		if (playerMessage.input_sum >= 1000)
		{
			press_d = true;
			int sum = playerMessage.input_sum - 1000;
			if (sum > 0)
			{
				switch (sum)
				{
				case 1:
					press_w = true;
					break;
				case 10:
					press_s = true;
					break;
				case 100:
					press_a = true;
					break;

				case 11:
					press_w = true;
					press_s = true;
					break;

				case 101:
					press_a = true;
					press_w = true;
					break;

				case 111:
					press_a = true;
					press_w = true;
					press_d = true;
					break;

				}
			}
		}
		else if (playerMessage.input_sum >= 100)
		{
			press_a = true;
			if (playerMessage.input_sum - 100 > 0)
			{
				int sum = playerMessage.input_sum - 100;
				if (sum > 100)
				{
					switch (sum)
					{
					case 1:
						press_w = true;
						break;
					case 10:
						press_s = true;
						break;
					case 100:
						press_a = true;
						break;

					case 11:
						press_w = true;
						press_s = true;
						break;
					}
				}
			}
			else if (playerMessage.input_sum >= 10)
			{
				press_s = true;
				int sum = playerMessage.input_sum - 10;
				if (sum > 10)
				{
					switch (sum)
					{
					case 1:
						press_w = true;
						break;
					}
				}
			}
			else
			{
				press_w = true;
			}


		}
		if (press_w)
		{
			playerMessage.posz += 10.f;
		}
		if (press_s)
		{
			playerMessage.posz -= 10.f;
		}
		if (press_a)
		{
			playerMessage.posx -= 10.f;
		}
		if (press_d)
		{
			playerMessage.posx += 10.f;
		}
	}
}

void Server::UpdateClients(unsigned int messageId)
{
	GameState game_state;
	std::string state = "";
	for (int i = 0; i < g_PlayerMessages.size(); i++)
	{
		PlayerStateMessage playerStateMessage = g_PlayerMessages[i];
		state += std::to_string(g_PlayerMessages.size());
		state += ",";
		state += std::to_string(playerStateMessage.id);
		state += ",";
		state += std::to_string(playerStateMessage.count);
		state += ",";
		state += std::to_string(playerStateMessage.posx);
		state += ",";
		state += std::to_string(playerStateMessage.posz);
		state += ",";
		state += std::to_string(playerStateMessage.bulx);
		state += ",";
		state += std::to_string(playerStateMessage.bulz);
		if (i != g_PlayerMessages.size() - 1)
			state += ".";
		else state += "!";
	}
	game_state.game_state = state;


	//printf("SEND: { %.2f, %.2f }\n", g_GameStateMessage.player.x, g_GameStateMessage.player.y);

	for (int i = 0; i < g_ClientInfo.size(); i++) {
		const ClientInfo& clientInfo = g_ClientInfo[i];
		int sendResult = sendto(
			g_ServerInfo.socket,
			(const char*)&game_state,
			sizeof(game_state),
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
	g_PlayerMessages.clear();
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

//PlayerInfo& Server::GetPlayerInfoReferenceById(USHORT id)
//{
//	if (id == 0) return g_GameStateMessage.player1;
//	if (id == 1) return g_GameStateMessage.player2;
//	if (id == 2) return g_GameStateMessage.player3;
//	if (id == 3) return g_GameStateMessage.player4;
//
//	assert(-1);
//	return g_GameStateMessage.player1;
//}
