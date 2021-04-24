#pragma once

namespace Utils
{
	void SetThreadName(std::thread::native_handle_type handle, const std::string & name);
	void SetCurrentThreadName(const std::string & name);
}
