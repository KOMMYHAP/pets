#pragma once
// #include <map>
// #include <mutex>

#include "tools/Callback.h"
#include "PacketsId.pb.h"

class OperationManager;

namespace google {
	namespace protobuf {
		class Message;
	}
}

namespace Network
{
	struct ReceivedPacket;
	class PackageManager;
	class PeerConnection;

	template <class T>
	using TypedResponseCallback = TypedCallback<const T &>;
	using ResponseCallback = TypedCallback<const google::protobuf::Message &>;
	
	class Peer
	{
	public:
		Peer(PackageManager & packageManager, OperationManager & operationManager);
		~Peer();

		uint16_t OpenLocalConnection(const std::vector<uint16_t> & localPorts);
		bool OpenRemoteConnection(uint16_t remotePort, const std::string & ip); 
		void CloseRemoteConnection();

		uint16_t GetLocalPort() const;

		template <class T>
		void SendPacket(const T & request);
		
		template <class T>
		void Subscribe(TypedResponseCallback<T> response);

		void ProcessReceivedPackets();
		void ReceivePacket(ReceivedPacket receivedPacket);

	private:
		void SendPacket(uint32_t packageId, const google::protobuf::Message & request);
		void ReceivePacket(uint32_t packageId, const google::protobuf::Message & response);
		
		PackageManager &									_packageManager;
		OperationManager &									_operationManager;
		std::list<ResponseCallback>							_subscribers;
		std::unique_ptr<PeerConnection>						_connection;
		std::string											_buffer;
	};

	template <class T>
	void Peer::SendPacket(const T & request)
	{
		if constexpr (std::is_base_of_v<google::protobuf::Message, T>)
		{
			using namespace my::proto::package;
			PackageId packageId = T::GetDescriptor()->options().GetExtension(package_id);
			SendPacket(static_cast<uint32_t>(packageId), *static_cast<const google::protobuf::Message *>(&request));
		}
	}

	template <class T>
	void Peer::Subscribe(TypedResponseCallback<T> response)
	{
		auto owner = response.Owner();
		auto upcastedResponse = ResponseCallback(std::move(owner), [response = std::move(response)](const google::protobuf::Message & msg) 
		{
			if (const auto * castedMsg = dynamic_cast<const T *>(&msg))
			{
				response(*castedMsg);
			}
		});
		_subscribers.emplace_back(std::move(upcastedResponse));
	}
}
