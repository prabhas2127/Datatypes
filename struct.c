#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

typedef struct {

    int a;
    double b;
    char c;

} my_struct;


int main(int argc, char **argv){
  
    MPI_Init(&argc,&argv);
   
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size < 2){
        fprintf(stderr, "word size must be greater than 1 for this exxample...\n");
        MPI_Abort(MPI_COMM_WORLD, 1);

    }

    my_struct data;
    MPI_Datatype struct_type;


    int blocklengths[3] = {1,1,1};
    MPI_Aint displacements[3];
    MPI_Datatype types[3] = {MPI_INT, MPI_DOUBLE, MPI_CHAR};

    
   displacements[0] = offsetof(my_struct, a);
   displacements[1] = offsetof(my_struct, b);
   displacements[2] = offsetof(my_struct, c);
   
   MPI_Type_create_struct(3, blocklengths, displacements, types, &struct_type);
   MPI_Type_commit(&struct_type);

   if(rank == 0){
   
       data.a = 69;
       data.b = 3.17;
       data.c = 'P';
   
       MPI_Send(&data, 1, struct_type, 1, 0, MPI_COMM_WORLD);
       printf("process 0 sent struct : { a : %d, b : %.2f, c : %c}\n", data.a, data.b, data.c);

   }else if(rank == 1){
      
       MPI_Recv(&data, 1, struct_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       printf("process 1 received struct : { a : %d, b : %.2f, c : %c}\n", data.a, data.b, data.c);
   }

   MPI_Type_free(&struct_type);
   MPI_Finalize();
   return 0;


}
