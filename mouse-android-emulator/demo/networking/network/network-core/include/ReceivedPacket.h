#pragma once
#include <string>

namespace Network
{
	struct ReceivedPacket
	{
		ReceivedPacket() = default;
		ReceivedPacket(uint32_t id, std::string data) noexcept;
		
		const uint32_t			id = 0;
		const std::string		data;
	};
}
