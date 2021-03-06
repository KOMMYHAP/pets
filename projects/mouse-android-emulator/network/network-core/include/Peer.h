#pragma once
#include "tools/Callback.h"
#include "PacketsId.pb.h"

class OperationManager;

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
		std::map<uint32_t, std::vector<ResponseCallback>>	_subscribers;
		std::unique_ptr<PeerConnection>						_connection;
		std::string											_buffer;
	};

	template<class Packet>
	uint32_t GetPackageId()
	{
		using namespace my::proto::package;
		return static_cast<uint32_t>(Packet::GetDescriptor()->options().GetExtension(package_id));
	}

	template <class T>
	void Peer::SendPacket(const T & request)
	{
		if constexpr (std::is_base_of_v<google::protobuf::Message, T>)
		{
			using namespace my::proto::package;
			SendPacket(GetPackageId<T>(), *static_cast<const google::protobuf::Message *>(&request));
		}
		else
		{
			static_assert(Utils::always_false_v<T>, "Packet must be derived of google::protobuf::Message.");
		}
	}

	template <class T>
	void Peer::Subscribe(TypedResponseCallback<T> response)
	{
		if constexpr (!std::is_base_of_v<google::protobuf::Message, T>)
		{
			static_assert(Utils::always_false_v<T>, "Packet must be derived of google::protobuf::Message.");
		}
		auto owner = response.Owner();
		auto upcastedResponse = ResponseCallback(std::move(owner), [response = std::move(response)](const google::protobuf::Message & msg) 
		{
			if (const T * castedMsg = static_cast<const T *>(&msg))
			{
				response(*castedMsg);
			}
		});

		std::vector<ResponseCallback> & callbacks = _subscribers[GetPackageId<T>()];
		callbacks.emplace_back(std::move(upcastedResponse));
	}
}
