#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int i;
    const int count = 3;
    const int blocklength = 3;
    const int stride = 6;
    int data[15];
    MPI_Datatype vector_type;
    MPI_Type_vector(count, blocklength, stride, MPI_INT, &vector_type);
    MPI_Type_commit(&vector_type);
    if (rank == 0)
    {
        for (i = 0; i < 15; i++)
        {
            data[i] = i + 1;
        }

        MPI_Send(data, 1, vector_type, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent data: ");
        for (i = 0; i < 15; i++)
        {
            printf("%d ", data[i]);
        }
        printf("\n");
    }
    else if (rank == 1)
    {
        for (i = 0; i < 15; i++)
        {
            data[i] = 0;
        }
        MPI_Recv(data, 1, vector_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: ");
        for (i = 0; i < 15; i++)
        {
            printf("%d ", data[i]);
        }
        printf("\n");
    }
    MPI_Type_free(&vector_type);
    MPI_Finalize();

    return 0;
}
