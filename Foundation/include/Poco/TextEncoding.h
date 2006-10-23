//
// TextEncoding.h
//
// $Id: //poco/1.2/Foundation/include/Poco/TextEncoding.h#1 $
//
// Library: Foundation
// Package: Text
// Module:  TextEncoding
//
// Definition of the abstract TextEncoding class.
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


#ifndef Foundation_TextEncoding_INCLUDED
#define Foundation_TextEncoding_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


class Foundation_API TextEncoding
	/// An abstract base class for implementing text encodings
	/// like UTF-8 or ISO 8859-1. 
	///
	/// Subclasses must override the characterMap() and convert()
	/// methods.
{
public:
	enum
	{
		MAX_SEQUENCE_LENGTH = 6 /// The maximum character byte sequence length supported.
	};
	
	typedef int CharacterMap[256];
		/// The map[b] member gives information about byte sequences
		/// whose first byte is b.
		/// If map[b] is c where c is >= 0, then b by itself encodes the Unicode scalar value c.
		/// If map[b] is -1, then the byte sequence is malformed.
		/// If map[b] is -n, where n >= 2, then b is the first byte of an n-byte
		/// sequence that encodes a single Unicode scalar value. Byte sequences up
		/// to 6 bytes in length are supported.

	virtual ~TextEncoding();
		/// Destroys the encoding.
			
	virtual const CharacterMap& characterMap() const = 0;
		/// Returns the CharacterMap for the encoding.
		/// The CharacterMap should be kept in a static member. As
		/// characterMap() can be called frequently, it should be
		/// implemented in such a way that it just returns a static
		/// map. If the map is built at runtime, this should be
		/// done in the constructor.
		
	virtual int convert(const unsigned char* bytes) const;
		/// The convert function is used to convert multibyte sequences;
		/// bytes will point to a byte sequence of n bytes where 
		/// getCharacterMap()[*bytes] == -n.
		///
		/// The convert function must return the Unicode scalar value
		/// represented by this byte sequence or -1 if the byte sequence is malformed.
		/// The default implementation returns (int) bytes[0].

	virtual int convert(int ch, unsigned char* bytes, int length) const;
		/// Transform the Unicode character ch into the encoding's 
		/// byte sequence. The method returns the number of bytes
		/// used. The method must not use more than length characters.
		/// Bytes and length can also be null - in this case only the number
		/// of bytes required to represent ch is returned.
		/// If the character cannot be converted, 0 is returned and
		/// the byte sequence remains unchanged.
		/// The default implementation simply returns 0.
};


} // namespace Poco


#endif // Foundation_TextEncoding_INCLUDED
