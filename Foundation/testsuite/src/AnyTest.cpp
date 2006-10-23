//
// AnyTest.cpp
//
// $Id: //poco/1.2/Foundation/testsuite/src/AnyTest.cpp#2 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#include "AnyTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/Any.h"
#include "Poco/Bugcheck.h"
#include <vector>


using namespace Poco;


class SomeClass
{
public:
	int i;
	std::string str;
	SomeClass(int h, std::string s): i (h), str(s)
	{
	};
	bool operator==(const SomeClass& other) const
	{
		return i == other.i && str == other.str;
	}
};


AnyTest::AnyTest(const std::string& name): CppUnit::TestCase(name)
{
}


AnyTest::~AnyTest()
{
}


void AnyTest::testInt()
{
	Any a = 13;
	assert (a.type() == typeid(int));
	int* i = AnyCast<int>(&a);
	assert (*i == 13);
	Any b = a;
	assert (b.type() == typeid(int));
	int *cpyI = AnyCast<int>(&b);
	assert (*cpyI == *i);
	*cpyI = 20;
	assert (*cpyI != *i);
	std::string* s = AnyCast<std::string>(&a);
	assert (s == NULL);

	int tmp = AnyCast<int>(a);
	const Any c = a;
	tmp = AnyCast<int>(a);
}


void AnyTest::testComplexType()
{
	SomeClass str(13,std::string("hello"));
	Any a = str;
	Any b = a;
	assert (a.type() == typeid(SomeClass));
	assert (b.type() == typeid(SomeClass));
	SomeClass str2 = AnyCast<SomeClass>(a);
	assert (str == str2);
	const SomeClass& strCRef = RefAnyCast<SomeClass>(a);
	assert (str == strCRef);
	SomeClass& strRef = RefAnyCast<SomeClass>(a);
	assert (str == strRef);
}


void AnyTest::testVector()
{
	std::vector<int> tmp;
	tmp.push_back(1);
	tmp.push_back(2);
	tmp.push_back(3);
	Any a = tmp;
	assert (a.type() == typeid(std::vector<int>));
	std::vector<int>tmp2 = AnyCast<std::vector<int> >(a);
	const std::vector<int >& vecCRef = RefAnyCast<std::vector<int> >(a);
	std::vector<int >& vecRef = RefAnyCast<std::vector<int> >(a);
	vecRef[0] = 0;
	assert (vecRef[0] == vecCRef[0]);
}


void AnyTest::setUp()
{
}


void AnyTest::tearDown()
{
}


CppUnit::Test* AnyTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("AnyTest");

	CppUnit_addTest(pSuite, AnyTest, testInt);
	CppUnit_addTest(pSuite, AnyTest, testComplexType);
	CppUnit_addTest(pSuite, AnyTest, testVector);

	return pSuite;
}
