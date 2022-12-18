#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <string>
#include "proto_game.pb.h"
#include "../UDPServer/Common.h"
#include "inc_opengl.h"


enum ClientState {
	NotInitialized,
	Initialized,
	UnknownError,
};

class Client
{
public:
	Client();
	~Client();

	void Initialize();
	void Destroy();

	void SetServerAddressAndPort(std::string address, int port);
	bool SendUpdateToServer(InputMessage& data);
	bool CheckForUpdateFromGameServer();

	void Update(float deltaTime);
	int GetID() { return this->ID; }
	unsigned int g_MessageSendCount = 0;
	float px, pz, bx, bz;
	bool dead = false;

	bool waitForInputToBeSentToServer = false;
	GameState g_ServerGameState;
private:
	int ID = -1;
	int sendCount = 0;
	SOCKET m_ServerSocket;
	sockaddr_in m_AddrInfo;
	int m_AddressSize;
	ClientState m_ClientState;
	float m_NextSendTime;
	float m_NextRecvTime;
	double m_SendTimeDelta;
	float m_RecvTimeDelta;
	float m_CurrentTime;
};

