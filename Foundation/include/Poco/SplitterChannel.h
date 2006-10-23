//
// SplitterChannel.h
//
// $Id: //poco/1.2/Foundation/include/Poco/SplitterChannel.h#2 $
//
// Library: Foundation
// Package: Logging
// Module:  SplitterChannel
//
// Definition of the SplitterChannel class.
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


#ifndef Foundation_SplitterChannel_INCLUDED
#define Foundation_SplitterChannel_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Channel.h"
#include "Poco/Mutex.h"
#include <vector>


namespace Poco {


class Foundation_API SplitterChannel: public Channel
	/// This channel sends a message to multiple
	/// channels simultaneously.
{
public:
	SplitterChannel();
		/// Creates the SplitterChannel.

	void addChannel(Channel* pChannel);
		/// Attaches a channel, which may not be null.
		
	void removeChannel(Channel* pChannel);
		/// Removes a channel.

	void log(const Message& msg);
		/// Sends the given Message to all
		/// attaches channels. 

	void setProperty(const std::string& name, const std::string& value);
		/// Sets or changes a configuration property.
		///
		/// Only the "channel" property is supported, which allows
		/// adding a comma-separated list of channels via the LoggingRegistry.
		/// The "channel" property is set-only.
		/// To simplify file-based configuration, all property
		/// names starting with "channel" are treated as "channel".

	void close();
		/// Removes all channels.
		
	int count() const;
		/// Returns the number of channels in the SplitterChannel.

protected:
	~SplitterChannel();

private:
	typedef std::vector<Channel*> ChannelVec;
	
	ChannelVec        _channels;
	mutable FastMutex _mutex;
};


} // namespace Poco


#endif // Foundation_SplitterChannel_INCLUDED
