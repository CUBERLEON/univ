#include <iostream>
#include <cstdio>
#include <thread>
#include <future>
#include <cassert>

using namespace std;

mutex cout_lock;

void print(const vector<vector<int>>& m)
{
	for (int i = 0; i < m.size(); ++i)
	{
		for (int j = 0; j < m[i].size(); ++j)
		{
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
}

void multiply(int i, int j, const vector<vector<int>>& a, const vector<vector<int>>& b, vector<vector<int>>& c)
{
	int n;
	assert((n = a[i].size()) == b.size());

	int result = 0;
	for (int k = 0; k < n; ++k)
	{
		result += a[i][k] * b[k][j];
	}
	
	lock_guard<mutex> lock(cout_lock);
	cout << i + 1 << " x " << j + 1 << " = " << result << endl;
	c[i][j] = result;
}

int main()
{
	srand(time(0));

	constexpr int MOD = 100;

	int n, m, k;
	cin >> n >> m >> k;
	
	vector<vector<int>> a(n, vector<int>(m, 0));
	vector<vector<int>> b(m, vector<int>(k, 0));
	vector<vector<int>> c(n, vector<int>(k, 0));

	for_each(a.begin(), a.end(), [=](auto& r) { generate(r.begin(), r.end(), [=]() { return rand() % MOD; }); });
	for_each(b.begin(), b.end(), [=](auto& r) { generate(r.begin(), r.end(), [=]() { return rand() % MOD; }); });

	cout << "A:" << endl;
	print(a);
	cout << "B:" << endl;
	print(b);

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

	vector<thread> threads;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < k; ++j)
		{
			threads.emplace_back(multiply, i, j, cref(a), cref(b), ref(c));
		}
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	cout << "Elapsed time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

	lock_guard<mutex> lock(cout_lock);
	cout << "C = A x B:" << endl;
	print(c);

	system("pause");

	return EXIT_SUCCESS;
}