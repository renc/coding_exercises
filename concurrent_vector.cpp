
#include <cstdio>
#include <string>
#include <iostream>
#include <ctime>
#include <chrono>
#include <vector> 

#include <concurrent_vector.h>


int iLoopCount = 9999;
struct TestVector {
	std::vector<int> arr; // 没有mtx的话, crash.  vector iterator not incremental
						  //QVector<int> arr; // with mtx, 
	std::mutex g_mtx; // without the lock_guard in the next funcs, crash.
					  //Concurrency::concurrent_vector<int> arr;
	void doAdd() {
		//while (1) 
		{
			for (int i = 0; i < iLoopCount; ++i)
			{
				std::lock_guard<std::mutex> lg(g_mtx);
				arr.push_back(i);
			}
		}
	}
	void doIterate() {
		for (int i = 0; i < iLoopCount; ++i) {
			std::lock_guard<std::mutex> lg(g_mtx);
			for (auto item : arr)
			{
				//std::cerr << item << ", ";//this io does not matter.
			}
		}
	}
};
TestVector testVector;
struct TestVector2 {
	Concurrency::concurrent_vector<int> arr;
	void doAdd() {
		//while (1) 
		{
			for (int i = 0; i < iLoopCount; ++i)
			{
				arr.push_back(i);
			}
		}
	}
	void doIterate() {
		for (int i = 0; i < iLoopCount; ++i) {
			for (auto item : arr)
			{
				//std::cerr << item << ", ";//this io does not matter.
			}
		}
	}
};
TestVector2 testVector2;


int main()
{
	if (1) 
	{
			std::chrono::high_resolution_clock::time_point time0 = std::chrono::high_resolution_clock::now();
			std::thread t1(&TestVector::doAdd, &testVector);
			std::thread t2(&TestVector::doIterate, &testVector);
			t1.join();
			t2.join();
			std::chrono::high_resolution_clock::time_point time1 = std::chrono::high_resolution_clock::now();
			std::thread t3(&TestVector2::doAdd, &testVector2);
			std::thread t4(&TestVector2::doIterate, &testVector2);
			t3.join();
			t4.join();
			std::chrono::high_resolution_clock::time_point time2 = std::chrono::high_resolution_clock::now();
			std::cerr << "Took "
				<< std::chrono::duration_cast<std::chrono::nanoseconds>(time1 - time0).count() << "(nanoseconds).\n";
			std::cerr << "Took "
				<< std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1).count() << "(nanoseconds).\n";
	}
	return 1;
}
