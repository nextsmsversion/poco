//
// MD4Engine.h
//
// $Id: //poco/1.2/Foundation/include/Poco/MD4Engine.h#1 $
//
// Library: Foundation
// Package: Crypt
// Module:  MD4Engine
//
// Definition of class MD4Engine.
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
//
// MD4 (RFC 1320) algorithm:
// Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
// rights reserved.
//
// License to copy and use this software is granted provided that it
// is identified as the "RSA Data Security, Inc. MD4 Message-Digest
// Algorithm" in all material mentioning or referencing this software
// or this function.
//
// License is also granted to make and use derivative works provided
// that such works are identified as "derived from the RSA Data
// Security, Inc. MD4 Message-Digest Algorithm" in all material
// mentioning or referencing the derived work.
//
// RSA Data Security, Inc. makes no representations concerning either
// the merchantability of this software or the suitability of this
// software for any particular purpose. It is provided "as is"
// without express or implied warranty of any kind.
//
// These notices must be retained in any copies of any part of this
// documentation and/or software.
//


#ifndef Foundation_MD4Engine_INCLUDED
#define Foundation_MD4Engine_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/DigestEngine.h"


namespace Poco {


class Foundation_API MD4Engine: public DigestEngine
	/// This class implementes the MD4 message digest algorithm,
	/// described in RFC 1320.
{
public:
	enum
	{
		BLOCK_SIZE  = 64,
		DIGEST_SIZE = 16
	};

	MD4Engine();
	~MD4Engine();
		
	unsigned digestLength() const;
	void reset();
	const DigestEngine::Digest& digest();

protected:
	void updateImpl(const void* data, unsigned length);

private:
	static void transform(UInt32 state[4], const unsigned char block[64]);
	static void encode(unsigned char* output, const UInt32* input, unsigned int len);
	static void decode(UInt32* output, const unsigned char* input, unsigned int len);

	struct Context
	{
		UInt32 state[4];          // state (ABCD)
		UInt32 count[2];          // number of bits, modulo 2^64 (lsb first)
		unsigned char buffer[64]; // input buffer
	};

	Context _context;
	DigestEngine::Digest _digest;
};


} // namespace Poco


#endif // Foundation_MD5Engine_INCLUDED
