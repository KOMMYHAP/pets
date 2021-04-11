#include "ReceivedPacket.h"

namespace Network
{
	ReceivedPacket::ReceivedPacket(uint32_t id, std::string data) noexcept
		: id(id)
		, data(std::move(data))
	{}
}
