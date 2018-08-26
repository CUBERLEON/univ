#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <cstdio>
#include <mpi.h>
#include <chrono>
#include <cstdlib>
#include <cassert>

#include "blowfish.h"

#define MAX_TEXT_LEN (10000 + 1)
#define ITERATIONS_CNT 1000000

using namespace std;

int main(int argc, char** argv)
{
    CBlowFish blowfish;

    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int elements_per_proc = 0;

    char text_ptr[MAX_TEXT_LEN];
    memset(text_ptr, 0, MAX_TEXT_LEN);

    int text_len = 0;

    blowfish.Initialize((BYTE*)"23452345", 8);

    if (world_rank == 0)
    {
        printf("Text: ");
        flush(cout);
        gets_s(text_ptr);
        //scanf("%s", &text_ptr);
        text_len = strlen(text_ptr);

        elements_per_proc = (text_len + world_size - 1) / world_size;
    }

    MPI_Bcast(&text_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&elements_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(&text_ptr, text_len, MPI_BYTE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    //printf("proc %d: %d %d %s\n", world_rank, text_len, elements_per_proc, text_ptr);
    //flush(cout);

    char* sub_text = new char[elements_per_proc + 1];
    assert(sub_text != NULL);
    memset(sub_text, 0, elements_per_proc + 1);

    MPI_Scatter(text_ptr, elements_per_proc, MPI_BYTE, sub_text, elements_per_proc, MPI_BYTE, 0, MPI_COMM_WORLD);

    //printf("rec %d: %d %s\n", world_rank, strlen(sub_text), sub_text);
    //flush(cout);

    char enc_sub_text[MAX_TEXT_LEN];
    memset(enc_sub_text, 0, MAX_TEXT_LEN);
    char dec_sub_text[MAX_TEXT_LEN];
    memset(dec_sub_text, 0, MAX_TEXT_LEN);

    for (int i = 0; i < ITERATIONS_CNT; ++i)
    {
        blowfish.Encode(sub_text, enc_sub_text, strlen(sub_text));
        blowfish.Decode(enc_sub_text, dec_sub_text, strlen(enc_sub_text));
    }

    char* result = nullptr;
    if (world_rank == 0)
    {
        result = new char[MAX_TEXT_LEN];
        memset(result, 0, sizeof(char) * (MAX_TEXT_LEN));
        assert(result != NULL);
    }

    //printf("gather %d: %d %s\n", world_rank, strlen(dec_sub_text), dec_sub_text);
    //flush(cout);
    MPI_Gather(dec_sub_text, elements_per_proc, MPI_BYTE, result, elements_per_proc, MPI_BYTE, 0, MPI_COMM_WORLD);
    
    if (world_rank == 0)
    {
        double end = MPI_Wtime();

        printf("Elapsed time: %lf s\n", end - start);
        printf("Decoded text: %d %s\n", strlen(result), result);
        flush(cout);
    }

    if (world_rank == 0)
    {
        delete[] result;
    }
    delete[] sub_text;

    MPI_Finalize();

    return 0;
}