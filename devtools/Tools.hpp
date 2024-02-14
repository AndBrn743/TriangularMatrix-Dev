//
// Created by Andy on 2/15/2024.
//

#pragma once

#include <string>

#if defined(__has_include) && __has_include(<cxxabi.h>)
#include <cxxabi.h>
#endif


template <typename T>
inline std::string TypeName()
{
#if defined(__has_include) && __has_include(<cxxabi.h>)
	int status;
	const std::string mangledName = typeid(T).name();
	const char* name = abi::__cxa_demangle(mangledName.c_str(), nullptr, nullptr, &status);
	if (status != 0)
	{
		return "UnknownType";
	}
	return name;
#else
	return typeid(T).name();
#endif
}
