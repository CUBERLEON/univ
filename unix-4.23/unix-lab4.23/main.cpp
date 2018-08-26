#include <iostream>
#include <cstdio>
#include <thread>
#include <future>
#include <cassert>
#include <mutex>

using namespace std;

mutex m;

void f_safe(int& x)
{
	lock_guard<mutex> lock(m);

	for (int i = 0; i < 1000000; ++i)
	{
		++x;
	}
}

void f_unsafe(int& x)
{
	for (int i = 0; i < 1000000; ++i)
	{
		++x;
	}
}

int main()
{
	int x = 0;

    thread t1(f_safe, ref(x));
	thread t2(f_safe, ref(x));
	
	t1.join();
	t2.join();
	
	cout << x << endl;

	system("pause");

	return EXIT_SUCCESS;
}