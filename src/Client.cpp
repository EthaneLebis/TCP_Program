#include "../include/Client.h"
#include "../include/Authentication.h"

Client::Client(SOCKET _socket, const std::string& _clientIP)
	: _socket(_socket), clientIP(_clientIP) { }

std::string Client::processMessage(const std::string& _message) {
    std::istringstream iss(_message);
    std::string command, target, arg;

    iss >> command >> target >> arg;

    if (command.empty()) return ""; 

    if (command == "SET" && target == "/name") {
        if (arg.empty()) {
            return "ERROR: Missing name argument. Usage: SET /name <name>.";
        }

        Authentication user(arg);
        if (user.getId() != -1) {
            return "OK: User created with ID " + std::to_string(user.getId() - 1);
        }
        return "ERROR: Could not write user to database file.";
    }

    if (command == "GET" && target == "/name") {
        if (arg.empty()) {
            return "ERROR: Missing ID argument. Usage: GET /name <id>.";
        }

        try {
            int targetId = std::stoi(arg);
            std::string name = Authentication::getName(targetId);

            if (!name.empty()) {
                return "VALUE: " + name;
            }
            return "ERROR: User ID " + std::to_string(targetId) + " not found.";
        }
        catch (...) {
            return "ERROR: User ID must be a valid number.";
        }
    }

    return "ERROR: Unknown command. Supported commands: SET /name <name> | GET /name <id>";
}

void Client::run() {
	char buffer[1024];		// Only allow 1024 bytes that kb

	while (true) {
		// Takes client's input. The  - 1 is for '\0'

		int bytesReceived = recv(_socket, buffer, sizeof(buffer) - 1, 0);

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

		// "\n" should add like that, not inside the  send(). Because char* + char is not correct

		std::string response = processMessage(message) + "\n";

		send(_socket, response.c_str(), static_cast<int>(response.size()), 0);
	}
}