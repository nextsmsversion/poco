//
// DynamicFactoryTest.cpp
//
// $Id: //poco/1.2/Foundation/testsuite/src/DynamicFactoryTest.cpp#1 $
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


#include "DynamicFactoryTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/DynamicFactory.h"
#include "Poco/Exception.h"
#include <memory>


using Poco::DynamicFactory;
using Poco::Instantiator;


namespace
{
	class Base
	{
	public:
		Base()
		{
		}
		
		virtual ~Base()
		{
		}
	};
	
	class A: public Base
	{
	};
	
	class B: public Base
	{
	};
}


DynamicFactoryTest::DynamicFactoryTest(const std::string& name): CppUnit::TestCase(name)
{
}


DynamicFactoryTest::~DynamicFactoryTest()
{
}


void DynamicFactoryTest::testDynamicFactory()
{
	DynamicFactory<Base> dynFactory;
	
	dynFactory.registerClass<A>("A");
	dynFactory.registerClass<B>("B");
	
	assert (dynFactory.isClass("A"));
	assert (dynFactory.isClass("B"));
	
	assert (!dynFactory.isClass("C"));
	
	std::auto_ptr<A> a(dynamic_cast<A*>(dynFactory.createInstance("A")));
	std::auto_ptr<B> b(dynamic_cast<B*>(dynFactory.createInstance("B")));
	
	assertNotNull(a.get());
	assertNotNull(b.get());
	
	try
	{
		dynFactory.registerClass<A>("A");
		fail("already registered - must throw");
	}
	catch (Poco::ExistsException&)
	{
	}
	
	dynFactory.unregisterClass("B");
	assert (dynFactory.isClass("A"));
	assert (!dynFactory.isClass("B"));
	
	try
	{
		std::auto_ptr<B> b(dynamic_cast<B*>(dynFactory.createInstance("B")));
		fail("unregistered - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void DynamicFactoryTest::setUp()
{
}


void DynamicFactoryTest::tearDown()
{
}


CppUnit::Test* DynamicFactoryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DynamicFactoryTest");

	CppUnit_addTest(pSuite, DynamicFactoryTest, testDynamicFactory);

	return pSuite;
}
