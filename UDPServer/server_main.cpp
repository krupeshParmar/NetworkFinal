#include <conio.h>
#include <chrono>

#include "Server.h"

#define ESC 27

bool gameOver = false;

std::chrono::steady_clock::time_point lastUpdateTime;
std::chrono::steady_clock::time_point currentUpdateTime;
float currentTime = 0;
float nextNetworkSend = 0;
float sendDeltaTime = 0.1f;
unsigned int latestMessageId;

Server server;

float TimeDifference() {
	float deltaTimeInSeconds;

	lastUpdateTime = currentUpdateTime;
	currentUpdateTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = currentUpdateTime - lastUpdateTime;
	deltaTimeInSeconds = elapsed.count();

	return deltaTimeInSeconds;
}

void GetClientsInfo() {
	int count = 0;
	bool reading = true;
	while (reading) {
		int result = server.Receive();
		count += result;

		if (result == 0)
			reading = false;
	}
}

void Update() {
	if (TimeDifference() > 1) {
		//printf("Here =========================================== 1: \n");
		return;
	}

	if (TimeDifference() < 100) {
		currentTime += TimeDifference();
		//printf("Here =========================================== 2: %f  \n", TimeDifference());
		//printf("Here =========================================== 3: %f  \n", currentTime);
	}

	GetClientsInfo();

	server.UpdatePlayerState();

	if (currentTime < nextNetworkSend)
		return;

	nextNetworkSend = currentTime + sendDeltaTime;

	server.UpdateClients(latestMessageId);


}

void ProcessKeyboardInput() {
	if (_kbhit()) {
		int ch = _getch();
		if (ch == ESC)
			gameOver = true;
	}
}

int main()
{
	server.Initialize();

	while (!gameOver) {
		ProcessKeyboardInput();
		Update();
	}
	server.Shutdown();

	return 0;
}