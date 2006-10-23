//
// TextIterator.h
//
// $Id: //poco/1.2/Foundation/include/Poco/TextIterator.h#1 $
//
// Library: Foundation
// Package: Text
// Module:  TextIterator
//
// Definition of the TextIterator class.
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


#ifndef Foundation_TextIterator_INCLUDED
#define Foundation_TextIterator_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


class TextEncoding;


class Foundation_API TextIterator
	/// An unidirectional iterator for iterating over characters in a string.
	/// The TextIterator uses a TextEncoding object to
	/// work with multi-byte character encodings like UTF-8.
	/// Characters are reported in Unicode.
	///
	/// Example: Count the number of UTF-8 characters in a string.
	///
	///     UTF8Encoding utf8Encoding;
	///     std::string utf8String("....");
	///     TextIterator it(utf8String, utf8Encoding);
	///     TextIterator end(utf8String);
	///     int n = 0;
	///     while (it != end) { ++n; ++it; }
	///
	/// NOTE: When an UTF-16 encoding is used, surrogate pairs will be
	/// reported as two separate characters, due to restrictions of
	/// the TextEncoding class.
{
public:
	TextIterator();
		/// Creates an uninitialized TextIterator.
		
	TextIterator(const std::string& str, const TextEncoding& encoding);
		/// Creates a TextIterator for the given string.
		/// The encoding object must not be deleted as long as the iterator
		/// is in use.

	TextIterator(const std::string::const_iterator& begin, const std::string::const_iterator& end, const TextEncoding& encoding);
		/// Creates a TextIterator for the given range.
		/// The encoding object must not be deleted as long as the iterator
		/// is in use.

	TextIterator(const std::string& str);
		/// Creates an end TextIterator for the given string.

	TextIterator(const std::string::const_iterator& end);
		/// Creates an end TextIterator.

	~TextIterator();
		/// Destroys the TextIterator.
	
	TextIterator(const TextIterator& it);
		/// Copy constructor.
	
	TextIterator& operator = (const TextIterator& it);
		/// Assignment operator.
		
	void swap(TextIterator& it);
		/// Swaps the iterator with another one.
	
	int operator * () const;
		/// Returns the Unicode value of the current character.
		/// If there is no valid character at the current position,
		/// -1 is returned.
		
	TextIterator& operator ++ (); 
		/// Prefix increment operator.

	TextIterator operator ++ (int);		
		/// Postfix increment operator.

	bool operator == (const TextIterator& it) const;
		/// Compares two iterators for equality.
		
	bool operator != (const TextIterator& it) const;
		/// Compares two iterators for inequality.
		
private:
	const TextEncoding*         _pEncoding;
	std::string::const_iterator _it;
	std::string::const_iterator _end;
};


//
// inlines
//
inline bool TextIterator::operator == (const TextIterator& it) const
{
	return _it == it._it;
}


inline bool TextIterator::operator != (const TextIterator& it) const
{
	return _it != it._it;
}


inline void swap(TextIterator& it1, TextIterator& it2)
{
	it1.swap(it2);
}


} // namespace Poco


#endif // Foundation_TextIterator_INCLUDED
