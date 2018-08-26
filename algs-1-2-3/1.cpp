#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>
#include <string>

constexpr int N = 7, M = 20;
constexpr int MOD = 50;

struct cell
{
    int* data = nullptr;
    cell* next_row = nullptr;
    cell* next_col = nullptr;
};

int table[N][N];
cell multilist[N][N];

cell* last_row = nullptr;
cell* last_cols[N];

int main()
{
    std::srand(std::time(nullptr));

    for (size_t i = 0; i < M; ++i)
    {
        size_t x = std::rand() % N;
        size_t y = std::rand() % N;

        int value = std::rand() % MOD;

        table[x][y] = value;
    }

    

    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            std::cout << "\t" << table[i][j] << " ";
        }
        std::cout << std::endl;
    }

    for (int i = N - 1; i >= 0; --i)
    {
        last_row = nullptr;
        for (int j = N - 1; j >= 0; --j)
        {
            if (!table[i][j]) continue;

            multilist[i][j].data = &table[i][j];
            multilist[i][j].next_row = last_row;
            multilist[i][j].next_col = last_cols[j];

            last_row = &multilist[i][j];
            last_cols[j] = &multilist[i][j];
        }
    }

    std::cout << std::endl;

    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            if (!multilist[i][j].data) continue;

            std::cout << "\t(" << (multilist[i][j].next_row ? std::to_string(*multilist[i][j].next_row->data) : "null") << "," << (multilist[i][j].next_col ? std::to_string(*multilist[i][j].next_col->data) : "null") << ")" << " ";
        }
        std::cout << std::endl;
    }

    system("pause");

    return EXIT_SUCCESS;
}