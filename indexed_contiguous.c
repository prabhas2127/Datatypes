#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define N 20

int main(int argc, char ** argv){

       MPI_Init(&argc, &argv);

       int i, rank, size;
       MPI_Comm_rank(MPI_COMM_WORLD,&rank);
       MPI_Comm_size(MPI_COMM_WORLD,&size);

       if(size < 2){
          fprintf(stderr, "world size must be greater than 1 for this example\n");
          MPI_Abort(MPI_COMM_WORLD, 1);
       }

       const int count = 4;
       const int stride = 5;

       int blocklength[4] = {stride, stride, stride, stride};
       int displacements[4] = {0*stride, 1*stride, 2*stride, 3*stride};

       int data[N];
   
       MPI_Datatype indexed_type;
       MPI_Type_indexed(count, blocklength, displacements, MPI_INT, &indexed_type);
       MPI_Type_commit(&indexed_type);

       if(rank == 0){
          for(i=0;i<N;i++){
              data[i] = i + 1;
          }

          MPI_Send(data, 1, indexed_type, 1, 0, MPI_COMM_WORLD);
          printf("process 0 sent data : ");
          for(i=0;i<N;i++){
              printf("%d ",data[i]);
          }

          printf("\n");

       } else if(rank == 1){
             for(i=0;i<N;i++){
                 data[i] = 0;
             }

             MPI_Recv(data, 1, indexed_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
             printf("process 1 received data : ");
             for(i=0;i<N;i++){
                 printf("%d ",data[i]);
             }

             printf("\n");
       }

       MPI_Type_free(&indexed_type);
       MPI_Finalize();
       return 0;

}
