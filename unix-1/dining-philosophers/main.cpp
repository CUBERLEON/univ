#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>

std::mutex cout_mutex;

struct Fork
{
    std::mutex mutex;
};

struct Dinner
{
    std::atomic<bool> ready { false };
    std::array<Fork, 5> forks;
};

class Philosopher
{
	std::mt19937 rng{ std::random_device{}() };

    const Dinner& dinner;
    Fork& left;
    Fork& right;
    std::thread worker;

    void wait();
    void eat();
    void think();

public:
	int eat_cnt = 0;
	const std::string name;

    Philosopher(std::string name, const Dinner& dinn, Fork& l, Fork& r)
        : name(std::move(name))
        , dinner(dinn)
        , left(l)
        , right(r)
        , worker(&Philosopher::wait, this)
    {
    }
    ~Philosopher()
    {
        worker.join();
    }
};

void Philosopher::wait()
{
    while (!dinner.ready);

    while (dinner.ready)
    {
        std::lock(left.mutex, right.mutex);
        eat();
        think();
    }
}

void Philosopher::eat()
{
    std::lock_guard<std::mutex> left_lock(left.mutex, std::adopt_lock);
    std::lock_guard<std::mutex> right_lock(right.mutex, std::adopt_lock);

    std::uniform_int_distribution<> dist(1, 6);
    
    if (!dinner.ready)
        return;
    
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << name << " started eating." << std::endl;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)) * 25);
    
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << name << " finished eating." << std::endl;

		++eat_cnt;
    }
}

void Philosopher::think()
{
    std::uniform_int_distribution<> wait(1, 6);
    std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 75));
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << name << " is thinking." << std::endl;
    }
    if (!dinner.ready)
        return;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 75));
    
    {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << name << " is hungry again!" << std::endl;
    }
}

int main()
{
    Dinner dinner;
    std::array<Philosopher, 5> philosophers
	{ {
		{ "Philosopher #1", dinner, dinner.forks[0], dinner.forks[1] },
		{ "Philosopher #2", dinner, dinner.forks[1], dinner.forks[2] },
		{ "Philosopher #3", dinner, dinner.forks[2], dinner.forks[3] },
		{ "Philosopher #4", dinner, dinner.forks[3], dinner.forks[4] },
		{ "Philosopher #5", dinner, dinner.forks[4], dinner.forks[0] },
    } };
    
	std::cout << "Dinner started!" << std::endl;
    dinner.ready = true;
    
	std::this_thread::sleep_for(std::chrono::seconds(20));
    
	dinner.ready = false;
    
	std::lock_guard<std::mutex> cout_lock(cout_mutex);
    std::cout << "Dinner finished!" << std::endl;

	std::cout << "Statistics:" << std::endl;
	for (auto& philosopher : philosophers)
	{
		std::cout << philosopher.name << ": " << philosopher.eat_cnt << std::endl;
	}
    
    system("pause");

    return 0;
}