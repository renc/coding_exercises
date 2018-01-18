#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include <queue>
#include <cassert>

using namespace std;

class UserData
{
public:
	string msg;
	int year; 
};
enum MSGType
{
	MSG_POST_USER_DATA, 
	MSG_TIMER, 
	MSG_EXIT_THREAD 
};
class ThreadMsg
{
public:
	ThreadMsg(enum MSGType type, const UserData *pMsg) : id(type), msg(pMsg) {};
	MSGType id;
	const UserData *msg;
};
class WorkerThread
{
public:
	/// Constructor
	WorkerThread(const char* threadName) : THREAD_NAME(threadName) {};

	/// Destructor
	~WorkerThread() {};

	/// Called once to create the worker thread
	/// @return TRUE if thread is created. FALSE otherwise.
	bool CreateThread();

	/// Called once a program exit to exit the worker thread
	void ExitThread();

	/// Get the ID of this thread instance
	/// @return The worker thread ID
	std::thread::id GetThreadId() { return  m_thread->get_id(); }

	/// Get the ID of the currently executing thread
	/// @return The current thread ID
	//static std::thread::id GetCurrentThreadId();

	/// Add a message to thread queue.
	/// @param[in] data - thread specific information created on the heap using operator new.
	void PostMsg(const UserData* data);

private:
	WorkerThread(const WorkerThread&);
	WorkerThread& operator=(const WorkerThread&);

	/// Entry point for the worker thread
	void Process();

	/// Entry point for timer thread
	void TimerThread();

	std::thread* m_thread;
	std::queue<ThreadMsg*> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::atomic<bool> m_timerExit;
	const char* THREAD_NAME;
};
bool WorkerThread::CreateThread()
{
	if (!m_thread)
		m_thread = new thread(&WorkerThread::Process, this);
	return true;
}
void WorkerThread::ExitThread()
{
	if (!m_thread)
		return;

	// Create a new ThreadMsg
	ThreadMsg* threadMsg = new ThreadMsg(MSG_EXIT_THREAD, 0);

	// Put exit thread message into the queue
	{
		lock_guard< mutex> lock(m_mutex);
		m_queue.push(threadMsg);
		m_cv.notify_one();
	}

	m_thread->join();
	delete m_thread;
	m_thread = 0;
}
void WorkerThread::PostMsg(const UserData* data)
{
	assert(m_thread);

	ThreadMsg* threadMsg = new ThreadMsg(MSG_POST_USER_DATA, data);

	// Add user data msg to queue and notify worker thread
	std::unique_lock< std::mutex> lk(m_mutex);
	m_queue.push(threadMsg);
	m_cv.notify_one();
}
void WorkerThread::Process()
{
	m_timerExit = false;
	std::thread timerThread(&WorkerThread::TimerThread, this);

	while (1)
	{
		ThreadMsg* msg = 0;
		{
			// Wait for a message to be added to the queue
			std::unique_lock< std::mutex> lk(m_mutex);
			while (m_queue.empty())
				m_cv.wait(lk);

			if (m_queue.empty())
				continue;

			msg = m_queue.front();
			m_queue.pop();
		}

		switch (msg->id)
		{
		case MSG_POST_USER_DATA:
		{
			assert(msg->msg != NULL);

			// Convert the ThreadMsg void* data back to a UserData*
			const UserData* userData = static_cast< const UserData*>(msg->msg);

			cout << userData->msg.c_str() << " " << userData->year << " on " << THREAD_NAME << endl;

			// Delete dynamic data passed through message queue
			delete userData;
			delete msg;
			break;
		}

		case MSG_TIMER:
			cout << "Timer expired on " << THREAD_NAME << endl;
			delete msg;
			break;

		case MSG_EXIT_THREAD:
		{
			m_timerExit = true;
			timerThread.join();

			delete msg;
			std::unique_lock< std::mutex> lk(m_mutex);
			while (!m_queue.empty())
			{
				msg = m_queue.front();
				m_queue.pop();
				delete msg;
			}

			cout << "Exit thread on " << THREAD_NAME << endl;
			return;
		}

		default:
			assert(false);
		}
	}
}
void WorkerThread::TimerThread()
{
	while (!m_timerExit)
	{
		// Sleep for 250ms then put a MSG_TIMER message into queue
		std::this_thread::sleep_for(250ms);

		ThreadMsg* threadMsg = new ThreadMsg(MSG_TIMER, 0);

		// Add timer msg to queue and notify worker thread
		std::unique_lock< std::mutex> lk(m_mutex);
		m_queue.push(threadMsg);
		m_cv.notify_one();
	}
}

// renc: two worker thread, each has an event loop for processing.
// 
// Worker thread instances
WorkerThread workerThread1("WorkerThread1");
WorkerThread workerThread2("WorkerThread2");

int main(void)
{
	// Create worker threads
	workerThread1.CreateThread();
	workerThread2.CreateThread();

	// Create message to send to worker thread 1
	UserData* userData1 = new UserData();
	userData1->msg = "Hello world";
	userData1->year = 2017;

	// Post the message to worker thread 1
	workerThread1.PostMsg(userData1);

	// Create message to send to worker thread 2
	UserData* userData2 = new UserData();
	userData2->msg = "Goodbye world";
	userData2->year = 2017;

	// Post the message to worker thread 2
	workerThread2.PostMsg(userData2);

	// Give time for messages processing on worker threads
	this_thread::sleep_for(1s);

	workerThread1.ExitThread();
	workerThread2.ExitThread();

	return 0;
}