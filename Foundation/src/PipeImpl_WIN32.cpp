//
// PipeImpl_WIN32.cpp
//
// $Id: //poco/1.2/Foundation/src/PipeImpl_WIN32.cpp#1 $
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


#include "Poco/PipeImpl_WIN32.h"
#include "Poco/Exception.h"


namespace Poco {


PipeImpl::PipeImpl()
{
	SECURITY_ATTRIBUTES attr;
	attr.nLength              = sizeof(attr);
	attr.lpSecurityDescriptor = NULL;
	attr.bInheritHandle       = FALSE;
	
	if (!CreatePipe(&_readHandle, &_writeHandle, &attr, 0))
		throw CreateFileException("anonymous pipe");
}


PipeImpl::~PipeImpl()
{
	closeRead();
	closeWrite();
}


int PipeImpl::writeBytes(const void* buffer, int length)
{
	poco_assert (_writeHandle != INVALID_HANDLE_VALUE);

	DWORD bytesWritten = 0;
	if (!WriteFile(_writeHandle, buffer, length, &bytesWritten, NULL))
		throw WriteFileException("anonymous pipe");
	return bytesWritten;
}


int PipeImpl::readBytes(void* buffer, int length)
{
	poco_assert (_readHandle != INVALID_HANDLE_VALUE);

	DWORD bytesRead = 0;
	BOOL ok = ReadFile(_readHandle, buffer, length, &bytesRead, NULL);
	if (ok || GetLastError() == ERROR_BROKEN_PIPE)
		return bytesRead;
	else
		throw ReadFileException("anonymous pipe");
}


PipeImpl::Handle PipeImpl::readHandle() const
{
	return _readHandle;
}


PipeImpl::Handle PipeImpl::writeHandle() const
{
	return _writeHandle;
}


void PipeImpl::closeRead()
{
	if (_readHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_readHandle);
		_readHandle = INVALID_HANDLE_VALUE;
	}
}


void PipeImpl::closeWrite()
{
	if (_writeHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_writeHandle);
		_writeHandle = INVALID_HANDLE_VALUE;
	}
}


} // namespace Poco
