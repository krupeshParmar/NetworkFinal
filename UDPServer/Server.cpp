#include "Server.h"
#include "proto_game.pb.h"

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
	recvAddr.sin_port = htons(DEFAULT_PORT);
	recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(g_ServerInfo.socket, (SOCKADDR*)&recvAddr, sizeof(recvAddr));
	if (result != 0)
	{
		wprintf(L"bind failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return -3;
	}
	else printf("bind: Success!\n");
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

void Server::Shutdown() {
	printf("Closing . . . \n");
	closesocket(g_ServerInfo.socket);
	WSACleanup();
}