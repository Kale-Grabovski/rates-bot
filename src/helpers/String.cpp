#include <vector>
#include <string>
#include "String.h"

std::vector<std::string> String::split(const std::string str, const std::string delim) {
	auto i = 0;
	auto pos = str.find(delim);
	std::vector<std::string> chunks;
	
	while (pos != std::string::npos) {
		chunks.push_back(str.substr(i, pos - i));

		i = ++pos;
		pos = str.find(delim, pos);

		if (pos == std::string::npos) {
			chunks.push_back(str.substr(i, str.length()));
		}
	}

	return chunks;
}
