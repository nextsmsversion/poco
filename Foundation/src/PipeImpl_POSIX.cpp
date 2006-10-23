//
// PipeImpl_POSIX.cpp
//
// $Id: //poco/1.2/Foundation/src/PipeImpl_POSIX.cpp#2 $
//
// Library: Foundation
// Package: Processes
// Module:  PipeImpl
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


#include "Poco/PipeImpl_POSIX.h"
#include "Poco/Exception.h"
#include <sys/types.h>
#include <unistd.h>


namespace Poco {


PipeImpl::PipeImpl()
{
	int fds[2];
	int rc = pipe(fds);
	if (rc == 0)
	{
		_readfd  = fds[0];
		_writefd = fds[1];
	}
	else throw CreateFileException("anonymous pipe");
}


PipeImpl::~PipeImpl()
{
	closeRead();
	closeWrite();
}


int PipeImpl::writeBytes(const void* buffer, int length)
{
	poco_assert (_writefd != -1);

	int n;
	do
	{
		n = write(_writefd, buffer, length);
	}
	while (n < 0 && errno == EINTR);
	if (n >= 0)
		return n;
	else
		throw WriteFileException("anonymous pipe");
}


int PipeImpl::readBytes(void* buffer, int length)
{
	poco_assert (_readfd != -1);

	int n;
	do
	{
		n = read(_readfd, buffer, length);
	}
	while (n < 0 && errno == EINTR);
	if (n >= 0)
		return n;
	else
		throw ReadFileException("anonymous pipe");
}


PipeImpl::Handle PipeImpl::readHandle() const
{
	return _readfd;
}


PipeImpl::Handle PipeImpl::writeHandle() const
{
	return _writefd;
}


void PipeImpl::closeRead()
{
	if (_readfd != -1)
	{
		close(_readfd);
		_readfd = -1;
	}
}


void PipeImpl::closeWrite()
{
	if (_writefd != -1)
	{
		close(_writefd);
		_writefd = -1;
	}
}


} // namespace Poco
