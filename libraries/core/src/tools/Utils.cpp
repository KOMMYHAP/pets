#include "tools/Utils.h"

#include <iostream>

#ifdef WIN32
#include "windows.h"
#include "processthreadsapi.h"
namespace 
{
	void SetThreadNameImpl(HANDLE handle, const std::string & name)
	{
		DWORD id = GetThreadId(handle);
		if (id != 0)
		{
			auto ws = std::make_unique<wchar_t[]>(name.size() + 1);
			mbstowcs_s(nullptr, ws.get(), name.size() + 1, name.c_str(), name.size());
			HRESULT result = SetThreadDescription(handle, ws.get());
			if (FAILED(result))
			{
				std::cerr << "Cannot set a thread name \"" << name << "\": error code " << GetLastError() << std::endl;
			}
		}
	}
}
#else
namespace
{
	void SetThreadNameImpl(std::thread::native_handle_type handle, const std::string& name)
	{
	}

	std::thread::native_handle_type GetCurrentThread()
	{
		return {};
	}
}
#endif

namespace Utils
{
	void SetThreadName(const std::string& name)
	{
#ifdef TRACY_ENABLE
		tracy::SetThreadName(name.c_str());
#else
		SetThreadNameImpl(GetCurrentThread(), name);
#endif	
	}
}
