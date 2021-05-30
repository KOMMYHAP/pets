#include "tools/ParsedCommandLine.h"
#include <cerrno>
namespace 
{
	enum class ParserState
	{
		ArgumentLong,
		ArgumentShort,
		Value
	};
}

ParsedCommandLine::ParsedCommandLine(int argc, char** argv)
{
	_runCommand = argv[0];

	auto state = ParserState::Value;
	std::string_view lastArgument;
	for (int i = 1; i < argc; ++i)
	{
		std::string_view token = argv[i];
		if (token.length() >= 1 && token[0] == '-')
		{
			if (token.length() >= 2 && token[1] == '-')
			{
				state = ParserState::ArgumentLong;
				token = token.substr(2);
			}
			else
			{
				state = ParserState::ArgumentShort;
				token = token.substr(1);
			}

			if (token.empty())
			{
				continue;
			}
		}
		else
		{
			state = ParserState::Value;
		}

		if (state == ParserState::Value)
		{
			if (!lastArgument.empty())
			{
				_arguments[lastArgument] = token;
				lastArgument = std::string_view();
			}
			else
			{
				_freeArguments.emplace_back(token);
			}
		}
		else
		{
			if (!lastArgument.empty())
			{
				_arguments[lastArgument];
			}

			std::string_view argumentName = token;
			std::string_view argumentValue;
			if (state == ParserState::ArgumentLong)
			{
				auto valueOffset = token.find('=');
				if (valueOffset != token.npos)
				{
					if (valueOffset + 1 < token.length())
					{
						argumentValue = token.substr(valueOffset + 1);
					}
					argumentName = token.substr(0, valueOffset);
				}
			}
			_arguments[argumentName] = argumentValue;
		}
	}
}

std::string_view ParsedCommandLine::GetRunCommand() const
{
	return _runCommand;
}

bool ParsedCommandLine::HasArgument(const std::string& argument) const
{
	return _arguments.count(argument) == 1;
}

std::string ParsedCommandLine::GetOrDefault(const std::string& argument, const std::string& defaultValue) const
{
	std::string_view value = Get(argument);
	return !value.empty() ? std::string(value) : defaultValue;
}

bool ParsedCommandLine::GetBoolOrDefault(const std::string& argument, bool defaultValue) const
{
	std::string_view value = Get(argument);
	if (value == "true" || value == "1" || value == "yes")
	{
		return true;
	}
	return value.empty() && defaultValue;
}

int64_t ParsedCommandLine::GetIntOrDefault(const std::string& argument, int64_t defaultValue) const
{
	std::string_view value = Get(argument);
	if (value.empty())
	{
		return defaultValue;
	}

	char * valueEnd = nullptr;
	errno = 0;
	const int64_t number = std::strtoll(value.data(), &valueEnd, 10);
	const bool noError = errno == 0;
	
	const bool exactlyMatch = valueEnd == value.data() + value.length();
	if (noError && exactlyMatch)
	{
		return number;
	}
	return defaultValue;
}

std::string ParsedCommandLine::GetFreeArgument(size_t index) const
{
	if (index < _freeArguments.size())
	{
		return std::string(_freeArguments[index]);
	}
	return {};
}

std::string_view ParsedCommandLine::Get(const std::string& argument) const
{
	auto it = _arguments.find(argument);
	if (it != _arguments.end())
	{
		return it->second;
	}
	return {};
}
