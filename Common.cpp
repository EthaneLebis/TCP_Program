#include "Common.h"

void Common::logger(const std::string& _message, int _status = 0) {
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm{ };
	localtime_s(&tm, &t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%H:%M:%S");

	std::string timeStamp = oss.str();

	std::cout << "[" << timeStamp << "] ";

	if (_status == 1)
		std::cout << "ERROR: ";

	std::cout << _message << "\n";

	return;
}