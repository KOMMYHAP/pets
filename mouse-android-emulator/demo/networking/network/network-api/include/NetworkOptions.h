#pragma once

#include <string>

class NetworkOptions
{
public:
	NetworkOptions();
	NetworkOptions(const std::string & data);
	~NetworkOptions();
	
	std::string ToStyledString() const;

	std::string		GetString(const std::string & field, const std::string & defaultValue);
	int64_t			GetInt(const std::string & field, int64_t defaultValue);

private:
	struct Impl;
	std::unique_ptr<Impl>		_impl;
};
