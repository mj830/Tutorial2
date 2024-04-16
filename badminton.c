#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int token;
  // Receive from the lower process and send to the higher process. Take care
  // of the special case when you are the first process to prevent deadlock.
  if (world_rank != 0) {
    MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received token %d from process %d\n", world_rank, token,
           world_rank - 1);
    // Inform the teammate in the other group
    int teammate = (world_rank % 2 == 0) ? world_rank - 2 : world_rank + 2;
    printf("Process %d informed its teammate (process %d) that it received token %d\n", world_rank, teammate, token);
  } else {
    // Set the token's value if you are process 0
    token = -1;
  }
  MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0,
           MPI_COMM_WORLD);
  printf("Process %d sent token %d to process %d\n", world_rank, token,
         (world_rank + 1) % world_size);
  // Inform the teammate in the other group
  int teammate = (world_rank % 2 == 0) ? world_rank - 2 : world_rank + 2;
  printf("Process %d informed its teammate (Process %d) that it sent token %d\n", world_rank, teammate, token);
  
  // Now process 0 can receive from the last process. This makes sure that at
  // least one MPI_Send is initialized before all MPI_Recvs (again, to prevent
  // deadlock)
  if (world_rank == 0) {
    MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received token %d from process %d\n", world_rank, token,
           world_size - 1);
    // Inform the teammate in the other group
    int teammate = (world_rank % 2 == 0) ? world_rank - 2 : world_rank + 2;
    printf("Process %d informed its teammate (Process %d) that it received token %d\n", world_rank, teammate, token);
  }
  MPI_Finalize();
}
