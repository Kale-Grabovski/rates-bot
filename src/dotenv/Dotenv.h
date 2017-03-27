#ifndef DOTENV_H
#define DOTENV_H

#include <string>
#include <map>

class Dotenv 
{
public:
	std::string get(std::string);

private:
	void readFile();

	std::string filename = ".env";
	std::map<std::string, std::string> settings;
};

#endif // DOTENV_H
