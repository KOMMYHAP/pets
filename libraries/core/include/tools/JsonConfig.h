#pragma once

class JsonConfig
{
public:
	enum class Status
	{
		IncorrectData,
		NotInitialized,
		NoError
	};

	JsonConfig();
	JsonConfig(const std::string & jsonData);
	~JsonConfig();

	const std::string & Serialize() const;

	bool Good() const;
	Status GetStatus() const;
	const std::string & GetParseErrors() const;

	int64_t GetInt(const std::string& field, int64_t defaultValue);
	std::string GetString(const std::string& field, const std::string& defaultValue);

private:
	void ParseJson(const std::string& buffer);

	struct Impl;
	std::unique_ptr<Impl> 	_impl;
	Status					_status = Status::NotInitialized;
	std::string				_parseErrors;
};
