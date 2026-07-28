#pragma once

#include "Common.h"

class Authentication {
	int id;
	std::string name;

	static inline const std::string DB_PATH = "database/users.txt";
	static inline std::mutex dbMutex;

	int saveUser(std::string _name);

	int getNextId();
	
public:

		Authentication(std::string _name);

		int getId() { return id; }

		static std::string getName(int _id);
};