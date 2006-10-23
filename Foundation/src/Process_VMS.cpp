//
// Process_VMS.cpp
//
// $Id: //poco/1.2/Foundation/src/Process_VMS.cpp#1 $
//
// Library: Foundation
// Package: Processes
// Module:  Process
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


#include "Poco/Process_VMS.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NamedEvent.h"
#include <times.h>
#include <time.h>


namespace Poco {


//
// ProcessHandleImpl
//
ProcessHandleImpl::ProcessHandleImpl(pid_t pid):
	_pid(pid)
{
}


ProcessHandleImpl::~ProcessHandleImpl()
{
}


pid_t ProcessHandleImpl::id() const
{
	return _pid;
}


int ProcessHandleImpl::wait() const
{
	int status;
	if (waitpid(_pid, &status, 0) != _pid)
		throw SystemException("Cannot wait for process", NumberFormatter::format(_pid));
	return WEXITSTATUS(status);
}


//
// ProcessImpl
//
ProcessImpl::PIDImpl ProcessImpl::idImpl()
{
	return getpid();
}


void ProcessImpl::timesImpl(long& userTime, long& kernelTime)
{
	struct tms buffer;
	times(&buffer)*1000/CLOCKS_PER_SEC;
	userTime   = buffer.tms_utime/CLOCKS_PER_SEC;
	kernelTime = buffer.tms_stime/CLOCKS_PER_SEC;
}


ProcessHandleImpl* ProcessImpl::launchImpl(const std::string& command, const ArgsImpl& args, Pipe* inPipe, Pipe* outPipe, Pipe* errPipe)
{
	char** argv = new char*[args.size() + 2];
	int i = 0;
	argv[i++] = const_cast<char*>(command.c_str());
	for (ArgsImpl::const_iterator it = args.begin(); it != args.end(); ++it) 
		argv[i++] = const_cast<char*>(it->c_str());
	argv[i] = NULL;
	try
	{
		int pid = vfork();
		if (pid < 0)
		{
			throw SystemException("Cannot fork process for", command);		
		}
		else if (pid == 0)
		{
			if (execvp(command.c_str(), argv) == -1)
				throw SystemException("Cannot execute command", command);
		}
		else 
		{
			delete [] argv;
			return new ProcessHandleImpl(pid);
		}
	}
	catch (...)
	{
		delete [] argv;
		throw;
	}
}


void ProcessImpl::killImpl(PIDImpl pid)
{
	if (kill(pid, SIGKILL) != 0)
	{
		switch (errno)
		{
		case ESRCH:
			throw NotFoundException("cannot kill process");
		case EPERM:
			throw NoPermissionException("cannot kill process");
		default:
			throw SystemException("cannot kill process");
		}
	}
}


void ProcessImpl::requestTerminationImpl(PIDImpl pid)
{
	std::string evName("POCOTRM");
	evName.append(NumberFormatter::formatHex(pid, 8));
	NamedEvent ev(evName);
	ev.set();
}


} // namespace Poco
