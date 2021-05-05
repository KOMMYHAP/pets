#pragma once
#include <map>

class ParsedCommandLine
{
public:
	ParsedCommandLine(int argc, char ** argv);

	std::string_view GetRunCommand() const;

	bool		HasArgument(const std::string & argument) const;
	
	std::string GetOrDefault(const std::string & argument, const std::string & defaultValue) const;
	bool		GetBoolOrDefault(const std::string & argument, bool defaultValue) const;
	int64_t		GetIntOrDefault(const std::string & argument, int64_t defaultValue) const;
	std::string GetFreeArgument(size_t index) const;

private:
	std::string_view Get(const std::string & argument) const;
	
	std::map<std::string_view, std::string_view>	_arguments;
	std::vector<std::string_view>					_freeArguments;
	std::string_view								_runCommand;
};
