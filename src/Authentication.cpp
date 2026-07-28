#include "../include/Authentication.h"

static void ensureDirectoryExists(const std::string& filePath) {
	std::filesystem::path path(filePath);
	if (path.has_parent_path()) {
		std::filesystem::create_directories(path.parent_path());
	}
}

Authentication::Authentication(std::string _name)
	: name(_name) { 
	
	this -> id = saveUser(name);
}

int Authentication::getNextId() {
	std::ifstream userFile(DB_PATH);
	if (!userFile.is_open()) return 1;
	std::string line;
	int maxId = 0;
	while (std::getline(userFile, line)) {
		std::istringstream iss(line);
		int currentId;
		if (iss >> currentId && currentId > maxId)
			maxId = currentId;
	}
	return maxId + 1;
}


int Authentication::saveUser(std::string _name) {
	std::lock_guard<std::mutex> lock(dbMutex);

	int newId = getNextId();

	ensureDirectoryExists(DB_PATH);

	std::ofstream createUserFile(DB_PATH, std::ios::app);
	if (!createUserFile.is_open()) {
		std::cerr << "Could not open file at path: " << DB_PATH << "\n";
		return -1;
	}

	createUserFile << newId << " " << _name << "\n";
	createUserFile.flush();

	return newId;
}

std::string Authentication::getName(int _id) {
	std::lock_guard<std::mutex> lock(dbMutex);

	ensureDirectoryExists(DB_PATH);

	std::ifstream userFile(DB_PATH);
	if (!userFile.is_open()) return "";

	std::string line;

	while (std::getline(userFile, line)) {
		if (line.empty()) continue;

		std::istringstream iss(line);
		int currentId;
		std::string userName;

		if (iss >> currentId >> userName) {
			if (currentId == _id) {
				return userName;
			}
		}
	}

	return "";
}