#include "tools/JsonConfig.h"

#include "json/json.h"

struct JsonConfig::Impl
{
	Json::Value root;
	std::string cachedStyledView;
};

JsonConfig::JsonConfig()
	: _impl(std::make_unique<Impl>())
{	
}

JsonConfig::JsonConfig(const std::string& jsonData)
{
	ParseJson(jsonData);
}

JsonConfig::~JsonConfig() = default;

bool JsonConfig::Good() const
{
	return _status == Status::NoError;
}

JsonConfig::Status JsonConfig::GetStatus() const
{
	return _status;
}

const std::string& JsonConfig::GetParseErrors() const
{
	return _parseErrors;
}

int64_t JsonConfig::GetInt(const std::string& field, int64_t defaultValue)
{
	auto value = _impl->root.get(field, Json::nullValue);
	if (value.isString())
	{
		return value.asInt64();
	}
	return defaultValue;
}

std::string JsonConfig::GetString(const std::string& field, const std::string& defaultValue)
{
	auto value = _impl->root.get(field, Json::nullValue);
	if (value.isString())
	{
		return value.asString();
	}
	return defaultValue;
}

void JsonConfig::ParseJson(const std::string& buffer)
{
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	auto reader = std::unique_ptr<Json::CharReader>(builder.newCharReader());
	bool parsed = reader->parse(buffer.data(), buffer.data() + buffer.size(), &_impl->root, &_parseErrors);
	if (!parsed)
	{
		_status = Status::IncorrectData;
		return;
	}
	_status = Status::NoError;
}

const std::string & JsonConfig::Serialize() const
{
	if (_impl->cachedStyledView.empty())
	{
		_impl->cachedStyledView = _impl->root.toStyledString();
	}
	return _impl->cachedStyledView;
}
