
#include "PackageManager.h"
#include "PacketsId.pb.h"
#include "Main.pb.h"
#include "Connection.pb.h"

namespace 
{
	template <class T>
	void RegisterPackage(std::map<uint32_t, Network::PackageCreator> & creators)
	{
		using namespace my::proto::package;
		PackageId packageId = T::GetDescriptor()->options().GetExtension(package_id);
		creators[static_cast<int>(packageId)] = [](){ return std::make_unique<T>(); };
	}
}

namespace Network
{
	void PackageManager::RegisterPackages()
	{
		using namespace my::proto::package;
		RegisterPackage<ConnectionRequest>(_creators);
		RegisterPackage<ConnectionResponse>(_creators);
		RegisterPackage<MousePositionMessage>(_creators);
	}

	std::unique_ptr<google::protobuf::Message> PackageManager::Create(uint32_t id)
	{
		auto it = _creators.find(id);
		if (it != _creators.end())
		{
			auto & creator = it->second;
			return creator();
		}
		return nullptr;
	}
}
