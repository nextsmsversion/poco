//
// SharedLibrary.cpp
//
// $Id: //poco/1.2/Foundation/src/SharedLibrary.cpp#1 $
//
// Library: Foundation
// Package: SharedLibrary
// Module:  SharedLibrary
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/SharedLibrary.h"
#include "Poco/Exception.h"


#if defined(hpux) || defined(_hpux)
#include "SharedLibrary_HPUX.cpp"
#elif defined(POCO_OS_FAMILY_UNIX)
#include "SharedLibrary_UNIX.cpp"
#elif defined(POCO_OS_FAMILY_WINDOWS) && defined(POCO_WIN32_UTF8)
#include "SharedLibrary_WIN32U.cpp"
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "SharedLibrary_WIN32.cpp"
#elif defined(POCO_OS_FAMILY_VMS)
#include "SharedLibrary_VMS.cpp"
#endif


namespace Poco {


SharedLibrary::SharedLibrary()
{
}


SharedLibrary::SharedLibrary(const std::string& path)
{
	loadImpl(path);
}


SharedLibrary::~SharedLibrary()
{
}


void SharedLibrary::load(const std::string& path)
{
	loadImpl(path);
}


void SharedLibrary::unload()
{
	unloadImpl();
}


bool SharedLibrary::isLoaded() const
{
	return isLoadedImpl();
}


bool SharedLibrary::hasSymbol(const std::string& name)
{
	return findSymbolImpl(name) != 0;
}


void* SharedLibrary::getSymbol(const std::string& name)
{
	void* result = findSymbolImpl(name);
	if (result)
		return result;
	else
		throw NotFoundException(name);
}


const std::string& SharedLibrary::getPath() const
{
	return getPathImpl();
}


std::string SharedLibrary::suffix()
{
	return suffixImpl();
}


} // namespace Poco
