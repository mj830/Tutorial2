#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const int BADMINTON_LIMIT = 16;

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming 4 processes for this task
  if (world_size != 4) {
    fprintf(stderr, "This is a four player only game\n");
    exit(EXIT_FAILURE);
  }

  int badminton_count = 0;
  int partner_rank = (world_rank + 1) % world_size;
  int teammate_rank = (world_rank % 2 == 0) ? (world_rank + 2) % world_size : (world_rank + 1) % world_size;

  while (badminton_count < BADMINTON_LIMIT) {
    if (world_rank == badminton_count % world_size) {
      // Increment the badminton count before you send it
      badminton_count++;
      MPI_Send(&badminton_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
      printf("P%d sent rallyCount of %d to P%d\n",
             world_rank, badminton_count, partner_rank);
      MPI_Send(&badminton_count, 1, MPI_INT, teammate_rank, 0, MPI_COMM_WORLD);
      printf("P%d informed P%d\n", world_rank, teammate_rank);
    } else {
      MPI_Recv(&badminton_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      printf("P%d received rallyCount %d from P%d\n",
             world_rank, badminton_count, partner_rank);
      MPI_Recv(&badminton_count, 1, MPI_INT, teammate_rank, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      printf("P%d informed P%d\n", world_rank, teammate_rank);
    }
  }
  MPI_Finalize();
}
