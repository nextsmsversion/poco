//
// FileStreamFactory.h
//
// $Id: //poco/1.2/Foundation/include/Poco/FileStreamFactory.h#1 $
//
// Library: Foundation
// Package: URI
// Module:  FileStreamFactory
//
// Definition of the FileStreamFactory class.
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


#ifndef Foundation_FileStreamFactory_INCLUDED
#define Foundation_FileStreamFactory_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/URIStreamFactory.h"


namespace Poco {


class Path;


class Foundation_API FileStreamFactory: public URIStreamFactory
	/// An implementation of the URIStreamFactory interface
	/// that handles file URIs.
{
public:
	FileStreamFactory();
		/// Creates the FileStreamFactory.

	~FileStreamFactory();
		/// Destroys the FileStreamFactory.
		
	std::istream* open(const URI& uri);
		/// Creates and opens a file stream in binary mode for the given URI.
		/// The URI must be either a file URI or a relative URI reference
		/// containing a path to a local file.
		///
		/// Throws an FileNotFound exception if the file cannot
		/// be opened.
		
	std::istream* open(const Path& path);
		/// Creates and opens a file stream in binary mode for the given path.
		///
		/// Throws an FileNotFound exception if the file cannot
		/// be opened.
};


} // namespace Poco


#endif // Foundation_FileStreamFactory_INCLUDED
