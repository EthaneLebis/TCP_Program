#pragma once

#include "Common.h"

class Server {
	int		port;
	SOCKET	serverSocket = INVALID_SOCKET;
	int		isRunning = false;

	bool initWinsock();
	bool createSocket();
	bool bindSocket();
	bool startListening();
	void acceptLoop();
	void handleClient(SOCKET _clientSocket, const sockaddr_in& clientAddr);

	public:

		explicit Server(int _port);

		~Server();

		void start();

		void stop();
};