#include "files/FileManager.h"

#include <fstream>

FileManager::FileManager(const std::string& root)
	: _root(root)
{
	if (_root.empty())
	{
		_root = "./";
	}
	else
	{
		for (char & c : _root)
		{
			if (c == '\\')
			{
				c = '/';
			}
		}
		if (_root.back() != '/')
		{
			_root += "/";
		}
	}
}

std::ifstream& FileManager::ReadFile(const std::string& name)
{
	std::ifstream ifs(_root + name);
	return ifs;
}

std::ofstream& FileManager::WriteFile(const std::string& name)
{
	std::ofstream ofs(_root + name);
	return ofs;
}
