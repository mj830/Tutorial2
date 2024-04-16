// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Four player badminton example with MPI_Send and MPI_Recv. Players pass a
// token in a sequence until it reaches a given value.
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const int PLAYER_LIMIT = 4;

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming 4 processes for this task
  if (world_size != PLAYER_LIMIT) {
    fprintf(stderr, "This is a four player only game\n");
    exit(EXIT_FAILURE);
  }

  int token;
  int next_rank = (world_rank + 1) % PLAYER_LIMIT;
  int prev_rank = (world_rank + PLAYER_LIMIT - 1) % PLAYER_LIMIT;

  while (1) {
    if (world_rank == 0) {
      // Initialize the token's value if you are process 0
      token = -1;
    }

    MPI_Send(&token, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    printf("Player %d sent token %d to Player %d\n",
           world_rank, token, next_rank);

    MPI_Recv(&token, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Player %d received token %d from Player %d\n",
           world_rank, token, prev_rank);

    // Check if the token limit is reached
    if (token >= PLAYER_LIMIT) {
      break;
    }

    // Increment the token before passing it
    token++;
  }

  MPI_Finalize();
}