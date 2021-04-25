#include "NetworkOptions.h"

#include <iostream>

#include "json/json.h"

struct NetworkOptions::Impl
{
	Json::Value root;
};

NetworkOptions::NetworkOptions()
	: _impl(std::make_unique<Impl>())
{
}

NetworkOptions::NetworkOptions(const std::string& settings)
	: NetworkOptions()
{
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::String errors;
	auto reader = std::unique_ptr<Json::CharReader>(builder.newCharReader());
	bool parsed = reader->parse(settings.data(), settings.data() + settings.size(), &_impl->root, &errors);
	if (!parsed)
	{
		std::cerr << "Cannot parse network options. Details: " << errors << std::endl;
	}
}

NetworkOptions::~NetworkOptions() = default;

std::string NetworkOptions::ToStyledString() const
{
	return _impl->root.toStyledString();
}

std::string NetworkOptions::GetString(const std::string& field, const std::string& defaultValue)
{
	auto value = _impl->root.get(field, Json::nullValue);
	if (value.isString())
	{
		return value.asString();
	}
	return defaultValue;
}

int64_t NetworkOptions::GetInt(const std::string& field, int64_t defaultValue)
{
	auto value = _impl->root.get(field, Json::nullValue);
	if (value.isIntegral())
	{
		return value.asInt64();
	}
	return defaultValue;
}
