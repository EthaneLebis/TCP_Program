#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

#include "Client.h"

class Common {

	public:

		static void logger(const std::string& _message, int _status);
};