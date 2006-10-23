//
// TeeStreamTest.cpp
//
// $Id: //poco/1.2/Foundation/testsuite/src/TeeStreamTest.cpp#1 $
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#include "TeeStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/TeeStream.h"
#include <sstream>


using Poco::TeeInputStream;
using Poco::TeeOutputStream;


TeeStreamTest::TeeStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


TeeStreamTest::~TeeStreamTest()
{
}


void TeeStreamTest::testTeeInputStream()
{
	std::istringstream istr("foo");
	std::ostringstream ostr;
	TeeInputStream tis(istr);
	tis.addStream(ostr);
	std::string s;
	tis >> s;
	assert (ostr.str() == "foo");
}


void TeeStreamTest::testTeeOutputStream()
{
	std::ostringstream ostr1;
	std::ostringstream ostr2;
	TeeOutputStream tos(ostr1);
	tos.addStream(ostr2);
	tos << "bar" << std::flush;
	assert (ostr1.str() == "bar");
	assert (ostr2.str() == "bar");
}


void TeeStreamTest::setUp()
{
}


void TeeStreamTest::tearDown()
{
}


CppUnit::Test* TeeStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TeeStreamTest");

	CppUnit_addTest(pSuite, TeeStreamTest, testTeeInputStream);
	CppUnit_addTest(pSuite, TeeStreamTest, testTeeOutputStream);

	return pSuite;
}
