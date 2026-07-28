#include "Server.h"

Server::Server(int _port)
	: port(_port) { }

Server::~Server() {
	if (serverSocket != INVALID_SOCKET) {
		closesocket(serverSocket);
		serverSocket = INVALID_SOCKET;
	}

	// Should use this function each socket successfully called and complete. 

	WSACleanup(); 
}

void Server::start() {
	if (!initWinsock())		return;
	if (!createSocket())		return;
	if (!bindSocket())		return;
	if (!startListening())		return;

	isRunning = true;

	acceptLoop();
}

void Server::stop() {
	isRunning = false;
}

bool Server::initWinsock() {
	// WSADATA is information about windows socket

	WSADATA wsaData;

	// MAKEWORD(2, 2) means WinSock version 0x0202

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {	// If 0, initialized succesfully.
		Common::logger("WSAStartup() failed.", 1);

		return false;
	}

	Common::logger("WSAStartup() initialized.", 0);

	return true;
}

bool Server::createSocket() {
	// AF_INET for IPv4 addresses (standart 32-bit address)
	// AF_INET6 for IPv6 addresses

	// About Network Protocols 
	// https://inhandgo.com/blogs/articles/understanding-internet-protocols-http-https-tcp-udp-websocket-smtp-and-ftp
	// SOCK_STREAM for TCP slower, but better for transfers
	// SOCK_DGRAM for UDP (mainly for streaming, online games) faster, but not guaranteed transfer

	// 0 means protocol. It allows operating system to choose defaut protocol automatically (IPPROTO_TCP for these conditions)

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) {
		Common::logger("socket() failed: " + std::to_string(WSAGetLastError()), 1);

		return false;
	}

	int opt = 1;

	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt)) == SOCKET_ERROR) {
		Common::logger("setsocketpt() failed" + std::to_string(WSAGetLastError()), 0);
	}

	Common::logger("socket() initialized.", 0);

	return true;
}

bool Server::bindSocket() {
	sockaddr_in addr { };
	addr.sin_addr.s_addr = INADDR_ANY;	// INADDR_ANY is 0.0.0.0 
	addr.sin_family = AF_INET;			// Address family is IPv4
	addr.sin_port = htons(port);		// Host to network short(in CPU it is 0x2823).Must convert port with htons(). It converts 16-bit port number(9000) from host byte order to network byte order.

	if (bind(serverSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
		Common::logger("bind() failed: " + std::to_string(WSAGetLastError()), 1);

		return false;
	}

	Common::logger("bind() initialized: " + std::to_string(port), 0);

	return true;
}

bool Server::startListening() {
	// 5 means waiting ueue (backlog) (Example : this is waiting room chairs, accept() is host to take people to their tables, std::thread is waiter)
	// for real production servers should use SOMAXCONN. It is Win32 constand, maximum backlog queue size
	// For example if many people try to press same button, first 5 people will connect, remaining will drop

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		Common::logger("listen() failed: " + std::to_string(WSAGetLastError()), 1);

		return false;
	}

	Common::logger("Listening on port " + std::to_string(port), 0);

	return true;
}

void Server::acceptLoop() {
	while (isRunning) {
		sockaddr_in clientAddr { };
		int addrSize = sizeof(clientAddr);

		SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &addrSize);

		if (clientSocket == INVALID_SOCKET) {
			if (isRunning)
				Common::logger("accept() failed: " + std::to_string(WSAGetLastError()), 1);

			break;
		}

		//{
		//	char ip[INET_ADDRSTRLEN];

		//	// inet_ntop is network to presentation. Converts binary IP to string (192.168.1.45)

		//	inet_ntop(AF_INET, &clientAddr.sin_addr, ip, INET_ADDRSTRLEN);

		//	Common::logger("Client connected: " + std::string(ip), 0);

		//	handleClient(clientSocket, clientAddr);
		//}

		std::thread([this, clientSocket, clientAddr]() {
			handleClient(clientSocket, clientAddr);
		}).detach();
	}
}

void Server::handleClient(SOCKET _clientSocket, const sockaddr_in& _clientAddr) {
	char ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &_clientAddr.sin_addr, ip, INET_ADDRSTRLEN);

	unsigned short clientPort = ntohs(_clientAddr.sin_port);

	std::string clientId = std::string(ip) + ":" + std::to_string(clientPort);

	Common::logger("Client connected: " + clientId, 0);

	Client handler(_clientSocket, clientId);
	handler.run();

	closesocket(_clientSocket);

	Common::logger("Client disconnected: " + clientId, 0);
}