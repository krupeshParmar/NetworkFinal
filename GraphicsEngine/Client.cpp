#include "Client.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"
#include <Windows.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) 

Client::Client()
	: m_ServerSocket(INVALID_SOCKET)
	, m_AddrInfo()
	, m_AddressSize(0)
	, m_SendTimeDelta(0.01f)
	, m_RecvTimeDelta(1 / 5.0f)
	, m_NextSendTime(0)
	, m_NextRecvTime(0)
	, m_CurrentTime(0)
{
	this->ID = -1;
}

Client::~Client()
{

}

void Client::Initialize()
{
	m_SendTimeDelta = glfwGetTime();
	if (m_ClientState == Initialized)
		return;
	// Initialization 
	WSADATA wsaData;
	int result;

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("NetworkClient::Initialize: WSAStartup failed with error %d\n", result);
		m_ClientState = UnknownError;
		return;
	}

	// Create our UDP Socket
	m_ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_ServerSocket == INVALID_SOCKET) {
		printf("NetworkClient::Initialize: socket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		m_ClientState = UnknownError;
		return;
	}

	// Immediately set the socket to be Non-Blocking
	DWORD NonBlock = 1;
	result = ioctlsocket(m_ServerSocket, FIONBIO, &NonBlock);
	if (result == SOCKET_ERROR) {
		printf("NetworkClient::Initialize: ioctlsocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		m_ClientState = UnknownError;
		return;
	}

	m_ClientState = Initialized;
}

void Client::Destroy()
{
	if (m_ClientState != Initialized)
		return;

	closesocket(m_ServerSocket);
	WSACleanup();
}

void Client::SetServerAddressAndPort(std::string address, int port)
{
	if (m_ClientState != Initialized)
		return;

	m_AddrInfo.sin_family = AF_INET;
	m_AddrInfo.sin_port = htons(port); 
	m_AddrInfo.sin_addr.s_addr = inet_addr(address.c_str());

	m_AddressSize = sizeof(m_AddrInfo);
}

bool Client::SendUpdateToServer(InputMessage& data)
{
	if (m_ClientState != Initialized)
		return false;

	if (glfwGetTime() - m_SendTimeDelta > 1)
	{
		sendCount = 0;
		m_SendTimeDelta = glfwGetTime();
	}
	else {
		if (sendCount > 5)
			return true;
	}

	/*if (m_CurrentTime < m_NextSendTime)
		return false;
	m_NextSendTime += m_SendTimeDelta;*/

	/*proto_game::PlayerState playerState;
	
	playerState.set_playerid(data.id);
	playerState.set_requestno(data.count);
	playerState.set_posx(data.posx);
	playerState.set_posz(data.posz);
	playerState.set_bulletposx(data.bulx);
	playerState.set_bulletposz(data.bulz);
	playerState.set_isshot(data.isshot);
	playerState.set_input(data.input_sum);
	std::string buffer = "";
	buffer = playerState.SerializeAsString();*/
	//std::cout << "PosX, Posz" << data.posx << ", " << data.posz << std::endl;
	/*data.posx = ceil(data.posx * 10.f) / 10.f;
	data.posz = ceil(data.posz * 10.f) / 10.f;*/

	int sendResult = sendto(
		m_ServerSocket,			// the SOCKET we created for our server
		(const char*)&data,		// The data we are sending
								// This is a POINTER to a UserInputMessage type
								// casted to a const char pointer.
		sizeof(InputMessage),			// The length of the message we are sending in bytes
		0,						// We are not setting any flags
		(SOCKADDR*)&m_AddrInfo, // The address of our server set in SetServerAddressAndPort
		m_AddressSize			// The length of our AddrInfo
	);
	if (sendResult > 8)
		int breakme = 0;
	if (sendResult == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			printf("NetworkClient::SendUpdateToServer: send failed with error: %d\n", WSAGetLastError());
			closesocket(m_ServerSocket);
			WSACleanup();
			m_ClientState = UnknownError;
			return false;
		}

		return false;
	}
	else if (sendResult > 0) {
		sendCount++;
		return true;
	}
	return false;
}

bool Client::CheckForUpdateFromGameServer()
{
	if (m_ClientState != Initialized)
		return false;

	//if (m_CurrentTime < m_NextRecvTime)
	//	return gameState;
	//m_NextRecvTime += m_RecvTimeDelta;

	const int BUF_SIZE = 512;	// Size of the buffer, that is the maximum we can recv in this call
	char buf[BUF_SIZE];

	int recvResult = recvfrom(m_ServerSocket, buf, BUF_SIZE, 0, (SOCKADDR*)&m_AddrInfo, &m_AddressSize);
	if (recvResult == SOCKET_ERROR)
	{
		int lastError = WSAGetLastError();
		if (lastError != WSAEWOULDBLOCK)
		{
			/*
			* The virtual circuit was reset by the remote side executing a hard or abortive close.
			* The application should close the socket; it is no longer usable. On a UDP-datagram
			* socket this error indicates a previous send operation resulted in an ICMP Port
			* Unreachable message.
			*/
			if (lastError == WSAECONNRESET) {
				printf("NetworkClient::CheckForUpdateFromGameServer: recv failed, the server was unreachable in the last send call.");
				m_ClientState = UnknownError;
				return false;
			}

			printf("NetworkClient::CheckForUpdateFromGameServer: recv failed with error: %d\n", WSAGetLastError());
			closesocket(m_ServerSocket);
			WSACleanup();
			m_ClientState = UnknownError;
		}

		return false;
	}

	/*proto_game::GameState game;
	game.ParseFromString(buf);*/
	/*std::string game_state = std::string(buf);
	int count = 0;
	std::cout << "GameState: " << game_state << std::endl;
	std::string ch = "s";*/
	memcpy(&g_ServerGameState, (const void*)buf, sizeof(GameState));
	if (g_ServerGameState.messageID == 1)
	{
		this->ID = g_ServerGameState.player1.id;
		std::cout << "I have " << this->ID << " ID" << std::endl;
	}
	//std::cout << "Player1 px: " << g_ServerGameState.player1.posx << ", px: " << g_ServerGameState.player1.posz << std::endl;
	return true;
	/*
	while (ch != "!" && ch != "")
	{
		if (ch == ".")
			count++;
		ch = game_state[count++];
		int no_players = std::stoi(ch);
		count++;
		ch = game_state[count++];
		float id = std::stof(ch);
		count++;
		int mes_count = game_state[count++];
		count++;
		ch = "";
		while (game_state[count] != ',')
		{
			ch += game_state[count++];
		}
		count++;
		float posx = std::stof(ch);
		ch = "";
		while (game_state[count] != ',')
		{
			ch += game_state[count++];
		}
		count++;
		float posz = std::stof(ch);
		ch = "";
		while (game_state[count] != ',')
		{
			ch += game_state[count++];
		}
		count++;
		float bulx = std::stof(ch);
		ch = "";
		while (game_state[count] != 'A' || game_state[count] != '.')
		{
			ch += game_state[count++];
		}
		float bulz = std::stof(ch);
		this->px = posx;
		this->pz = posz;
		this->bx = bulx;
		this->bz = bulz;
		std::cout << "PX" << this->px << std::endl;
		std::cout << "PZ" << this->pz << std::endl;
		if (id == this->ID)
		{
			
		}
	}
	*/
}

void Client::Update(float deltaTime)
{
	if (deltaTime > 100)
		deltaTime = 0;
	m_CurrentTime += deltaTime;
}
