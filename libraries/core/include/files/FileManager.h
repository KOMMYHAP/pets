#pragma once
#include <string>

class FileManager
{
public:
	FileManager(const std::string & root);
	
	std::ifstream & ReadFile(const std::string & name);
	std::ofstream & WriteFile(const std::string & name);

private:
	const std::string & GetRoot() const;

	std::string		_root;
};
