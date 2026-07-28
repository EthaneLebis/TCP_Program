#include <iostream>

#include "../include/Common.h"
#include "../include/Server.h"
#include "../include/Client.h"

int main() {
	Server server(9000);

	server.start();

	return 0;
}