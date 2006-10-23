//
// HMACEngine.h
//
// $Id: //poco/1.2/Foundation/include/Poco/HMACEngine.h#1 $
//
// Library: Foundation
// Package: Crypt
// Module:  HMACEngine
//
// Definition of the HMACEngine class.
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


#ifndef Foundation_HMACEngine_INCLUDED
#define Foundation_HMACEngine_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/DigestEngine.h"
#include <string.h>


namespace Poco {


template <class Engine>
class HMACEngine: public DigestEngine
	/// This class implementes the HMAC message
	/// authentication code algorithm, as specified
	/// in RFC 2104. The underlying DigestEngine
	/// (MD5Engine, SHA1Engine, etc.) must be given as 
	/// template argument.
	/// Since the HMACEngine is a DigestEngine, it can
	/// be used with the DigestStream class to create
	/// a HMAC for a stream.
{
public:
	enum
	{
		BLOCK_SIZE  = Engine::BLOCK_SIZE,
		DIGEST_SIZE = Engine::DIGEST_SIZE
	};
	
	HMACEngine(const std::string& passphrase)
	{
		init(passphrase.data(), (unsigned) passphrase.length());
	}
	
	HMACEngine(const char* passphrase, unsigned length)
	{
		poco_check_ptr (passphrase);

		init(passphrase, length);
	}
	
	~HMACEngine()
	{
		memset(_ipad, 0, BLOCK_SIZE);
		memset(_opad, 0, BLOCK_SIZE);
		delete [] _ipad;
		delete [] _opad;
	}
		
	unsigned digestLength() const
	{
		return DIGEST_SIZE;
	}
	
	void reset()
	{
		_engine.reset();
		_engine.update(_ipad, BLOCK_SIZE);
	}
	
	const DigestEngine::Digest& digest()
	{
		const DigestEngine::Digest& d = _engine.digest();
		char db[DIGEST_SIZE];
		char* pdb = db;
		for (DigestEngine::Digest::const_iterator it = d.begin(); it != d.end(); ++it)
			*pdb++ = *it;
		_engine.reset();
		_engine.update(_opad, BLOCK_SIZE);
		_engine.update(db, DIGEST_SIZE);
		const DigestEngine::Digest& result = _engine.digest();
		reset();
		return result;
	}

protected:
	void init(const char* passphrase, unsigned length)
	{
		_ipad = new char[BLOCK_SIZE];
		_opad = new char[BLOCK_SIZE];
		memset(_ipad, 0, BLOCK_SIZE);
		memset(_opad, 0, BLOCK_SIZE);
		if (length > BLOCK_SIZE)
		{
			_engine.reset();
			_engine.update(passphrase, length);
			const DigestEngine::Digest& d = _engine.digest();
			char* ipad = _ipad;
			char* opad = _opad;
			int n = BLOCK_SIZE;
			for (DigestEngine::Digest::const_iterator it = d.begin(); it != d.end() && n-- > 0; ++it)
			{
				*ipad++ = *it;
				*opad++ = *it;
			}
		}
		else
		{
			memcpy(_ipad, passphrase, length);
			memcpy(_opad, passphrase, length);
		}
		for (int i = 0; i < BLOCK_SIZE; ++i)
		{
			_ipad[i] ^= 0x36;
			_opad[i] ^= 0x5c;
		}
		reset();
	}
	
	void updateImpl(const void* data, unsigned length)
	{
		_engine.update(data, length);
	}

private:
	HMACEngine();

	Engine _engine;
	char*  _ipad;
	char*  _opad;
};


} // namespace Poco


#endif // Foundation_HMACEngine_INCLUDED
