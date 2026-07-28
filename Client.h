#pragma once

#include "Common.h"

class Client {
	SOCKET socket;
	std::string clientIP;

	std::string processMessage(const std::string& _message);

	public:

		explicit Client(SOCKET _socket, const std::string& clientIP);
		
		void run();
};