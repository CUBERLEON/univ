#include <cstdio>
#include <chrono>
#include <omp.h>
#define NUM_ROWS_A 2000    
#define NUM_COLUMNS_A 2000    
#define NUM_ROWS_B 2000    
#define NUM_COLUMNS_B 2000    

void makeAB();      
void printArray();   

int i, j, k;  
double mat_a[NUM_ROWS_A][NUM_COLUMNS_A];   
double mat_b[NUM_ROWS_B][NUM_COLUMNS_B];   
double mat_result[NUM_ROWS_A][NUM_COLUMNS_B];

int main(int argc, char *argv[])
{
    omp_set_dynamic(0);
    omp_set_num_threads(8);

    makeAB();
    auto start_time = std::chrono::system_clock::now();

#pragma omp parallel for
    for (i = 0; i < NUM_ROWS_A; i++) {        
        for (j = 0; j < NUM_COLUMNS_B; j++) {    
            for (k = 0; k < NUM_ROWS_B; k++) {    
                mat_result[i][j] += (mat_a[i][k] * mat_b[k][j]);
            }
        }
    }
 
    auto end_time = std::chrono::system_clock::now();
    
    printf("\nRunning Time = %lf seconds\n", std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.);

    system("pause");
    
    return 0;
}
void makeAB()
{
    for (i = 0; i < NUM_ROWS_A; i++) {
        for (j = 0; j < NUM_COLUMNS_A; j++) {
            mat_a[i][j] = i + j;
        }
    }
    for (i = 0; i < NUM_ROWS_B; i++) {
        for (j = 0; j < NUM_COLUMNS_B; j++) {
            mat_b[i][j] = i*j;
        }
    }
}
void printArray()
{
    for (i = 0; i < NUM_ROWS_A; i++) {
        printf("\n");
        for (j = 0; j < NUM_COLUMNS_A; j++)
            printf("%8.2f  ", mat_a[i][j]);
    }
    printf("\n\n\n");
    for (i = 0; i < NUM_ROWS_B; i++) {
        printf("\n");
        for (j = 0; j < NUM_COLUMNS_B; j++)
            printf("%8.2f  ", mat_b[i][j]);
    }
    printf("\n\n\n");
    for (i = 0; i < NUM_ROWS_A; i++) {
        printf("\n");
        for (j = 0; j < NUM_COLUMNS_B; j++)
            printf("%8.2f  ", mat_result[i][j]);
    }
    printf("\n\n");
}