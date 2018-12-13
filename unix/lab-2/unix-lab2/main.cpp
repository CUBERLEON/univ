#include <iostream>
#include <cstdio>
#include <thread>
#include <future>

using namespace std;

int f(int& x)
{
	this_thread::sleep_for(200ms);

	return x & 1;
}

int g(int& x)
{
	this_thread::sleep_for(10s);

	return x & 14;
}

int main()
{
	int x;
	cin >> x;

	future<int> res_f = async(&f, std::ref(x));
	future<int> res_g = async(&g, std::ref(x));

	while (res_f.wait_for(3s) != future_status::ready ||
		   res_g.wait_for(3s) != future_status::ready)
	{
		cout << "Waiting is taking too long. Continue? (y/n)" << endl;

		char ans;
		cin >> ans;
		if (ans != 'y')
		{
			exit(EXIT_FAILURE);
		}

		cout << "Waiting..." << endl;
	}

	cout << "Finished: " << (res_f.get() || res_g.get()) << endl;

	system("pause");

	return EXIT_SUCCESS;
}