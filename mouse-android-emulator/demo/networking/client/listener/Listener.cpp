#include "NetworkInterface.h"
#include "NetworkOptions.h"

int main()
{
	auto options = std::make_unique<NetworkOptions>();
	auto networkInterface = NetworkInterface(std::move(options));
	return 0;
}
