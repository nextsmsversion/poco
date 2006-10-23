//
// NotificationQueue.h
//
// $Id: //poco/1.2/Foundation/include/Poco/NotificationQueue.h#1 $
//
// Library: Foundation
// Package: Notifications
// Module:  NotificationQueue
//
// Definition of the NotificationQueue class.
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


#ifndef Foundation_NotificationQueue_INCLUDED
#define Foundation_NotificationQueue_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Mutex.h"
#include "Poco/Event.h"
#include <deque>


namespace Poco {


class Notification;
class NotificationCenter;


class Foundation_API NotificationQueue
	/// A NotificationQueue object provides a way to implement asynchronous
	/// notifications. This is especially useful for sending notifications
	/// from one thread to another, for example from a background thread to 
	/// the main (user interface) thread. 
	/// 
	/// The NotificationQueue can also be used to distribute work from
	/// a controlling thread to one or more worker threads. Each worker thread
	/// repeatedly calls waitDequeueNotification() and processes the
	/// returned notification. Special care must be taken when shutting
	/// down a queue with worker threads waiting for notifications.
	/// The recommended sequence to shut down and destroy the queue is to
	///   1. set a termination flag for every worker thread
	///   2. call the wakeUpAll() method
	///   3. join each worker thread
	///   4. destroy the notification queue.
{
public:
	NotificationQueue();
		/// Creates the NotificationQueue.

	~NotificationQueue();
		/// Destroys the NotificationQueue.

	void enqueueNotification(Notification* pNotification);
		/// Enqueues the given notification by adding it to
		/// the end of the queue (FIFO).
		/// The queue takes ownership of the notification, thus
		/// a call like
		///     notificationQueue.enqueueNotification(new MyNotification);
		/// does not result in a memory leak.
		
	void enqueueUrgentNotification(Notification* pNotification);
		/// Enqueues the given notification by adding it to
		/// the front of the queue (LIFO). The event therefore gets processed
		/// before all other events already in the queue.
		/// The queue takes ownership of the notification, thus
		/// a call like
		///     notificationQueue.enqueueUrgentNotification(new MyNotification);
		/// does not result in a memory leak.

	Notification* dequeueNotification();
		/// Dequeues the next pending notification.
		/// Returns 0 (null) if no notification is available.
		/// The caller gains ownership of the notification and
		/// is expected to release it when done with it.

	Notification* waitDequeueNotification();
		/// Dequeues the next pending notification.
		/// If no notification is available, waits for a notification
		/// to be enqueued. 
		/// The caller gains ownership of the notification and
		/// is expected to release it when done with it.
		/// This method returns 0 (null) if wakeUpWaitingThreads()
		/// has been called by another thread.

	Notification* waitDequeueNotification(long milliseconds);
		/// Dequeues the next pending notification.
		/// If no notification is available, waits for a notification
		/// to be enqueued up to the specified time.
		/// Returns 0 (null) if no notification is available.
		/// The caller gains ownership of the notification and
		/// is expected to release it when done with it.

	void dispatch(NotificationCenter& notificationCenter);
		/// Dispatches all queued notifications to the given
		/// notification center.

	void wakeUpAll();
		/// Wakes up all threads that wait for a notification.
	
	bool empty() const;
		/// Returns true iff the queue is empty.
		
	int size() const;
		/// Returns the number of notifications in the queue.

	void clear();
		/// Removes all notifications from the queue.
		
	bool hasIdleThreads() const;	
		/// Returns true if the queue has at least one thread waiting 
		/// for a notification.
		
	static NotificationQueue& defaultQueue();
		/// Returns a reference to the default
		/// NotificationQueue.

protected:
	Notification* dequeueOne();
	
private:
	typedef std::deque<Notification*> NfQueue;
	struct WaitInfo
	{
		Notification* pNf;
		Event         nfAvailable;
	};
	typedef std::deque<WaitInfo*> WaitQueue;

	NfQueue           _nfQueue;
	WaitQueue         _waitQueue;
	mutable FastMutex _mutex;
};


} // namespace Poco


#endif // Foundation_NotificationQueue_INCLUDED
