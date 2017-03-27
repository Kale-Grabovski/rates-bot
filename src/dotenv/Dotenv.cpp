#include "Dotenv.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

// Returns dotenv property by passed key
std::string Dotenv::get(std::string property) {
	if (settings.size() == 0) {
		readFile();
	}

	return settings[property];
};

// Read dotenv file and save the config into settings property
void Dotenv::readFile() {
	std::string line;
	std::ifstream infile(filename);

	while (infile >> line) {
		settings[line.substr(0, line.find('='))] = line.substr(line.find('=') + 1);
	}
}
