#pragma once

#include "Common.h"

class Authentication {
	int id;
	std::string name;

	static inline const std::string DB_PATH = "/Projects/TCP_Program/database/users.txt";
	static inline std::mutex dbMutex;

	int saveUser(std::string _name);
	
public:

		Authentication(std::string _name);


		int getId();

		static std::string getName(int _id);

		int getId() const { return id; }
};