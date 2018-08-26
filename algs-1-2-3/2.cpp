#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>

const int N = 100000;
const int M = N / 100;

std::vector< std::list<int> > table;

std::size_t hash(int x)
{
    return x % M;
}

int main()
{
    std::srand(std::time(nullptr));

    table.resize(M);

    for (std::size_t i = 0; i < N; ++i)
    {
        int value = std::rand();

        table[hash(value)].push_back(value);
    }

    std::size_t longest = 0;
    std::size_t shortest = 0;
    
    for (int i = 0; i < M; ++i)
    {
        if (table[i] < table[shortest])
        {
            shortest = i;
        }
        if (table[i] > table[longest])
        {
            longest = i;
        }
    }

    std::cout << "Shortest: value=" << shortest << " cnt=" << table[shortest].size() << std::endl;
    std::cout << "Longest: value=" << longest << " cnt=" << table[longest].size() << std::endl;

    system("pause");

    return EXIT_SUCCESS;
}