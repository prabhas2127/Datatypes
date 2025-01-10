#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

typedef struct
{

    int PRN, age, marks[4];
    double salary;
    char name[20], email[20];

} student;

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2)
    {
        fprintf(stderr, "word size must be greater than 1 for this exxample...\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    student data;
    MPI_Datatype struct_type;

    int blocklengths[6] = {4, 1, 1, 20, 1, 5};
    MPI_Aint displacements[6];
    MPI_Datatype types[6] = {MPI_CHAR, MPI_INT, MPI_INT, MPI_CHAR, MPI_DOUBLE, MPI_INT};

    displacements[0] = offsetof(student, name);
    displacements[1] = offsetof(student, PRN);
    displacements[2] = offsetof(student, age);
    displacements[3] = offsetof(student, email);
    displacements[4] = offsetof(student, salary);
    displacements[5] = offsetof(student, marks);
    MPI_Type_create_struct(6, blocklengths, displacements, types, &struct_type);
    MPI_Type_commit(&struct_type);

    if (rank == 0)
    {

        strcpy(data.name, "abhi");
        data.PRN = 240840001;
        data.age = 24;
        strcpy(data.email, "abhi@abhi.com");
        data.salary = 10000.01111;
        data.marks[0] = 40;
        data.marks[1] = 39;
        data.marks[2] = 40;
        data.marks[3] = 40;
        MPI_Send(&data, 1, struct_type, 1, 0, MPI_COMM_WORLD);
        printf("process 0 sent struct : \n name : %s,\n PRN : %d,\n age : %d,\n email : %s,\n salary : %2f,\n marks : [%d, %d, %d, %d]\n\n", data.name, data.PRN, data.age, data.email, data.salary, data.marks[0], data.marks[1], data.marks[2], data.marks[3]);
    }
    else if (rank == 1)
    {

        MPI_Recv(&data, 1, struct_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("process 1 received struct : \n name : %s,\n PRN : %d,\n age : %d,\n email : %s,\n salary : %2f,\n marks : [%d, %d, %d, %d]\n\n", data.name, data.PRN, data.age, data.email, data.salary, data.marks[0], data.marks[1], data.marks[2], data.marks[3]);
    }
    MPI_Type_free(&struct_type);
    MPI_Finalize();
    return 0;
}
