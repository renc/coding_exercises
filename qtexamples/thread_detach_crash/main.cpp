#include <thread>
#include <iostream>

struct Test {
	bool bActive{ true };

	void update() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		std::cerr << std::this_thread::get_id() << std::endl;
		if (!bActive) // crash here after Test instance is destroyed
			return; 
	}

	Test() {
		std::thread([this] {while (true) update(); }).join();
	}
	~Test() {

	}
};
int main(int argc, char *argv[])
{
	{
		Test test;
	}
	return 0;
}
