#pragma once
#include <functional>
#include <map>
#include <memory>

namespace google {
	namespace protobuf {
		class Message;
	}
}

namespace Network
{
	using PackageCreator = std::function<std::unique_ptr<google::protobuf::Message>()>;
	
	class PackageManager
	{
	public:
		void RegisterPackages();
		
		std::unique_ptr<google::protobuf::Message> Create(uint32_t id);

	private:
		std::map<uint32_t, PackageCreator>	_creators;
	};
}
