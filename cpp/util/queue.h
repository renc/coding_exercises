#pragma once 

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

#include <future>
#include <functional>
#include <type_traits>
#include <vector>
#include <thread>
#include <cassert>

// two different concepts: 
// unbound and bounded: about the size, can be extend or not. 
// block and unblock: does it need to wait or it can return immediate 
namespace renc {

	namespace vorb {
	https://github.com/mvorbrodt/blog/blob/master/src/queue.hpp
		template <typename T>
		class Queue {
		public:
			// no blocking
			void push(T element) {
				std::lock_guard<std::mutex> lck(m_mtx);
				m_queue.push(element);
				m_cv.notify_all();
			}
			// can be blocking 
			void pop(T& element) {
				{
					std::unique_lock<std::mutex> lck(m_mtx);
					m_cv.wait(lck, [&] { return m_queue.empty() == false; });
				}
				element = m_queue.front();
				m_queue.pop();
			}
			// no blocking
			bool try_pop(T& element) {
				std::lock_guard<std::mutex> lck(m_mtx); // ?? maybe blocking here?? not easy to implement, maybe use atomic<int> count?
				if (m_queue.empty()) rerturn false;

				element = m_queue.front();
				m_queue.pop();
				return true;
			}
			bool empty() const {
				std::lock_guard<std::mutex> lck(m_mtx);
				return m_queue.empty();
			}
		private:
			std::queue<T> m_queue;
			std::mutex m_mtx;
			std::condition_variable m_cv;
		};

		// https://vorbrodt.blog/2019/02/03/blocking-queue/
		// copy the implement here. semaphore
		class semaphore
		{
		public:
			semaphore(unsigned int count) : m_count(count) { static_assert(m_count > -1); }
			semaphore(const semaphore&&) = delete;
			semaphore(semaphore&&) = delete;
			semaphore& operator = (const semaphore&) = delete;
			semaphore& operator = (semaphore&&) = delete;
			~semaphore() = default;

			void post()
			{
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					++m_count;
				}
				m_cv.notify_one();
			}

			void wait()
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.wait(lock, [&] { return m_count > 0; }); // == while (m_count <=0) { m_cv.wait(lock); }
				--m_count;
			} // wait until m_count > 0

		private:
			std::mutex m_mutex;
			std::condition_variable m_cv;
			unsigned int m_count;
		};

		template<typename T>
		class blocking_queue
		{
		public:
			blocking_queue(unsigned int size)
				: m_size(size), m_pushIndex(0), m_popIndex(0), m_count(0),
				m_data((T*)operator new(size * sizeof(T))),
				m_openSlots(size), m_fullSlots(0) {}

			blocking_queue(const blocking_queue&) = delete;
			blocking_queue(blocking_queue&&) = delete;
			blocking_queue& operator = (const blocking_queue&) = delete;
			blocking_queue& operator = (blocking_queue&&) = delete;

			~blocking_queue()
			{
				while (m_count--)
				{
					m_data[m_popIndex].~T();
					m_popIndex = ++m_popIndex % m_size;
				}
				operator delete(m_data);
			}

			void push(const T& item)
			{
				m_openSlots.wait(); // wait until there is open slots
				{
					std::lock_guard<std::mutex> lock(m_cs);
					new (m_data + m_pushIndex) T(item);
					m_pushIndex = ++m_pushIndex % m_size;
					++m_count;
				}
				m_fullSlots.post(); // increase the available elements.
			}

			void pop(T& item)
			{
				m_fullSlots.wait(); // wait until there is popable element.
				{
					std::lock_guard<std::mutex> lock(m_cs);
					item = m_data[m_popIndex];
					m_data[m_popIndex].~T();
					m_popIndex = ++m_popIndex % m_size;
					--m_count;
				}
				m_openSlots.post(); // increase the open slots.
			}

			bool empty()
			{
				std::lock_guard<std::mutex> lock(m_cs);
				return m_count == 0;
			}

		private:
			unsigned int m_size;
			unsigned int m_pushIndex;
			unsigned int m_popIndex;
			unsigned int m_count;
			T* m_data;

			renc::semaphore m_openSlots;
			renc::semaphore m_fullSlots;
			std::mutex m_cs;
		};

		// https://vorbrodt.blog/2019/02/12/simple-thread-pool/  thread pool  
		class thread_pool
		{
		public:
			thread_pool(unsigned int queueDepth = std::thread::hardware_concurrency(),
				size_t threads = std::thread::hardware_concurrency())
				:m_workQueue(queueDepth)
			{
				assert(queueDepth != 0);
				assert(threads != 0);
				for (size_t i = 0; i < threads; ++i) {
					m_threads.emplace_back(
						std::thread([this]() {

						while (true) {
							auto workItem = m_workQueue.pop();
							if (workItem == nullptr) {
								m_workQueue.push(nullptr);
								break;
							}
							workItem();
						}
					}
					));
				}
			}
			using Proc = std::function<void(void)>;
		private:
			using ThreadPool = std::vector<std::thread>;
			ThreadPool m_threads;
			blocking_queue<Proc> m_workQueue;
		};
	} // namespace of vorb 

	namespace force {
		class Task {
		public:
			virtual ~Task() {}
			void execute() = 0; 
		};
		class Thread {
		public:
			void run() {
				while (m_bAskToStop == false)
			} 
			std::atomic_bool m_bAskToStop = false;
		};
		class ThreadPool {
		private:

		};
	} // namespace of force 
}; // namespace renc 