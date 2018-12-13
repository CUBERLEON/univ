#include "EMST.h"
#include "Draw.h"

#include <iostream>
#include <chrono>

int main(int argc, char* argv[])
{
    srand(time(NULL));

    int n = 10000;

    auto t1 = std::chrono::high_resolution_clock::now();
    Delaunay del(n);
    QuadList quads = del.GetTriangulation();
    EdgeList mst = del.GetMST();
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Running time (ms): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << std::endl;

    Render(quads, mst);

    return 0;
}