#include <stdio.h>
#include <mpi.h>
#define N 3

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size, i, j; 
    int temp=1;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2)
    {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    

    int data[N][N];
    MPI_Datatype contiguous_type;

    MPI_Type_contiguous(N*N,MPI_INT,&contiguous_type);
    MPI_Type_commit(&contiguous_type);
    
    if (rank == 0)
    {int temp=1;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                data[i][j] = temp;
                temp++;
            }

        }    
            MPI_Send(data, 1,contiguous_type, 1, 0, MPI_COMM_WORLD);
            printf("the process matrix is : \n");
            for (i = 0; i < N; i++)
            {
                for (j = 0; j < N; j++)
                {
                    printf("%d\t", data[i][j]);
                }
                printf("\n");
            }
            printf("\n");
        
    }
    else if (rank == 1)
    {
        MPI_Recv(data, 1,contiguous_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: \n");
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                printf("%d\t", data[i][j]);
            }
            printf("\n");
        }

        printf("\n");

    }
    MPI_Type_free(&contiguous_type);
    MPI_Finalize();

    return 0;
}

