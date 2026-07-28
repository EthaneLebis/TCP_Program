#include "Client.h"

Client::Client(SOCKET _socket, const std::string& _clientIP)
	: socket(_socket), clientIP(_clientIP) { }

std::string Client::processMessage(const std::string& _message) {
	return _message;

	// To check GET, POST and other things based on string slice and call functions based on these 
}

void Client::run() {
	char buffer[1024];		// Only allow 1024 bytes that kb

	while (true) {
		// Takes client's input. The  - 1 is for '\0'

		int bytesReceived = recv(socket, buffer, sizeof(buffer) - 1, 0);

		// If bytesReceived is 0, means disconnected
		// If euals minus that SOCKET_ERROR (-1), means network failure. Can cause timed out, crashed

		if (bytesReceived <= 0) {
			break;
		}

		buffer[bytesReceived] = '\0';		// Making C style array, ends with '\0'
		std::string message(buffer, bytesReceived);		// Makes exact byte count 

		while (!message.empty() && (message.back() == '\r' || message.back() == '\n'))
			message.pop_back();

		if (!message.empty()) 
			Common::logger("[ " + clientIP + " ] " + message, 0);

		std::string response = processMessage(message);

		send(socket, response.c_str() + '\n', static_cast<int>(response.size()), 0);
	}
}