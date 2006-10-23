//
// InflatingStream.cpp
//
// $Id: //poco/1.2/Foundation/src/InflatingStream.cpp#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ZLibStream
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


#include "Poco/InflatingStream.h"
#include "Poco/Exception.h"


namespace Poco {


InflatingStreamBuf::InflatingStreamBuf(std::istream& istr, StreamType type): 
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::in),
	_pIstr(&istr),
	_pOstr(0),
	_eof(false)
{
	_zstr.zalloc    = Z_NULL;
	_zstr.zfree     = Z_NULL;
	_zstr.opaque    = Z_NULL;
	_zstr.next_in   = 0;
	_zstr.avail_in  = 0;
	_zstr.next_out  = 0;
	_zstr.avail_out = 0;

	int rc = inflateInit2(&_zstr, 15 + (type == STREAM_GZIP ? 16 : 0));
	if (rc != Z_OK) throw IOException(zError(rc)); 

	_buffer = new char[INFLATE_BUFFER_SIZE];
}


InflatingStreamBuf::InflatingStreamBuf(std::ostream& ostr, StreamType type): 
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::out),
	_pIstr(0),
	_pOstr(&ostr),
	_eof(false)
{
	_zstr.zalloc    = Z_NULL;
	_zstr.zfree     = Z_NULL;
	_zstr.opaque    = Z_NULL;
	_zstr.next_in   = 0;
	_zstr.avail_in  = 0;
	_zstr.next_out  = 0;
	_zstr.avail_out = 0;

	int rc = inflateInit2(&_zstr, 15 + (type == STREAM_GZIP ? 16 : 0));
	if (rc != Z_OK) throw IOException(zError(rc));

	_buffer = new char[INFLATE_BUFFER_SIZE];
}


InflatingStreamBuf::~InflatingStreamBuf()
{
	try
	{
		close();
	}
	catch (...)
	{
	}
	delete [] _buffer;
}


int InflatingStreamBuf::close()
{
	sync();
	if (_pIstr || _pOstr)
	{
		int rc = inflateEnd(&_zstr);
		if (rc != Z_OK) throw IOException(zError(rc));
		_pIstr = 0;
		_pOstr = 0;
	}
	return 0;
}


int InflatingStreamBuf::readFromDevice(char* buffer, std::streamsize length)
{
	if (_eof || !_pIstr) return 0;

	if (_zstr.avail_in == 0)
	{
		int n = 0;
		if (_pIstr->good())
		{
			_pIstr->read(_buffer, INFLATE_BUFFER_SIZE);
			n = _pIstr->gcount();
		}
		if (n == 0) return 0;
		_zstr.next_in   = (unsigned char*) _buffer;
		_zstr.avail_in  = n;
	}
	_zstr.next_out  = (unsigned char*) buffer;
	_zstr.avail_out = length;
	for (;;)
	{
		int rc = inflate(&_zstr, Z_NO_FLUSH);
		if (rc == Z_STREAM_END)
		{
			_eof = true;
			return length - _zstr.avail_out;
		}
		if (rc != Z_OK) throw IOException(zError(rc));
		if (_zstr.avail_out == 0)
			return length;
		if (_zstr.avail_in == 0)
		{
			int n = 0;
			if (_pIstr->good())
			{
				_pIstr->read(_buffer, INFLATE_BUFFER_SIZE);
				n = _pIstr->gcount();
			}
			if (n > 0)
			{
				_zstr.next_in  = (unsigned char*) _buffer;
				_zstr.avail_in = n;
			}
		}
	}
}


int InflatingStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	if (length == 0 || !_pOstr) return 0;
	
	_zstr.next_in   = (unsigned char*) buffer;
	_zstr.avail_in  = length;
	_zstr.next_out  = (unsigned char*) _buffer;
	_zstr.avail_out = INFLATE_BUFFER_SIZE;
	for (;;)
	{
		int rc = inflate(&_zstr, Z_NO_FLUSH);
		if (rc == Z_STREAM_END)
		{
			_pOstr->write(_buffer, INFLATE_BUFFER_SIZE - _zstr.avail_out);
			if (!_pOstr->good()) throw IOException(zError(rc));
			break;
		}
		if (rc != Z_OK) throw IOException(zError(rc)); 
		if (_zstr.avail_out == 0)
		{
			_pOstr->write(_buffer, INFLATE_BUFFER_SIZE);
			if (!_pOstr->good()) throw IOException(zError(rc));
			_zstr.next_out  = (unsigned char*) _buffer;
			_zstr.avail_out = INFLATE_BUFFER_SIZE;
		}
		if (_zstr.avail_in == 0)
		{
			_pOstr->write(_buffer, INFLATE_BUFFER_SIZE - _zstr.avail_out);
			if (!_pOstr->good()) throw IOException(zError(rc)); 
			_zstr.next_out  = (unsigned char*) _buffer;
			_zstr.avail_out = INFLATE_BUFFER_SIZE;
			break;
		}
	}
	return length;
}


InflatingIOS::InflatingIOS(std::ostream& ostr, InflatingStreamBuf::StreamType type):
	_buf(ostr, type)
{
	poco_ios_init(&_buf);
}


InflatingIOS::InflatingIOS(std::istream& istr, InflatingStreamBuf::StreamType type):
	_buf(istr, type)
{
	poco_ios_init(&_buf);
}


InflatingIOS::~InflatingIOS()
{
}


InflatingStreamBuf* InflatingIOS::rdbuf()
{
	return &_buf;
}


InflatingOutputStream::InflatingOutputStream(std::ostream& ostr, InflatingStreamBuf::StreamType type):
	InflatingIOS(ostr, type),
	std::ostream(&_buf)
{
}


InflatingOutputStream::~InflatingOutputStream()
{
}


int InflatingOutputStream::close()
{
	return _buf.close();
}


InflatingInputStream::InflatingInputStream(std::istream& istr, InflatingStreamBuf::StreamType type):
	InflatingIOS(istr, type),
	std::istream(&_buf)
{
}


InflatingInputStream::~InflatingInputStream()
{
}


} // namespace Poco
