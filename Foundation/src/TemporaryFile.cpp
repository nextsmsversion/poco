//
// TemporaryFile.cpp
//
// $Id: //poco/1.2/Foundation/src/TemporaryFile.cpp#1 $
//
// Library: Foundation
// Package: Filesystem
// Module:  TemporaryFile
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


#include "Poco/TemporaryFile.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include "Poco/Process.h"
#include "Poco/Mutex.h"
#include <set>
#include <sstream>


namespace Poco {


class TempFileCollector
{
public:
	TempFileCollector()
	{
	}
	
	~TempFileCollector()
	{
		for (std::set<std::string>::iterator it = _files.begin(); it != _files.end(); ++it)
		{
			try
			{
				File f(*it);
				f.remove(true);
			}
			catch (Exception&)
			{
			}
		}
	}
	
	void registerFile(const std::string& path)
	{
		FastMutex::ScopedLock lock(_mutex);

		Path p(path);
		_files.insert(p.absolute().toString());
	}
	
private:
	std::set<std::string> _files;
	FastMutex             _mutex;
};


TemporaryFile::TemporaryFile(): File(tempName()), _keep(false)
{
}


TemporaryFile::~TemporaryFile()
{
	if (!_keep)
	{
		try
		{
			remove(true);
		}
		catch (Exception&)
		{
		}
	}
}


void TemporaryFile::keep()
{
	_keep = true;
}

	
void TemporaryFile::keepUntilExit()
{
	_keep = true;
	registerForDeletion(path());
}


void TemporaryFile::registerForDeletion(const std::string& path)
{
	static TempFileCollector fc;
	fc.registerFile(path);
}


std::string TemporaryFile::tempName()
{
	std::ostringstream name;
	static FastMutex mutex;
	static unsigned long count = 0;
	mutex.lock();
	unsigned long n = count++;
	mutex.unlock();
	name << Path::temp();
	name << "tmp" << Process::id();
	for (int i = 0; i < 6; ++i)
	{
		name << char('a' + (n % 26));
		n /= 26;
	}
	return name.str();
}


} // namespace Poco
