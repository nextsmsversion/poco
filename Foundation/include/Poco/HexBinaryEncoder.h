//
// HexBinaryEncoder.h
//
// $Id: //poco/1.2/Foundation/include/Poco/HexBinaryEncoder.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  HexBinary
//
// Definition of the HexBinaryEncoder class.
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


#ifndef Foundation_HexBinaryEncoder_INCLUDED
#define Foundation_HexBinaryEncoder_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/UnbufferedStreamBuf.h"
#include <ostream>


namespace Poco {


class Foundation_API HexBinaryEncoderBuf: public UnbufferedStreamBuf
	/// This streambuf encodes all data written
	/// to it in hexBinary encoding and forwards it to a connected
	/// ostream. 
	/// In hexBinary encoding, each binary octet is encoded as a character tuple,  
	/// consisting of two hexadecimal digits ([0-9a-fA-F]) representing the octet code.
	/// See also: XML Schema Part 2: Datatypes (http://www.w3.org/TR/xmlschema-2/),
	/// section 3.2.15.
{
public:
	HexBinaryEncoderBuf(std::ostream& ostr);
	~HexBinaryEncoderBuf();
	int close();
	
	void setLineLength(int lineLength);
	int getLineLength() const;
	void setUppercase(bool flag = true);
	
private:
	int writeToDevice(char c);

	int _pos;
	int _lineLength;
	int _uppercase;
	std::ostream& _ostr;
};


class Foundation_API HexBinaryEncoderIOS: public virtual std::ios
	/// The base class for HexBinaryEncoder.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	HexBinaryEncoderIOS(std::ostream& ostr);
	~HexBinaryEncoderIOS();
	int close();
	HexBinaryEncoderBuf* rdbuf();

protected:
	HexBinaryEncoderBuf _buf;
};


class Foundation_API HexBinaryEncoder: public HexBinaryEncoderIOS, public std::ostream
	/// This ostream encodes all data
	/// written to it in BinHex encoding and forwards it to
	/// a connected ostream.
	/// Always call close() when done
	/// writing data, to ensure proper
	/// completion of the encoding operation.
	/// In hexBinary encoding, each binary octet is encoded as a character tuple,  
	/// consisting of two hexadecimal digits ([0-9a-fA-F]) representing the octet code.
	/// See also: XML Schema Part 2: Datatypes (http://www.w3.org/TR/xmlschema-2/),
	/// section 3.2.15.
{
public:
	HexBinaryEncoder(std::ostream& ostr);
	~HexBinaryEncoder();
};


} // namespace Poco


#endif // Foundation_HexBinaryEncoder_INCLUDED
