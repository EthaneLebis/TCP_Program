#include <iostream>

#include "Common.h"
#include "Server.h"
#include "Client.h"

int main() {
	Server server(9000);

	server.start();

	return 0;
}