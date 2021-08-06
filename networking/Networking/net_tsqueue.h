#pragma once

#include "net_common.h"

namespace net
{
	template<typename T>
	class tsqueue
	{
	public:
		tsqueue() = default;
		tsqueue(const tsqueue<T>&) = delete;
		virtual ~tsqueue() { clear(); }

	public:
		// returns and maintains item at front of queue
		const T& front()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.front();
		}

		// returns and maintains item at back of queue
		const T& back()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.back();
		}

		// removes and returns item from front of queue
		T pop_front()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(deqQueue.front());
			deqQueue.pop_front();
			return t;
		}

		// removes and returns item from back of queue
		T pop_back()
		{
			std::scoped_lock lock(muxQueue);
			auto t = std::move(deqQueue.back());
			deqQueue.pop_back();
			return t;
		}

		// adds an item to back of queue
		void push_back(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_back(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		// adds an item to front of queue
		void push_front(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_front(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		// returns true if the que has no items
		bool empty()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.empty();
		}

		// returns numver of items in queue
		size_t count()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.size();
		}

		// clears queue
		void clear()
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.clear();
		}

		void wait()
		{
			while (empty())
			{
				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.wait(ul);
			}
		}

	protected:
		std::mutex muxQueue;
		std::deque<T> deqQueue;
		std::condition_variable cvBlocking;
		std::mutex muxBlocking;
	};
}