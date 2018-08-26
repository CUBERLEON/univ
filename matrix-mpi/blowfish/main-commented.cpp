#include<stdio.h>
#include<mpi.h>
#define NUM_ROWS_A 2000 
#define NUM_COLUMNS_A 2000 
#define NUM_ROWS_B 2000 
#define NUM_COLUMNS_B 2000 
#define MASTER_TO_SLAVE_TAG 1 //помітки повідомлень тегів з головного потоку до побічних
#define SLAVE_TO_MASTER_TAG 4 //помітки повідомлень з побічних до головного потоку
void makeAB(); 
void printArray();
int rank; //змінна визначає номер потоку
int size; 
int i, j, k; 
double mat_a[NUM_ROWS_A][NUM_COLUMNS_A]; //створення матриці А
double mat_b[NUM_ROWS_B][NUM_COLUMNS_B]; //створення матриці В
double mat_result[NUM_ROWS_A][NUM_COLUMNS_B]; //матриця - результат
double start_time; 
double end_time;
int low_bound; //нижня межа рядків призначені побічному потоку
int upper_bound; //верхня межа рядків призначених побічному потоку
int portion; //кількість рядків призначених побічному потоку
MPI_Status status;
MPI_Request request;
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //отримання номеру потоку
    MPI_Comm_size(MPI_COMM_WORLD, &size); //знайти кількість потоків
                                          
    if (rank == 0) {
        makeAB();
        start_time = MPI_Wtime();
        for (i = 1; i < size; i++) {//для кожного потоку крім головного
            portion = (NUM_ROWS_A / (size - 1)); // кількість рядків, що обробляються побічними потоками
            low_bound = (i - 1) * portion;
            if (((i + 1) == size) && ((NUM_ROWS_A % (size - 1)) != 0)) {//якщо не можемо розділити націло кількість рядків на кількість потоків
                upper_bound = NUM_ROWS_A; //побічний потік оброблятиме всі рядки, що залишилися
            }
            else {
                upper_bound = low_bound + portion; // якщо рядки діляться націло на кількість потоків
            }
            //надсилаємо нижню межу
            MPI_Isend(&low_bound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &request);
            //надсилаємо верхню межу 
            MPI_Isend(&upper_bound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, &request);
            //надсилаємо порцію рядків
            MPI_Isend(&mat_a[low_bound][0], (upper_bound - low_bound) * NUM_COLUMNS_A, MPI_DOUBLE, i, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, &request);
        }
    }
    //надсилаємо матрицю В - стовпчики
    MPI_Bcast(&mat_b, NUM_ROWS_B*NUM_COLUMNS_B, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
    if (rank > 0) {
        //отримуємо нижню межу
        MPI_Recv(&low_bound, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &status);
        //отримуємо верхню межу
        MPI_Recv(&upper_bound, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, &status);
        //отримуємо порцію рядків матриці А
        MPI_Recv(&mat_a[low_bound][0], (upper_bound - low_bound) * NUM_COLUMNS_A, MPI_DOUBLE, 0, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, &status);
        for (i = low_bound; i < upper_bound; i++) {//ітерація рядків матриці А
            for (j = 0; j < NUM_COLUMNS_B; j++) {//ітерація стовпців матриці В
                for (k = 0; k < NUM_ROWS_B; k++) {//ітерація рядків матриці В
                    mat_result[i][j] += (mat_a[i][k] * mat_b[k][j]);
                }
            }
        }
        //надсилаємо назад нижню межу до головного потоку
        MPI_Isend(&low_bound, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, &request);
        //надсилаємо верхню межу до головного потоку
        MPI_Isend(&upper_bound, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, &request);
        //надсилаємо порцію результуючої матриці
        MPI_Isend(&mat_result[low_bound][0], (upper_bound - low_bound) * NUM_COLUMNS_B, MPI_DOUBLE, 0, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, &request);
    }
 //результат 
    if (rank == 0) {
        //збираємо результати
        for (i = 1; i < size; i++) {
            // отримуємо нижню межу з побічних потоків
            MPI_Recv(&low_bound, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, &status); 
            //отримуємо верхню межу з потоків
            MPI_Recv(&upper_bound, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, &status); 
            //отримуємо порцію з побічних потоків
            MPI_Recv(&mat_result[low_bound][0], (upper_bound - low_bound) * NUM_COLUMNS_B, MPI_DOUBLE, i, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, &status);
        }
        end_time = MPI_Wtime();
        printf("\nRunning Time = %f\n\n", end_time - start_time);
        //printArray();
    }
    MPI_Finalize(); //finalize MPI operations
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